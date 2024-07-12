#pragma once

#include <stdint.h>
#include <stddef.h>

#pragma pack(__push, 1)

enum PXE_status
{
  success = 0x00u,
  failure = 0x01u,
  bad_func = 0x02u,
  unsupported = 0x03u,
  keep_undi = 0x04u,
  keep_all = 0x05u,
  out_of_resources = 0x06u,
  arp_timeout = 0x11u,
  udp_closed = 0x18u,
  udp_open = 0x19u,
  tftp_closed = 0x1Au,
  tftp_open = 0x1Bu,
  mcopy_problem = 0x20u,
  bis_integrity_failure = 0x21u,
  bis_validate_failure = 0x22u,
  bis_init_failure = 0x23u,
  bis_shutdown_failure = 0x24u,
  bis_gboa_failure = 0x25u,
  bis_free_failure = 0x26u,
  bis_gsi_failure = 0x27u,
  bis_bad_cksum = 0x28u,
  tftp_cannot_arp_address = 0x30u,
  tftp_open_timeout = 0x32u,
  tftp_unknown_opcode = 0x33u,
  tftp_read_timeout = 0x35u,
  tftp_error_opcode = 0x36u,
  tftp_cannot_open_connection = 0x38u,
  tftp_cannot_read_from_connection = 0x39u,
  tftp_too_many_packages = 0x3Au,
  tftp_file_not_found = 0x3Bu,
  tftp_access_violation = 0x3Cu,
  tftp_no_mcast_address = 0x3Du,
  tftp_no_filesize = 0x3Eu,
  tftp_invalid_packet_size = 0x3Fu,
  dhcp_timeout = 0x51u,
  dhcp_no_ip_address = 0x52u,
  dhcp_no_bootfile_name = 0x53u,
  dhcp_bad_ip_address = 0x54u,
  undi_invalid_function = 0x60u,
  undi_mediatest_failed = 0x61u,
  undi_cannot_init_nic_for_mcast = 0x62u,
  undi_cannot_initialize_nic = 0x63u,
  undi_cannot_initialize_phy = 0x64u,
  undi_cannot_read_config_data = 0x65u,
  undi_cannot_read_init_data = 0x66u,
  undi_bad_mac_address = 0x67u,
  undi_bad_eeprom_checksum = 0x68u,
  undi_error_setting_isr = 0x69u,
  undi_invalid_state = 0x6Au,
  undi_transmit_error = 0x6Bu,
  undi_invalid_parameter = 0x6Cu,
  bstrap_prompt_menu = 0x74u,
  bstrap_mcast_addr = 0x76u,
  bstrap_missing_list = 0x77u,
  bstrap_no_response = 0x78u,
  bstrap_file_too_big = 0x79u,
  binl_canceled_by_keystroke = 0xA0u,
  binl_no_pxe_server = 0xA1u,
  not_available_in_pmode = 0xA2u,
  not_available_in_rmode = 0xA3u,
  busd_device_not_supported = 0xB0u,
  loader_no_free_base_memory = 0xC0u,
  loader_no_bc_romid = 0xC1u,
  loader_bad_bc_romid = 0xC2u,
  loader_bad_bc_runtime_image = 0xC3u,
  loader_no_undi_romid = 0xC4u,
  loader_bad_undi_romid = 0xC5u,
  loader_bad_undi_driver_image = 0xC6u,
  loader_no_pxe_struct = 0xC8u,
  loader_no_pxenv_struct = 0xC9u,
  loader_undi_start = 0xCAu,
  loader_bc_start = 0xCBu,

  pxe_not_available = 0xffffu,
  pxenv_bad_checksum = 0xfffeu,
  pxenv_bad_signature = 0xfffdu,
  version_not_supported = 0xfffcu,
  pxenv_bad_length = 0xfffbu,
  invalid_parameter = 0xfffau
};

enum PXENV_packet_type
{
  dhcp_discover = 1u,
  dhcp_ack = 2u,
  cached_reply = 3u
};

enum PXE_bootp_opcode
{
  bootp_req = 1u,
  bootp_rep = 2u
};

enum PXE_consts
{
  bootp_bcast = 0x00008000ul,
  vm_rfc1048 = 0x63825363ul
};

