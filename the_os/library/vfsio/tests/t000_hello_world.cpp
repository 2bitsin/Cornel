#include <iterator>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <bit>
#include <span>

#include "expect.hpp"

#include <textio/logger.hpp>
#include <utils/utils.hpp>

#include <vfat/defs.hpp>

using namespace std::string_literals;

struct file_device
{
	file_device(auto&& path_v): m_file(path_v, std::ios::binary) {}

	static constexpr inline std::size_t sector_size = 512u;
	
	auto read(auto& buffer, std::size_t start_v) -> std::size_t
	{		
		const auto size_v = std::size(buffer)*sizeof(*std::data(buffer));
		assert(size_v >= sector_size && !(size_v % sector_size));
		start_v *= sector_size;
		m_file.seekg(start_v);
		assert(m_file.good());
		m_file.read((char*)std::data(buffer), size_v);
		assert(m_file.good());
		const std::size_t result_v = m_file.gcount();
		assert(!(result_v % sector_size));
		return result_v / sizeof(*std::data(buffer));
	}

	std::ifstream m_file;
};

struct file_entry
{
	std::string name;
	std::uint32_t size;
	bool is_directory;
	std::uint32_t first_cluster;	
};

struct fat_root_directory_reader
{
	file_device& m_blkdev;
	
	using directory_entry = vfsio::vfat::directory_entry;
	
	std::uint32_t m_start;
	std::uint32_t m_count;
	std::uint32_t m_sector;
	std::uint32_t m_offset;
	std::uint32_t m_ep1sec;
	std::unique_ptr<directory_entry[]> m_bufptr;
	std::span<directory_entry> m_entries;
		
	fat_root_directory_reader(file_device& blkdev, auto const& small_v)
	: m_blkdev(blkdev) 		
	{
		m_start = small_v.BPB_RsvdSecCnt + small_v.BPB_FATSz16*small_v.BPB_NumFATs;
		m_count = (small_v.BPB_RootEntCnt * 32u + small_v.BPB_BytsPerSec - 1u) / small_v.BPB_BytsPerSec;
		m_sector = m_start;
		m_offset = 0u;
		m_ep1sec = (small_v.BPB_BytsPerSec + 31u) / 32u;
		m_bufptr = std::make_unique<directory_entry[]>(m_ep1sec);
		m_entries = std::span{ m_bufptr.get(), m_ep1sec };
		const auto result_v = m_blkdev.read(m_entries, m_sector);
		assert(result_v == m_entries.size());
	}

	auto read(file_entry& en) -> bool
	{
		while(true)
		{
			const auto sector_v = m_offset / m_ep1sec + m_start;
			const auto offset_v = m_offset % m_ep1sec;
			m_offset += 1u;
			if (sector_v != m_sector)
			{
				const auto result_v = m_blkdev.read(m_entries, sector_v);
				assert(result_v == m_entries.size());
				m_sector = sector_v;
			}
			const auto& entry_v = m_entries[offset_v];
			if (0xE5 == entry_v.DIR_Name[0])
				continue;
			if (0x00 == entry_v.DIR_Name[0])
				return false;
			
			std::string_view name (entry_v.DIR_Name, 8);
			while(name.back () == ' ')
				name.remove_suffix(1);
			std::string_view ext (&entry_v.DIR_Name[8], 3);
			while(ext.back () == ' ')
				ext.remove_suffix(1);
			en.name = name;
			if (!ext.empty ())
				en.name += ("."s + std::string(ext));
			en.size = entry_v.DIR_FileSize;
			en.first_cluster = entry_v.DIR_FstClusHI * 0x10000u + entry_v.DIR_FstClusLO;
			en.is_directory = !!(entry_v.DIR_Attr & 0x10u);
			return true;
		}
		
	}
};

struct fat_volume
{
	file_device& m_blkdev;

	union
	{
		std::byte bytes [512u];
		vfsio::vfat::small_bpb small;
		vfsio::vfat::large_bpb large;
	} m_bpb;

	std::uint32_t m_fat_size_sectors;
	std::uint32_t m_total_sectors;
	std::uint32_t m_root_dir_sectors;
	std::uint32_t m_data_sectors;

	fat_volume(auto&& blkdev): m_blkdev(blkdev)
	{
		using namespace vfsio::vfat;
		using namespace utils;
		
		const auto& small_v = m_bpb.small;
		const auto& large_v = m_bpb.large;
		
		assert(m_blkdev.read(m_bpb.bytes, 0u) == sizeof(m_bpb.bytes));
		assert((is_one_of<0x200u, 0x400u, 0x800u, 0x1000u>(small_v.BPB_BytsPerSec)));
		assert((is_one_of<1u, 2u, 4u, 8u, 16u, 32u, 64u, 128u>(small_v.BPB_SecPerClus)));
		assert(small_v.BPB_RsvdSecCnt >= 1u && small_v.BPB_RsvdSecCnt <= 32u);
		assert(small_v.BPB_NumFATs >= 1u && small_v.BPB_NumFATs <= 2u);
	
		m_fat_size_sectors = first_true_value(small_v.BPB_FATSz16, large_v.BPB_FATSz32);
		assert(m_fat_size_sectors >= 1u);		
		m_total_sectors = first_true_value(small_v.BPB_TotSec16, small_v.BPB_TotSec32);
		assert(m_total_sectors >= 1u);		
		m_root_dir_sectors = (small_v.BPB_RootEntCnt * 32u + small_v.BPB_BytsPerSec - 1u) / small_v.BPB_BytsPerSec;
		m_data_sectors = m_total_sectors - (small_v.BPB_RootEntCnt + small_v.BPB_NumFATs*m_fat_size_sectors + m_root_dir_sectors);		
		assert(m_data_sectors < m_total_sectors);		
	}	
	
	auto root() const -> fat_root_directory_reader
	{
		return { m_blkdev, m_bpb.small };
	}
	
};



int main(int,char** const) 
try
{
	std::filesystem::current_path(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\the_os\library\vfsio\workspace)");

	file_device fd("000_fat12.img");
	fat_volume fv(fd);

	auto root_v = fv.root();

	file_entry entry_v;
	while(root_v.read(entry_v))
	{
		Glog.info<"{:<12s} {:<12d} {:#08X} {:s}">(entry_v.name, entry_v.size, entry_v.first_cluster, entry_v.is_directory);
	}
	
  return 0;
}  
catch (std::exception const& ex)
{
	return 0;
}

