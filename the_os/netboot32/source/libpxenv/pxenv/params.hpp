#pragma once

#include <cstddef>
#include <cstdint>

#include <hardware/x86address16.hpp>

#include <pxenv/core.hpp>

#pragma pack(push, 1)

using pxenv_status = ::pxenv::pxenv_status;

struct segoff32
{
  std::uint32_t off;
  std::uint16_t seg;
};

using segoff = x86arch::address16;

struct segdesc32
{
  uint16_t real_seg; 
  uint32_t base;
  uint16_t size;
};

struct segdesc16
{
  uint16_t real_seg;
  uint16_t size;
};

struct PXENVplus
{
  char      signature[6];    
  uint16_t  version;
  uint8_t   length;
  uint8_t   checksum;
  segoff    entry_point_16;
  uint32_t  entry_point_32;
  uint16_t  entry_point_32_seg;
  segdesc16 seg_desc[5];
  segoff    pxe_bang_ptr;
};

struct bangPXE
{
  char      signature[4];
  uint8_t   length;
  uint8_t   checksum;
  uint8_t   revision;
  uint8_t   reserved0;
  segoff    undi_rom_id;
  segoff    base_rom_id;
  segoff    entry_point_16;
  segoff    entry_point_32;
  segoff    status_callback_16;
  uint8_t   reserved1;
  uint8_t   count_seg_desc;
  uint16_t  first_seg_sel;
  // Can be any number of segdescs, but let's say no more then 7+7
  segdesc32 seg_desc[7+7];
};

static_assert(sizeof(bangPXE) == 0x58 + sizeof(segdesc32)*7);
static_assert(offsetof(bangPXE, signature         ) == 0x00);
static_assert(offsetof(bangPXE, length            ) == 0x04);
static_assert(offsetof(bangPXE, checksum          ) == 0x05);
static_assert(offsetof(bangPXE, revision          ) == 0x06);
static_assert(offsetof(bangPXE, reserved0         ) == 0x07);
static_assert(offsetof(bangPXE, undi_rom_id       ) == 0x08);
static_assert(offsetof(bangPXE, base_rom_id       ) == 0x0c);
static_assert(offsetof(bangPXE, entry_point_16    ) == 0x10);
static_assert(offsetof(bangPXE, entry_point_32    ) == 0x14);
static_assert(offsetof(bangPXE, status_callback_16) == 0x18);
static_assert(offsetof(bangPXE, reserved1         ) == 0x1c);
static_assert(offsetof(bangPXE, count_seg_desc    ) == 0x1d);
static_assert(offsetof(bangPXE, first_seg_sel     ) == 0x1e);
static_assert(offsetof(bangPXE, seg_desc[0]       ) == 0x20);
static_assert(offsetof(bangPXE, seg_desc[1]       ) == 0x28);
static_assert(offsetof(bangPXE, seg_desc[2]       ) == 0x30);
static_assert(offsetof(bangPXE, seg_desc[3]       ) == 0x38);
static_assert(offsetof(bangPXE, seg_desc[4]       ) == 0x40);
static_assert(offsetof(bangPXE, seg_desc[5]       ) == 0x48);
static_assert(offsetof(bangPXE, seg_desc[6]       ) == 0x50);

static_assert(sizeof(PXENVplus) == 0x2c);
static_assert(offsetof(PXENVplus, signature             ) == 0x00);
static_assert(offsetof(PXENVplus, version               ) == 0x06);
static_assert(offsetof(PXENVplus, length                ) == 0x08);
static_assert(offsetof(PXENVplus, checksum              ) == 0x09);
static_assert(offsetof(PXENVplus, entry_point_16        ) == 0x0a);
static_assert(offsetof(PXENVplus, entry_point_32        ) == 0x0e);
static_assert(offsetof(PXENVplus, entry_point_32_seg    ) == 0x12);
static_assert(offsetof(PXENVplus, seg_desc[0].real_seg  ) == 0x14);
static_assert(offsetof(PXENVplus, seg_desc[0].size      ) == 0x16);
static_assert(offsetof(PXENVplus, seg_desc[1].real_seg  ) == 0x18);
static_assert(offsetof(PXENVplus, seg_desc[1].size      ) == 0x1a);
static_assert(offsetof(PXENVplus, seg_desc[2].real_seg  ) == 0x1c);
static_assert(offsetof(PXENVplus, seg_desc[2].size      ) == 0x1e);
static_assert(offsetof(PXENVplus, seg_desc[3].real_seg  ) == 0x20);
static_assert(offsetof(PXENVplus, seg_desc[3].size      ) == 0x22);
static_assert(offsetof(PXENVplus, seg_desc[4].real_seg  ) == 0x24);
static_assert(offsetof(PXENVplus, seg_desc[4].size      ) == 0x26);
static_assert(offsetof(PXENVplus, pxe_bang_ptr          ) == 0x28);

struct pxenv_get_cached_info_type
{
  pxenv_status  status;
  std::uint16_t packet_type;
  std::uint16_t buffer_size;
  segoff        buffer;
  std::uint16_t buffer_limit;
};  

struct pxenv_tftp_open_type
{
  pxenv_status  status;
  std::uint32_t server_ip;
  std::uint32_t gateway_ip;
  std::uint8_t  file_name[128];
  std::uint16_t server_port;
  std::uint16_t packet_size;
} ;

struct pxenv_tftp_get_fsize_type
{
  pxenv_status  status;
  std::uint32_t server_ip;
  std::uint32_t gateway_ip;
  std::uint8_t  file_name[128];
  std::uint32_t file_size;
} ;

struct pxenv_tftp_read_type
{
  pxenv_status  status;
  std::uint16_t packet_number;
  std::uint16_t buffer_size;
  segoff        buffer;
};

struct pxenv_tftp_close_type
{
  pxenv_status  status;
};

#pragma pack(pop)


