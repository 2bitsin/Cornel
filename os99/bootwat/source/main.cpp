#include <filesystem>
#include <iostream>
#include <fstream>
#include <format>
#include <vector>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <span>
#include <bit>

#if !defined(_MSC_VER)
  namespace std
  {
    template<typename... T>
    static auto println(std::format_string<T...> fmt, T&&... args) -> void
    {
      std::cout << std::format(fmt, std::forward<T>(args)...) << std::endl;
    }
  }
#else 
  #include <print>
#endif

static constexpr auto LOAD_ADDRESS = 0x7c00u;

#pragma pack(push, 1)
struct HeaderMZ
{
  uint16_t magic_word;
  uint16_t program_size_mod_512;
  uint16_t program_size_div_512;
  uint16_t number_of_relocs;
  uint16_t header_size_div_16;
  uint16_t minimum_heap_div_16;
  uint16_t maximum_heap_div_16;
  uint16_t initial_ss;
  uint16_t initial_sp;
  uint16_t checksum_word;
  uint16_t initial_ip;
  uint16_t initial_cs;
  uint16_t relocation_offset;
  uint16_t overlay_info[2];
};
struct Address
{
  uint16_t off;
  uint16_t seg;
};
static_assert(sizeof(HeaderMZ) == 30);
static_assert(sizeof(Address) == 4);
#pragma pack(pop)

struct ProgramData
{
  std::span<std::byte> binary;
  Address stack{ 0 };
  Address entry{ 0 };
};

static auto validate_path(std::filesystem::path const& path) -> void
{
  using namespace std;
  using namespace std::filesystem;

  if (!exists(path) || !is_regular_file(path)) {
    throw runtime_error(format(
      "{} not found, or is not a regular file.",
      path.string()
    ));
  }
  if (!file_size(path)) {
    throw runtime_error(format(
      "{} is empty.",
      path.string()
    ));
  }
}

static auto load(std::filesystem::path const& path) -> std::vector<std::byte>
{
  using namespace std;
  using namespace std::filesystem;

  auto const length = file_size(path);
  vector<byte> bits(length);
  ifstream file{ path, ios::binary };
  
  file.read((char*)bits.data(), 
    bits.size());
  if (static_cast<uintmax_t>(file.gcount()) < bits.size()) {
    throw runtime_error(format(
      "Failed to read {}.", 
      path.string()
    ));
  }
  return bits;
}

static auto save(std::filesystem::path const& path, auto&&... bits) -> void
  requires requires { { (std::span{ bits }, ...) }; }
{
  using namespace std;
  using namespace std::filesystem;
  static_assert(sizeof...(bits) > 0);
  ofstream sink{ path, ios::binary };
  if (!sink.is_open()) {
    throw runtime_error(format(
      "Can't open {} for writting.",
      path.string()));
  }
  sink.exceptions(ios::failbit | ios::badbit);
  try {
    (([&sink](auto&& bits) {
      static constexpr auto s = sizeof(*data(bits));
      sink.write((char*)data(bits), s * size(bits));
    })(bits), ...);
  }
  catch (exception& ex) {
    throw runtime_error(format(
      "I/O error while writting {} ({}).",
      path.string(), ex.what()));
  }
}

static auto prepare_exec(ProgramData& program, std::span<std::byte> binary, uint16_t load_base) -> void
{
  if (binary.size() <= sizeof(HeaderMZ)) {
    throw std::overflow_error(
      "Binary size invalid.");
  }
  auto& header = *std::bit_cast<HeaderMZ*>(binary.data());
  if (header.magic_word != 0x5A4Du) {
    throw std::runtime_error(
      "Bad binary format.");
  }
  auto const header_size = header.header_size_div_16 * 16;
  auto const relocation_offset = header.relocation_offset;
  auto const relocation_length = header.number_of_relocs;
  if (relocation_offset + relocation_length >= header_size 
    ||relocation_offset + relocation_length >= binary.size()) {
    throw std::runtime_error(
      "Bad binary format.");
  }
  std::span<Address const> reolocations { 
    std::bit_cast<Address const*>(binary.data()+relocation_offset),  
    relocation_length
  };

  program.binary = binary.subspan(header_size);  
  for (auto&& address : reolocations) {
    auto const linear = address.seg*0x10u + address.off;
    auto const target_s = program.binary
      .subspan(linear, sizeof(uint16_t));
    if (target_s.empty()) {
      throw std::runtime_error(
        "Bad binary format.");
    }
    auto& target = *std::bit_cast<uint16_t*>(
      target_s.data());
    std::println("relocating {:08x} : {:04x} -> {:04x}",
      linear, target, target + load_base);
    target += load_base;
  }
  program.stack.seg = header.initial_ss + load_base;
  program.stack.off = header.initial_sp;
  program.entry.seg = header.initial_cs + load_base;
  program.entry.off = header.initial_ip;
}

static auto prepare_stub(ProgramData const& data, std::span<std::byte> bits) -> void 
{
  auto stub_header = std::bit_cast<Address*>(bits.data() + 2u);
  stub_header[0] = data.stack;
  stub_header[1] = data.entry;
}

int main(int argc, char** argv) try
{
  using namespace std;
  using namespace std::filesystem;

  vector<path> args{ argv, argv + argc };


  if (args.size() < 3) {
    std::println("usage: {} <exe path> <out path>\n",
      args[0].filename().string());
    return -1;
  }

#ifndef NDEBUG
  args.emplace_back(args[0].parent_path() / ".." / "test.exe");
  args.emplace_back(args[0].parent_path() / ".." / "test.bin");
#endif
  
  auto const stub_path = args[0].parent_path() / "stub.bin";
  auto const exec_path = args[1];
  auto const outp_path = args[2];

  validate_path(stub_path);
  validate_path(exec_path);

  auto stub_bin = load(stub_path);
  auto exec_bin = load(exec_path);

  ProgramData program;
  auto const load_base = LOAD_ADDRESS / 16 + (stub_bin.size() + 15) / 16;
  prepare_exec(program, exec_bin, load_base);
  std::println("load  address : {:04x}:{:04x}", load_base, 0);
  std::println("entry address : {:04x}:{:04x}", program.entry.seg, program.entry.off);
  std::println("stack address : {:04x}:{:04x}", program.stack.seg, program.stack.off);
  prepare_stub(program, stub_bin);
  save(outp_path, stub_bin, program.binary);
  
  return 0;
}
catch (std::exception const& ex)
{
  std::println("ERROR: {}\n", ex.what());
}