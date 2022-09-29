#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

struct PXENVplus;
struct bangPXE;

namespace pxe_interface
{
  namespace packet_type
  {
    static inline constexpr const std::uint16_t dhcp_discover = 1;
    static inline constexpr const std::uint16_t dhcp_ack      = 2;
    static inline constexpr const std::uint16_t cached_reply  = 3;
  }

  enum class pxenv_status: std::uint16_t
  {
    success                           = 0x00u,
    failure                           = 0x01u,
    bad_func                          = 0x02u,
    unsupported                       = 0x03u,
    keep_undi                         = 0x04u,
    keep_all                          = 0x05u,
    out_of_resources                  = 0x06u,
    arp_timeout                       = 0x11u,
    udp_closed                        = 0x18u,
    udp_open                          = 0x19u,
    tftp_closed                       = 0x1Au,
    tftp_open                         = 0x1Bu,
    mcopy_problem                     = 0x20u,
    bis_integrity_failure             = 0x21u,
    bis_validate_failure              = 0x22u,
    bis_init_failure                  = 0x23u,
    bis_shutdown_failure              = 0x24u,
    bis_gboa_failure                  = 0x25u,
    bis_free_failure                  = 0x26u,
    bis_gsi_failure                   = 0x27u,
    bis_bad_cksum                     = 0x28u,    
    tftp_cannot_arp_address           = 0x30u,
    tftp_open_timeout                 = 0x32u,    
    tftp_unknown_opcode               = 0x33u,
    tftp_read_timeout                 = 0x35u,
    tftp_error_opcode                 = 0x36u,
    tftp_cannot_open_connection       = 0x38u,
    tftp_cannot_read_from_connection  = 0x39u,
    tftp_too_many_packages            = 0x3Au,
    tftp_file_not_found               = 0x3Bu,
    tftp_access_violation             = 0x3Cu,
    tftp_no_mcast_address             = 0x3Du,
    tftp_no_filesize                  = 0x3Eu,
    tftp_invalid_packet_size          = 0x3Fu,
    dhcp_timeout                      = 0x51u,
    dhcp_no_ip_address                = 0x52u,
    dhcp_no_bootfile_name             = 0x53u,
    dhcp_bad_ip_address               = 0x54u,
    undi_invalid_function             = 0x60u,
    undi_mediatest_failed             = 0x61u,
    undi_cannot_init_nic_for_mcast    = 0x62u,
    undi_cannot_initialize_nic        = 0x63u,
    undi_cannot_initialize_phy        = 0x64u,
    undi_cannot_read_config_data      = 0x65u,
    undi_cannot_read_init_data        = 0x66u,
    undi_bad_mac_address              = 0x67u,
    undi_bad_eeprom_checksum          = 0x68u,
    undi_error_setting_isr            = 0x69u,
    undi_invalid_state                = 0x6Au,
    undi_transmit_error               = 0x6Bu,
    undi_invalid_parameter            = 0x6Cu,    
    bstrap_prompt_menu                = 0x74u,
    bstrap_mcast_addr                 = 0x76u,
    bstrap_missing_list               = 0x77u,
    bstrap_no_response                = 0x78u,
    bstrap_file_too_big               = 0x79u,    
    binl_canceled_by_keystroke        = 0xA0u,
    binl_no_pxe_server                = 0xA1u,
    not_available_in_pmode            = 0xA2u,
    not_available_in_rmode            = 0xA3u,    
    busd_device_not_supported         = 0xB0u,    
    loader_no_free_base_memory        = 0xC0u,
    loader_no_bc_romid                = 0xC1u,
    loader_bad_bc_romid               = 0xC2u,
    loader_bad_bc_runtime_image       = 0xC3u,
    loader_no_undi_romid              = 0xC4u,
    loader_bad_undi_romid             = 0xC5u,
    loader_bad_undi_driver_image      = 0xC6u,
    loader_no_pxe_struct              = 0xC8u,
    loader_no_pxenv_struct            = 0xC9u,
    loader_undi_start                 = 0xCAu,
    loader_bc_start                   = 0xCBu,
    invalid_status                    = 0xFFFFu
  };

  static inline constexpr auto operator ! (const pxenv_status& status) noexcept -> bool
  {
    return pxenv_status::success != status;
  }

  struct tftp_params
  {
    std::uint32_t server_ip   { 0u    };
    std::uint16_t port        { 69u   };
    std::uint16_t packet_size { 512u  };
    std::uint32_t gateway_ip  { 0u    };
  };

  auto initialize       (bool first_time, ::PXENVplus&, ::bangPXE&) -> void;
  auto finalize         (bool last_time) -> void;

  auto get_cached_info  (std::uint16_t packet_type, std::span<const std::byte>& buffer, std::uint16_t& buffer_limit) -> pxenv_status;

  auto tftp_get_fsize   (std::string_view file_name, std::uint32_t& o_file_size, tftp_params const& options) -> pxenv_status;
  auto tftp_get_fsize   (std::string_view file_name, std::uint32_t& o_file_size) -> pxenv_status;

  auto tftp_open        (std::string_view file_name, tftp_params const& options) -> pxenv_status;
  auto tftp_open        (std::string_view file_name) -> pxenv_status;

  auto tftp_read        (std::span<std::byte>& buffer, std::uint16_t& o_packet_number) -> pxenv_status;
  auto tftp_close       () -> pxenv_status;


  auto download_file    (std::string_view file_name, std::span<std::byte>& buffer) -> pxenv_status; 
};
