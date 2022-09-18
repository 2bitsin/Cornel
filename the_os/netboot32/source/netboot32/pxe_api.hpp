#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)


struct pxe_api
{
  struct segoff
  {   
    uint16_t off;  
    uint16_t seg;
    
    template <typename T = void> 
    constexpr auto ptr() const noexcept -> T* 
    { return (T*)(seg*16ul + off); }
  };

  struct segdesc
  {
    uint16_t real_seg; 
    uint32_t base;
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

  struct bangPXE
  {
    char      signature[4];
    uint8_t   length;
    uint8_t   checksum;
    uint8_t   revision;
    uint8_t   reserved;
    segoff    undi_rom_id;
    segoff    base_rom_id;
    segoff    entry_point_16;
    segoff    entry_point_32;
    segoff    status_callback_16;
    uint8_t   rsv1;
    uint8_t   count_seg_desc;
    uint16_t  first_seg_sel;
    // Can be any number of segdescs
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"  
    segdesc   seg_desc[0];
 #pragma GCC diagnostic pop
  };

  static auto initialize(bool first_time, struct initialize_context const&) -> void;
  static auto finalize(bool last_time, struct initialize_context const&) -> void;
};

struct initialize_context
{
  struct pxe_api::PXENVplus& m_PXENVplus;
  struct pxe_api::bangPXE&   m_bangPXE;
};


#pragma pack(pop)