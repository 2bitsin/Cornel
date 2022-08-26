#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)

namespace PXE
{
  struct segoff
  {   
    uint16_t off;  
    uint16_t seg;
    
    template <typename T = void> 
    constexpr auto ptr() noexcept const -> T* 
    { return (T*)(seg*16ul + off); }
  };

  struct segdesc
  {
    uint16_t real_seg; 
    uint32_t phys_addr;
    uint16_t size;
  };

  struct pxenv
  {
    char      signature[6];    
    uint16_t  version;
    uint8_t   length;
    uint8_t   checksum;
    segoff    entry_point_16;
    uint32_t  entry_point_32;
    uint16_t  entry_point_32_seg;
    uint16_t  prot_mode_stack_seg;
    uint16_t  prot_mode_stack_size;
    uint16_t  base_code_seg;
    uint16_t  base_code_size;
    uint16_t  base_data_seg;
    uint16_t  base_data_size;
    uint16_t  undi_data_seg;
    uint16_t  undi_data_size;
    uint16_t  undi_code_seg;
    uint16_t  undi_code_size;
    segoff    pxe_bang_ptr;
  };

  struct pxebang
  {
    char      signature[4];
    uint8_t   length;
    uint8_t   checksum;
    uint8_t   rsv0;
    segoff    undi_rom_id;
    segoff    base_rom_id;
    segoff    entry_point_16;
    segoff    entry_point_32;
    segoff    status_callback_16;
    uint8_t   rsv1;
    uint8_t   count_seg_desc;
    uint16_t  first_seg_sel;
    segdesc   stack;
    segdesc   undi_data;
    segdesc   undi_code;
    segdesc   undi_code_write;
    segdesc   base_data;
    segdesc   base_code;
    segdesc   base_code_write;
  };
}

#pragma pack(pop)