enum PXE_command
{
  PXENV_GET_CACHED_INFO = 0x0071u,
  TFTP_READ_FILE        = 0x0023u,
  TFTP_GET_FILE_SIZE    = 0x0025u
};

struct SEGOFFS16
{
  uint16_t seg;
  uint16_t off;
};

struct SEGDESC
{
  uint16_t segment_address;
  uint32_t physical_address;
  uint16_t segment_length;
};

typedef uint16_t(__cdecl far *EntryPointSP_type)(uint16_t, void far *);

struct PXEbang
{
  char Signature[4];
  uint8_t StructLength;
  uint8_t StructCksum;
  uint8_t StructRev;
  uint8_t _reserved_0;
  SEGOFFS16 UNDIROMID;
  SEGOFFS16 BaseROMID;
  EntryPointSP_type EntryPointSP;
  SEGOFFS16 EntryPointESP;
  SEGOFFS16 StatusCallout;
  uint8_t _reserved_1;
  uint8_t SegDescCnt;
  uint16_t FirstSelector;
  SEGDESC Stack;
  SEGDESC UNDIData;
  SEGDESC UNDICode;
  SEGDESC UNDICodeWrite;
  SEGDESC BC_Data;
  SEGDESC BC_Code;
  SEGDESC BC_CodeWrite;
};

struct PXENVplus
{
  char Signature[6];
  uint16_t Version;
  uint8_t Length;
  uint8_t Checksum;
  SEGOFFS16 RMEntry;
  uint32_t PMOffset;
  uint16_t PMSelector;
  uint16_t StackSeg;
  uint16_t StackSize;
  uint16_t BC_CodeSeg;
  uint16_t BC_CodeSize;
  uint16_t BC_DataSeg;
  uint16_t BC_DataSize;
  uint16_t UNDIDataSeg;
  uint16_t UNDIDataSize;
  uint16_t UNDICodeSeg;
  uint16_t UNDICodeSize;
  PXEbang far *PXEPtr;
};

typedef uint8_t MAC_ADDR[16];
typedef uint8_t IP4[4];

struct PXE_bootph_type
{
  uint8_t Opcode; // PXE_bootp_opcode
  uint8_t Hardware;
  uint8_t Hardlen;
  uint8_t Gatehops;

  uint32_t Ident;

  uint16_t Seconds;
  uint16_t Flags; // bootp_bcast ?

  IP4 Cip;
  IP4 Yip;
  IP4 Sip;
  IP4 Gip;

  MAC_ADDR CAddr;

  char Sname[64];
  char Bootfile[128];
  union
  {
    uint8_t d[1264u];
    struct
    {
      uint32_t Magic; // vm_rfc1048
      uint32_t Flags;
      uint8_t pad[56];
    };
  } Vendor;
};

struct PXENV_get_cached_info_type
{
  uint16_t Status;
  uint16_t PacketType;
  uint16_t BufferSize;
  void far *Buffer;
  uint16_t BufferLimit;
};

struct TFTP_get_file_size_type
{
  uint16_t Status;
  IP4 ServerIPAddress;
  IP4 GatewayIPAddress;
  char FileName[128];
  uint32_t FileSize;
};

struct TFTP_read_file_type
{
  uint16_t Status;
  char FileName[128];
  uint32_t BufferSize;
  uint32_t Buffer;
  IP4 ServerIPAddress;
  IP4 GatewayIPAddress;
  IP4 McastIPAddress;
  uint16_t TFTPClntPort;
  uint16_t TFTPSrvPort;
  uint16_t TFTPOpenTimeOut;
  uint16_t TFTPReopenDelay;
};

#pragma pack(__pop)

PXE_status PXE_init();
PXE_status PXE_call_api(uint16_t, void far *);
void PXE_print_dhcp(PXE_bootph_type const far *packet);
void PXE_print_info();

typedef PXE_bootph_type const far *PXE_bootph_type_pfar;
PXE_status PXE_get_cached_info(PXENV_packet_type type, uint16_t *length, PXE_bootph_type_pfar *packet);
PXE_status PXE_query_size(char const far *file_name, uint32_t *size);
PXE_status PXE_download(char const far *file_name, uint32_t target_address, uint32_t* target_size);
