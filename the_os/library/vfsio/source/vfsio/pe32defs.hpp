#pragma once

#include <cstdint>
#include <cstddef>

namespace vfsio::pe32
{

#pragma pack(push, 1)

	static inline constexpr auto IMAGE_FILE_MACHINE_UNKNOWN                       = 0x0000  ; //  The content of this field is assumed 
	                                                                                          //  to be applicable to any machine type
	
	static inline constexpr auto IMAGE_FILE_MACHINE_ALPHA                         = 0x0184  ; //  Alpha AXP
	
	static inline constexpr auto IMAGE_FILE_MACHINE_AXP64			                    = 0x0284  ; //  Alpha 64 again ?
	
	static inline constexpr auto IMAGE_FILE_MACHINE_ALPHA64                       = 0x0284  ; //  Alpha 64
	
	static inline constexpr auto IMAGE_FILE_MACHINE_AM33                          = 0x01d3  ; //  Matsushita AM33
	
	static inline constexpr auto IMAGE_FILE_MACHINE_AMD64                         = 0x8664  ; //  x64
	
	static inline constexpr auto IMAGE_FILE_MACHINE_ARM                           = 0x01c0  ; //  ARM little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_ARM2                          = 0x01c4  ; //  ARM Thumb-2 little-endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_ARMNT                         = 0x01c4  ; //  ARM Thumb-2 little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_ARM64                         = 0xaa64  ; //  ARM64 little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_EBC                           = 0x0ebc  ; //  EFI byte code
	
	static inline constexpr auto IMAGE_FILE_MACHINE_I386                          = 0x014c  ; //  Intel 386 or later processors and 
	                                                                                          //  compatible processors
	
	static inline constexpr auto IMAGE_FILE_MACHINE_IA64                          = 0x0200  ; //  Intel Itanium processor family
	
	static inline constexpr auto IMAGE_FILE_MACHINE_M32R                          = 0x9041  ; //  Mitsubishi M32R little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_MIPS16                        = 0x0266  ; //  MIPS16
	
	static inline constexpr auto IMAGE_FILE_MACHINE_MIPSFPU                       = 0x0366  ; //  MIPS with FPU
	
	static inline constexpr auto IMAGE_FILE_MACHINE_MIPSFPU16                     = 0x0466  ; //  MIPS16 with FPU
	
	static inline constexpr auto IMAGE_FILE_MACHINE_POWERPC                       = 0x01f0  ; //  Power PC little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_POWERPCFP                     = 0x01f1  ; //  Power PC with floating point support
	
	static inline constexpr auto IMAGE_FILE_MACHINE_R3000                         = 0x0162  ; //  MIPS little-endian, 0x160 big-endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_R4000                         = 0x0166  ; //  MIPS little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_R10000                        = 0x0168  ; //  MIPS little endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_RISCV32                       = 0x5032  ; //  RISC-V 32-bit address space
	
	static inline constexpr auto IMAGE_FILE_MACHINE_RISCV64                       = 0x5064  ; //  RISC-V 64-bit address space
	
	static inline constexpr auto IMAGE_FILE_MACHINE_RISCV128                      = 0x5128  ; //  RISC-V 128-bit address space
	
	static inline constexpr auto IMAGE_FILE_MACHINE_SH3                           = 0x01a2  ; //  Hitachi SH3
	
	static inline constexpr auto IMAGE_FILE_MACHINE_SH3DSP                        = 0x01a3  ; //  Hitachi SH3 DSP
	
	static inline constexpr auto IMAGE_FILE_MACHINE_SH3E                          = 0x01a4  ; //  SH3E little-endian
	
	static inline constexpr auto IMAGE_FILE_MACHINE_SH4                           = 0x01a6  ; //  Hitachi SH4
	
	static inline constexpr auto IMAGE_FILE_MACHINE_SH5                           = 0x01a8  ; //  Hitachi SH5
	
	static inline constexpr auto IMAGE_FILE_MACHINE_THUMB                         = 0x01c2  ; //  Thumb
	
	static inline constexpr auto IMAGE_FILE_MACHINE_WCEMIPSV2                     = 0x0169  ; //  MIPS little-endian WCE v2
	
	
	static inline constexpr auto IMAGE_FILE_MACHINE_TRICORE												= 0x0520  ; //  Infineon
	
	static inline constexpr auto IMAGE_FILE_MACHINE_CEF														= 0x0CEF  ; //  CEF
	                                        
	static inline constexpr auto IMAGE_FILE_MACHINE_CEE														= 0x0CEE  ; //  CEE
	
	
	 /**********************************************************************
	  *  
	  */
	
	static inline constexpr auto IMAGE_FILE_RELOCS_STRIPPED                       = 0x0001  ; //  Image only, Windows CE, and Microsoft Windows NT and later. 
	                                                                                          //  This indicates that the file does not contain base relocations 
	                                                                                          //  and must therefore be loaded at its preferred base address. 
	                                                                                          //  If the base address is not available, the loader reports an 
	                                                                                          //  error. The default behavior of the linker is to strip base 
	                                                                                          //  relocations from executable (EXE) files.
	
	static inline constexpr auto IMAGE_FILE_EXECUTABLE_IMAGE                      = 0x0002  ; //  Image only. This indicates that the image file is valid and 
	                                                                                          //  can be run. If this flag is not set, it indicates a linker error.
	
	static inline constexpr auto IMAGE_FILE_LINE_NUMS_STRIPPED                    = 0x0004  ; //  COFF line numbers have been removed. This flag is deprecated 
	                                                                                          //  and should be zero.
	
	static inline constexpr auto IMAGE_FILE_LOCAL_SYMS_STRIPPED                   = 0x0008  ; //  COFF symbol table entries for local symbols have been removed. 
	                                                                                          //  This flag is deprecated and should be zero.
	
	static inline constexpr auto IMAGE_FILE_AGGRESSIVE_WS_TRIM                    = 0x0010  ; //  Obsolete. Aggressively trim working set. This flag is deprecated 
	                                                                                          //  for Windows 2000 and later and must be zero.
	
	static inline constexpr auto IMAGE_FILE_LARGE_ADDRESS_AWARE                   = 0x0020  ; //  Application can handle > 2-GB addresses.
	
	static inline constexpr auto IMAGE_FILE_16BIT_MACHINE                         = 0x0040  ; //  This flag is reserved for future use.
	
	static inline constexpr auto IMAGE_FILE_BYTES_REVERSED_LO                     = 0x0080  ; //  Little endian: the least significant bit (LSB) precedes the most 
	                                                                                          //  significant bit (MSB) in memory. This flag is deprecated and should
	                                                                                          //  be zero.
	
	static inline constexpr auto IMAGE_FILE_32BIT_MACHINE                         = 0x0100  ; //  Machine is based on a 32-bit-uint16_t architecture.
	
	static inline constexpr auto IMAGE_FILE_DEBUG_STRIPPED                        = 0x0200  ; //  Debugging information is removed from the image file.
	
	static inline constexpr auto IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP               = 0x0400  ; //  If the image is on removable media, fully load it and 
	                                                                                          //  copy it to the swap file.
	
	static inline constexpr auto IMAGE_FILE_NET_RUN_FROM_SWAP                     = 0x0800  ; //  If the image is on network media, fully load it and copy 
	                                                                                          //  it to the swap file.
	
	static inline constexpr auto IMAGE_FILE_SYSTEM                                = 0x1000  ; //  The image file is a system file, not a user program.
	
	static inline constexpr auto IMAGE_FILE_DLL                                   = 0x2000  ; //  The image file is a dynamic-link library (DLL). 
	                                                                                          //  Such files are considered executable files for almost 
	                                                                                          //  all purposes, although they cannot be directly run.
	
	static inline constexpr auto IMAGE_FILE_UP_SYSTEM_ONLY                        = 0x4000  ; //  The file should be run only on a uniprocessor machine.
	
	static inline constexpr auto IMAGE_FILE_BYTES_REVERSED_HI                     = 0x8000  ; //  Big endian: the MSB precedes the LSB in memory. 
	                                                                                          //  This flag is deprecated and should be zero.
	 /**********************************************************************
	  *  
	  */
	
	static inline constexpr auto IMAGE_NT_OPTIONAL_HDR32_MAGIC                    = 0x010b  ; //  The file is an executable image.
	
	static inline constexpr auto IMAGE_NT_OPTIONAL_HDR64_MAGIC                    = 0x020b  ; //  The file is an executable image.
	
	static inline constexpr auto IMAGE_ROM_OPTIONAL_HDR_MAGIC                     = 0x0107  ; //  The file is a ROM image.
	                                                                                          //  This flag is deprecated and should be zero.
	 /**********************************************************************
	  *  
	  */
	
	static inline constexpr auto IMAGE_SUBSYSTEM_UNKNOWN                          = 0x0000  ; //  An unknown subsystem4
	static inline constexpr auto IMAGE_SUBSYSTEM_NATIVE                           = 0x0001  ; //  Device drivers and native Windows processes
	static inline constexpr auto IMAGE_SUBSYSTEM_WINDOWS_GUI                      = 0x0002  ; //  The Windows graphical user interface (GUI) subsystem
	static inline constexpr auto IMAGE_SUBSYSTEM_WINDOWS_CUI                      = 0x0003  ; //  The Windows character subsystem
	                                                                      //      = 0x0004  ; //  ???
	static inline constexpr auto IMAGE_SUBSYSTEM_OS2_CUI                          = 0x0005  ; //  The OS/2 character subsystem
	                                                                      //      = 0x0006  ; //  ???
	static inline constexpr auto IMAGE_SUBSYSTEM_POSIX_CUI                        = 0x0007  ; //  The Posix character subsystem
	static inline constexpr auto IMAGE_SUBSYSTEM_NATIVE_WINDOWS                   = 0x0008  ; //  Native Win9x driver
	static inline constexpr auto IMAGE_SUBSYSTEM_WINDOWS_CE_GUI                   = 0x0009  ; //  Windows CE
	static inline constexpr auto IMAGE_SUBSYSTEM_EFI_APPLICATION                  = 0x000A  ; //  An Extensible Firmware Interface (EFI) application
	static inline constexpr auto IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER          = 0x000B  ; //  An EFI driver with boot services
	static inline constexpr auto IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER               = 0x000C  ; //  An EFI driver with run-time services
	static inline constexpr auto IMAGE_SUBSYSTEM_EFI_ROM                          = 0x000D  ; //  An EFI ROM image
	static inline constexpr auto IMAGE_SUBSYSTEM_XBOX                             = 0x000E  ; //  XBOX
	                                                                      //      = 0x000F  ; //  ???
	static inline constexpr auto IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION         = 0x0010  ; //  Windows boot application.
	
	 /**********************************************************************
	  *  
	  */
	
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_RESERVED0               = 0x0001  ; //  Reserved, must be zero.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_RESERVED1               = 0x0002  ; //  Reserved, must be zero.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_RESERVED2               = 0x0004  ; //  Reserved, must be zero.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_RESERVED3               = 0x0008  ; //  Reserved, must be zero.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_RESERVED4               = 0x0010  ; //  Reserved ?
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA         = 0x0020  ; //  Image can handle a high entropy 64-bit virtual address space.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE            = 0x0040  ; //  DLL can be relocated at load time.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY         = 0x0080  ; //  Code Integrity checks are enforced.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_NX_COMPAT               = 0x0100  ; //  Image is NX compatible.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_NO_ISOLATION            = 0x0200  ; //  Isolation aware, but do not isolate the image.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_NO_SEH                  = 0x0400  ; //  Does not use structured exception (SE) handling. No SE handler may be called in this image.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_NO_BIND                 = 0x0800  ; //  Do not bind the image.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_APPCONTAINER            = 0x1000  ; //  Image must execute in an AppContainer.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_WDM_DRIVER              = 0x2000  ; //  A WDM driver.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_GUARD_CF                = 0x4000  ; //  Image supports Control Flow Guard.
	static inline constexpr auto IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE   = 0x8000  ; //  Terminal Server aware.
	
	 /**********************************************************************
	  *  
	  */
	
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_EXPORT                     = 0x0000 ; // Export directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_IMPORT                     = 0x0001 ; // Import directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_RESOURCE                   = 0x0002 ; // Resource directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_EXCEPTION                  = 0x0003 ; // Exception directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_SECURITY                   = 0x0004 ; // Security directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_BASERELOC                  = 0x0005 ; // Base relocation table
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_DEBUG                      = 0x0006 ; // Debug directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_ARCHITECTURE               = 0x0007 ; // Architecture-specific data
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_GLOBALPTR                  = 0x0008 ; // The relative virtual address of global pointer
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_TLS                        = 0x0009 ; // Thread local storage directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG                = 0x000A ; // Load configuration directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT               = 0x000B ; // Bound import directory
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_IAT                        = 0x000C ; // Import address table
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT               = 0x000D ; // Delay import table
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR             = 0x000E ; // COM descriptor table
	static inline constexpr auto IMAGE_DIRECTORY_ENTRY_RESERVED                   = 0x000F ; // Reserved, must be zero
	
	static inline constexpr auto IMAGE_NUMBEROF_DIRECTORY_ENTRIES                 = 0x0010u ; // Number of data directory entries
	
	 /**********************************************************************
	  *  
	  */
	
	static inline constexpr auto IMAGE_SCN_RESERVED0                              = 0x00000001 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_RESERVED1                              = 0x00000002 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_RESERVED2                              = 0x00000004 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_TYPE_NO_PAD                            = 0x00000008 ; // The section should not be padded to the next boundary. This flag is obsolete and is replaced by IMAGE_SCN_ALIGN_1BYTES. This is valid only for object files.
	static inline constexpr auto IMAGE_SCN_RESERVED3                              = 0x00000010 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_CNT_CODE                               = 0x00000020 ; // The section contains executable code.
	static inline constexpr auto IMAGE_SCN_CNT_INITIALIZED_DATA                   = 0x00000040 ; // The section contains initialized data.
	static inline constexpr auto IMAGE_SCN_CNT_UNINITIALIZED_DATA                 = 0x00000080 ; // The section contains uninitialized data.
	static inline constexpr auto IMAGE_SCN_LNK_OTHER                              = 0x00000100 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_LNK_INFO                               = 0x00000200 ; // The section contains comments or other information. The .drectve section has this type. This is valid for object files only.
	static inline constexpr auto IMAGE_SCN_RESERVED4                              = 0x00000400 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_LNK_REMOVE                             = 0x00000800 ; // The section will not become part of the image. This is valid only for object files.
	static inline constexpr auto IMAGE_SCN_LNK_COMDAT                             = 0x00001000 ; // The section contains COMDAT data. For more information, see COMDAT Sections (Object Only). This is valid only for object files.
	static inline constexpr auto IMAGE_SCN_RESERVED5                              = 0x00002000 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_NO_DEFER_SPEC_EXC                      = 0x00004000 ; // Reset speculative exceptions handling bits in the TLB entries for this section.
	static inline constexpr auto IMAGE_SCN_GPREL                                  = 0x00008000 ; // The section contains data referenced through the global pointer (GP).
	static inline constexpr auto IMAGE_SCN_RESERVED6                              = 0x00010000 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_MEM_PURGEABLE                          = 0x00020000 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_MEM_LOCKED                             = 0x00040000 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_MEM_PRELOAD                            = 0x00080000 ; // Reserved for future use.
	static inline constexpr auto IMAGE_SCN_ALIGN_1BYTES                           = 0x00100000 ; // Align data on a 1-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_2BYTES                           = 0x00200000 ; // Align data on a 2-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_4BYTES                           = 0x00300000 ; // Align data on a 4-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_8BYTES                           = 0x00400000 ; // Align data on an 8-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_16BYTES                          = 0x00500000 ; // Align data on a 16-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_32BYTES                          = 0x00600000 ; // Align data on a 32-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_64BYTES                          = 0x00700000 ; // Align data on a 64-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_128BYTES                         = 0x00800000 ; // Align data on a 128-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_256BYTES                         = 0x00900000 ; // Align data on a 256-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_512BYTES                         = 0x00A00000 ; // Align data on a 512-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_1024BYTES                        = 0x00B00000 ; // Align data on a 1024-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_2048BYTES                        = 0x00C00000 ; // Align data on a 2048-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_4096BYTES                        = 0x00D00000 ; // Align data on a 4096-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_ALIGN_8192BYTES                        = 0x00E00000 ; // Align data on an 8192-byte boundary. Valid only for object files.
	static inline constexpr auto IMAGE_SCN_LNK_NRELOC_OVFL                        = 0x01000000 ; // The section contains extended relocations.
	static inline constexpr auto IMAGE_SCN_MEM_DISCARDABLE                        = 0x02000000 ; // The section can be discarded as needed.
	static inline constexpr auto IMAGE_SCN_MEM_NOT_CACHED                         = 0x04000000 ; // The section cannot be cached.
	static inline constexpr auto IMAGE_SCN_MEM_NOT_PAGED                          = 0x08000000 ; // The section is not pageable.
	static inline constexpr auto IMAGE_SCN_MEM_SHARED                             = 0x10000000 ; // The section can be shared in memory.
	static inline constexpr auto IMAGE_SCN_MEM_EXECUTE                            = 0x20000000 ; // The section can be executed as code.
	static inline constexpr auto IMAGE_SCN_MEM_READ                               = 0x40000000 ; // The section can be read.
	static inline constexpr auto IMAGE_SCN_MEM_WRITE                              = 0x80000000 ; // The section can be written to.
	
	 /**********************************************************************
	  *  
	  */
	
	
	struct PE_dos_mz_header
	{
	  uint16_t e_magic;                         // 0    Signature
	
	  uint16_t e_cblp;                          // 2    Number of bytes in the last page.
	
	  uint16_t e_cp;                            // 4    Number of whole/partial pages.
	
	  uint16_t e_crlc;                          // 6    Number of entries in the relocation table.
	
	  uint16_t e_cparhdr;                       // 8    The number of paragraphs taken up by the header. 
	                                            //      It can be any value, as the loader just uses it 
	                                            //      to find where the actual executable data starts. 
	                                            //      It may be larger than what the "standard" fields 
	                                            //      take up, and you may use it if you want to include 
	                                            //      your own header metadata, or put the relocation 
	                                            //      table there, or use it for any other purpose.
	
	  uint16_t e_minalloc;                      // 10   The number of paragraphs required by the program,
	                                            //      excluding the PSP and program image. If no free 
	                                            //      block is big enough, the loading stops.
	
	  uint16_t e_maxalloc;                      // 12   The number of paragraphs requested by the program. 
	                                            //      If no free block is big enough, the biggest one 
	                                            //      possible is allocated.
	
	  uint16_t e_ss;                            // 14   Relocatable segment address for SS.
	
	  uint16_t e_sp;                            // 16   Initial value for SP.
	
	  uint16_t e_csum;                          // 18   When added to the sum of all other uint16_ts in the 
	                                            //      file, the result should be zero.
	
	  uint16_t e_ip;                            // 20   Initial value for IP.
	
	  uint16_t e_cs;                            // 22   Relocatable segment address for CS.
	
	  uint16_t e_lfarlc;                        // 24   The (absolute) offset to the relocation table.
	
	  uint16_t e_ovno;                          // 26   Value used for overlay management. If zero, 
	                                            //      this is the main executable.
	
	  uint64_t e_res;                           // 28   Reserved
	
	  uint16_t e_oemid;                         // 36   Defined by name but no other information is given; typically zeroes
	
	  uint16_t e_oeminfo;                       // 38   Defined by name but no other information is given; typically zeroes
	
	  uint8_t  e_res2[20];                      // 40   Reserved
	
	  uint32_t e_lfanew;                        // 60   Starting address of the PE header
	};
	
	static_assert(sizeof(PE_dos_mz_header) == 64);
	
	struct PE_coff_standard
	{
	  uint32_t signature;                       // 0    Signature "PE\0\0"
	
	  uint16_t machine;                         // 4    The number that identifies the type of target machine. 
	                                            //      For more information, see Machine Types.
	
	  uint16_t number_of_sections;              // 6    The number of sections. This indicates the size of the 
	                                            //      section table, which immediately follows the headers.
	
	  uint32_t time_date_stamp;                 // 8    The low 32 bits of the number of seconds since 00:00 
	                                            //      January 1, 1970 (a C run-time time_t value), which 
	                                            //      indicates when the file was created.
	
	  uint32_t pointer_to_symbol_table;         // 12   The file offset of the COFF symbol table, or zero if 
	                                            //      no COFF symbol table is present. This value should be 
	                                            //      zero for an image because COFF debugging information 
	                                            //      is deprecated.
	
	  uint32_t number_of_symbols;               // 16   The number of entries in the symbol table. This data 
	                                            //      can be used to locate the string table, which immediately 
	                                            //      follows the symbol table. This value should be zero for an 
	                                            //      image because COFF debugging information is deprecated.
	
	  uint16_t size_of_optional_header;         // 20   The size of the optional header, which is required for 
	                                            //      executable files but not for object files. This value 
	                                            //      should be zero for an object file. For a description 
	                                            //      of the header format, see Optional Header (Image Only).
	
	  uint16_t characteristics;                 // 22   The flags that indicate the attributes of the file. 
	                                            //      For specific flag values, see Characteristics.
	
	};
	
	static_assert(sizeof(PE_coff_standard) == 24);
	
	struct PE_coff_optional_32
	{
	  uint16_t magic;                           // 0    The unsigned integer that identifies the state of the image file. 
	                                            //      The most common number is 0x10B, which identifies it as a normal 
	                                            //      executable file. 0x107 identifies it as a ROM image, and 0x20B 
	                                            //      identifies it as a PE32+ executable.
	
	  uint8_t  major_linker_version;            // 2    The linker major version number.
	
	  uint8_t  minor_linker_version;            // 3    The linker major version number.
	
	  uint32_t size_of_code;                    // 4    The size of the code (text) section, or the sum of all code sections 
	                                            //      if there are multiple sections.
	
	  uint32_t size_of_initialized_data;        // 8    The size of the initialized data section, or the sum of all such 
	                                            //      sections if there are multiple data sections.
	    
	  uint32_t size_of_uninitialized_data;      // 12   The size of the uninitialized data section (BSS), or the sum of all
	                                            //      such sections if there are multiple BSS sections.
	
	  uint32_t address_of_entry_point;          // 16   The address of the entry point relative to the image base when the 
	                                            //      executable file is loaded into memory. For program images, this is 
	                                            //      the starting address. For device drivers, this is the address of the 
	                                            //      initialization function. An entry point is optional for DLLs. When 
	                                            //      no entry point is present, this field must be zero.
	
	  uint32_t base_of_code;                    // 20   The address that is relative to the image base of the beginning-of-code 
	                                            //      section when it is loaded into memory.
	
	  uint32_t base_of_data;                    // 24   The address that is relative to the image base of the beginning-of-data 
	                                            //      section when it is loaded into memory.
	
	  uint32_t image_base;                      // 28   The preferred address of the first byte of image when loaded into 
	                                            //      memory; must be a multiple of 64 K. The default for DLLs is 0x10000000. 
	                                            //      The default for Windows CE EXEs is 0x00010000. The default for 
	                                            //      Windows NT, Windows 2000, Windows XP, Windows 95, Windows 98, and 
	                                            //      Windows Me is 0x00400000.
	
	  uint32_t section_alignment;               // 32   The alignment (in bytes) of sections when they are loaded into memory. 
	                                            //      It must be greater than or equal to FileAlignment. The default is the 
	                                            //      page size for the architecture.
	
	  uint32_t file_alignment;                  // 36   The alignment factor (in bytes) that is used to align the raw data of 
	                                            //      sections in the image file. The value should be a power of 2 between 
	                                            //      512 and 64 K, inclusive. The default is 512. If the SectionAlignment 
	                                            //      is less than the architecture's page size, then FileAlignment must 
	                                            //      match SectionAlignment.
	
	  uint16_t major_operating_system_version;  // 40   The major version number of the required operating system.
	
	  uint16_t minor_operating_system_version;  // 42   The minor version number of the required operating system.
	
	  uint16_t major_image_version;             // 44   The major version number of the image.
	
	  uint16_t minor_image_version;             // 46   The minor version number of the image.
	
	  uint16_t major_subsystem_version;         // 48   The major version number of the subsystem.
	
	  uint16_t minor_subsystem_version;         // 50   The minor version number of the subsystem.
	
	  uint32_t win32_version_value;             // 52   Reserved, must be zero.
	
	  uint32_t size_of_image;                   // 56   The size (in bytes) of the image, including all headers, 
	                                            //      as the image is loaded in memory. It must be a multiple 
	                                            //      of SectionAlignment.
	
	  uint32_t size_of_headers;                 // 60   The combined size of an MS-DOS stub, PE header, and section 
	                                            //      headers rounded up to a multiple of FileAlignment.
	
	  uint32_t check_sum;                       // 64   The image file checksum. The algorithm for computing the 
	                                            //      checksum is incorporated into IMAGHELP.DLL. The following are 
	                                            //      checked for validation at load time: all drivers, any DLL 
	                                            //      loaded at boot time, and any DLL that is loaded into a 
	                                            //      critical Windows process.
	
	  uint16_t subsystem;                       // 68   The subsystem that is required to run this image. For more 
	                                            //      information, see Windows Subsystem.
	
	  uint16_t dll_characteristics;             // 70   For more information, see DLL Characteristics later in this 
	                                            //      specification.
	
	  uint32_t size_of_stack_reserve;           // 72   The size of the stack to reserve. Only SizeOfStackCommit is 
	                                            //      committed; the rest is made available one page at a time 
	                                            //      until the reserve size is reached.
	
	  uint32_t size_of_stack_commit;            // 76   The size of the stack to commit.
	
	  uint32_t size_of_heap_reserve;            // 80   The size of the local heap space to reserve. Only 
	                                            //      SizeOfHeapCommit is committed; the rest is made available 
	                                            //      one page at a time until the reserve size is reached.
	
	  uint32_t size_of_heap_commit;             // 84   The size of the local heap space to commit.
	
	  uint32_t loader_flags;                    // 88   Reserved, must be zero.
	
	  uint32_t number_of_rva_and_sizes;         // 92   The number of data-directory entries in the remainder of 
	                                            //      the optional header. Each describes a location and size.
	};
	
	static_assert(sizeof(PE_coff_optional_32) == 96);
	
	struct PE_coff_optional_64
	{
	  uint16_t magic;                           // 0    The unsigned integer that identifies the state of the image file. 
	                                            //      The most common number is 0x10B, which identifies it as a normal 
	                                            //      executable file. 0x107 identifies it as a ROM image, and 0x20B 
	                                            //      identifies it as a PE32+ executable.
	
	  uint8_t  major_linker_version;            // 2    The linker major version number.
	
	  uint8_t  minor_linker_version;            // 3    The linker major version number.
	
	  uint32_t size_of_code;                    // 4    The size of the code (text) section, or the sum of all code sections 
	                                            //      if there are multiple sections.
	
	  uint32_t size_of_initialized_data;        // 8    The size of the initialized data section, or the sum of all such 
	                                            //      sections if there are multiple data sections.
	    
	  uint32_t size_of_uninitialized_data;      // 12   The size of the uninitialized data section (BSS), or the sum of all
	                                            //      such sections if there are multiple BSS sections.
	
	  uint32_t address_of_entry_point;          // 16   The address of the entry point relative to the image base when the 
	                                            //      executable file is loaded into memory. For program images, this is 
	                                            //      the starting address. For device drivers, this is the address of the 
	                                            //      initialization function. An entry point is optional for DLLs. When 
	                                            //      no entry point is present, this field must be zero.
	
	  uint32_t base_of_code;                    // 20   The address that is relative to the image base of the beginning-of-code 
	                                            //      section when it is loaded into memory.
	
	  uint64_t image_base;                      // 24   The preferred address of the first byte of image when loaded into 
	                                            //      memory; must be a multiple of 64 K. The default for DLLs is 0x10000000. 
	                                            //      The default for Windows CE EXEs is 0x00010000. The default for 
	                                            //      Windows NT, Windows 2000, Windows XP, Windows 95, Windows 98, and 
	                                            //      Windows Me is 0x00400000.
	
	  uint32_t section_alignment;               // 32   The alignment (in bytes) of sections when they are loaded into memory. 
	                                            //      It must be greater than or equal to FileAlignment. The default is the 
	                                            //      page size for the architecture.
	
	  uint32_t file_alignment;                  // 36   The alignment factor (in bytes) that is used to align the raw data of 
	                                            //      sections in the image file. The value should be a power of 2 between 
	                                            //      512 and 64 K, inclusive. The default is 512. If the SectionAlignment 
	                                            //      is less than the architecture's page size, then FileAlignment must 
	                                            //      match SectionAlignment.
	
	  uint16_t major_operating_system_version;  // 40   The major version number of the required operating system.
	
	  uint16_t minor_operating_system_version;  // 42   The minor version number of the required operating system.
	
	  uint16_t major_image_version;             // 44   The major version number of the image.
	
	  uint16_t minor_image_version;             // 46   The minor version number of the image.
	
	  uint16_t major_subsystem_version;         // 48   The major version number of the subsystem.
	
	  uint16_t minor_subsystem_version;         // 50   The minor version number of the subsystem.
	
	  uint32_t win32_version_value;             // 52   Reserved, must be zero.
	
	  uint32_t size_of_image;                   // 56   The size (in bytes) of the image, including all headers, 
	                                            //      as the image is loaded in memory. It must be a multiple 
	                                            //      of SectionAlignment.
	
	  uint32_t size_of_headers;                 // 60   The combined size of an MS-DOS stub, PE header, and section 
	                                            //      headers rounded up to a multiple of FileAlignment.
	
	  uint32_t check_sum;                       // 64   The image file checksum. The algorithm for computing the 
	                                            //      checksum is incorporated into IMAGHELP.DLL. The following are 
	                                            //      checked for validation at load time: all drivers, any DLL 
	                                            //      loaded at boot time, and any DLL that is loaded into a 
	                                            //      critical Windows process.
	
	  uint16_t subsystem;                       // 68   The subsystem that is required to run this image. For more 
	                                            //      information, see Windows Subsystem.
	
	  uint16_t dll_characteristics;             // 70   For more information, see DLL Characteristics later in this 
	                                            //      specification.
	
	  uint64_t size_of_stack_reserve;           // 72   The size of the stack to reserve. Only SizeOfStackCommit is 
	                                            //      committed; the rest is made available one page at a time 
	                                            //      until the reserve size is reached.
	
	  uint64_t size_of_stack_commit;            // 80   The size of the stack to commit.
	
	  uint64_t size_of_heap_reserve;            // 88   The size of the local heap space to reserve. Only 
	                                            //      SizeOfHeapCommit is committed; the rest is made available 
	                                            //      one page at a time until the reserve size is reached.
	
	  uint64_t size_of_heap_commit;             // 96   The size of the local heap space to commit.
	
	  uint32_t loader_flags;                    // 104  Reserved, must be zero.
	
	  uint32_t number_of_rva_and_sizes;         // 108  The number of data-directory entries in the remainder of 
	                                            //      the optional header. Each describes a location and size.
	};
	
	static_assert(sizeof(PE_coff_optional_64) == 112);
	
	struct PE_data_directory
	{
	  uint32_t virtual_address;                 // 0    Relative virtual address
	  uint32_t size;                            // 8    Entity size
	};
	
	static_assert(sizeof(PE_data_directory) == 8);
	
	struct PE_section_header
	{
	  char      name [8];                       // 0    An 8-byte, null-padded UTF-8 encoded string. If the string 
	                                            //      is exactly 8 characters long, there is no terminating null. 
	                                            //      For longer names, this field contains a slash (/) that is 
	                                            //      followed by an ASCII representation of a decimal number that 
	                                            //      is an offset into the string table. Executable images do not 
	                                            //      use a string table and do not support section names longer 
	                                            //      than 8 characters. Long names in object files are truncated 
	                                            //      if they are emitted to an executable file.
	
	  uint32_t  virtual_size;                   // 8    The total size of the section when loaded into memory. If 
	                                            //      this value is greater than SizeOfRawData, the section is 
	                                            //      zero-padded. This field is valid only for executable images 
	                                            //      and should be set to zero for object files.
	
	  uint32_t  virtual_address;                // 12   For executable images, the address of the first byte of the 
	                                            //      section relative to the image base when the section is loaded 
	                                            //      into memory. For object files, this field is the address of 
	                                            //      the first byte before relocation is applied; for simplicity, 
	                                            //      compilers should set this to zero. Otherwise, it is an arbitrary 
	                                            //      value that is subtracted from offsets during relocation
	
	  uint32_t  size_of_raw_data;               // 16   The size of the section (for object files) or the size of the 
	                                            //      initialized data on disk (for image files). For executable images, 
	                                            //      this must be a multiple of FileAlignment from the optional header. 
	                                            //      If this is less than VirtualSize, the remainder of the section is 
	                                            //      zero-filled. Because the SizeOfRawData field is rounded but the 
	                                            //      VirtualSize field is not, it is possible for SizeOfRawData to be 
	                                            //      greater than VirtualSize as well. When a section contains only 
	                                            //      uninitialized data, this field should be zero.
	
	  uint32_t  pointer_to_raw_data;            // 20   The file pointer to the first page of the section within the COFF 
	                                            //      file. For executable images, this must be a multiple of FileAlignment 
	                                            //      from the optional header. For object files, the value should be aligned 
	                                            //      on a 4-byte boundary for best performance. When a section contains 
	                                            //      only uninitialized data, this field should be zero.
	
	  uint32_t  pointer_to_relocations;         // 24   The file pointer to the beginning of relocation entries for the section. 
	                                            //      This is set to zero for executable images or if there are no relocations.
	
	  uint32_t  pointer_to_line_numbers;        // 28   The file pointer to the beginning of line-number entries for the section.
	                                            //      This is set to zero if there are no COFF line numbers. This value should 
	                                            //      be zero for an image because COFF debugging information is deprecated.
	
	  uint16_t  number_of_relocations;          // 32   The number of relocation entries for the section. This is set to zero for  
	                                            //      executable images.                                            
	
	  uint16_t  number_of_line_numbers;         // 34   The number of line-number entries for the section. This value should be 
	                                            //      zero for an image because COFF debugging information is deprecated.                                            
	
	  uint32_t  characteristics;                // 36   The flags that describe the characteristics of the section. For more 
	                                            //      information, see Section Flags.
	
	};
	
	static_assert(sizeof(PE_section_header) == 40);
	
	struct PE_import_directory_table
	{
	  uint32_t	import_lookup_table_rva;				// 0		The RVA of the import lookup table. This table contains a name or ordinal 
	                                            //			for each import. (The name "Characteristics" is used in Winnt.h, but no 
	                                            //			longer describes this field.)
	
	  uint32_t	time_data_stamp;								// 4		The stamp that is set to zero until the image is bound. After the image is 
	                                            //			bound, this field is set to the time/data stamp of the DLL.
	
	  uint32_t	forwarder_chain;								// 8		The index of the first forwarder reference.
	
	  uint32_t	name_rva;												// 12		The address of an ASCII string that contains the name of the DLL. This address 
	                                            //			is relative to the image base.
	
	  uint32_t	import_address_table_rva;				// 16		The RVA of the import address table. The contents of this table are identical 
	                                            //			to the contents of the import lookup table until the image is bound.
	
	};
	
	static_assert(sizeof(PE_import_directory_table) == 20);
	
	struct PE_delay_import_directory_table
	{
	  uint32_t	attributes;											//	0		Must be zero
	
	  uint32_t	name_rva;												//	4		The RVA of the name of the DLL to be loaded. The name resides in the read-only 
	                                            //			data section of the image.
	
	  uint32_t	module_handle;									//	8		The RVA of the module handle (in the data section of the image) of the DLL to be 
	                                            //			delay-loaded. It is used for storage by the routine that is supplied to manage 
	                                            //			delay-loading.
	
	  uint32_t	import_address_table_rva;				//	12	The RVA of the delay-load import address table. For more information, see Delay 
	                                            //			Import Address Table (IAT).
	
	  uint32_t	import_lookup_table_rva;				//	16	The RVA of the delay-load name table, which contains the names of the imports
	                                            //			that might need to be loaded. This matches the layout of the import name table. 
	                                            //			For more information, see Hint/Name Table.
	
	  uint32_t	bound_delay_import_table_rva;		//	20	The RVA of the bound delay-load address table, if it exists.
	
	  uint32_t	unload_delay_import_table_rva;	//	24	The RVA of the unload delay-load address table, if it exists. This is an exact 
	                                            //			copy of the delay import address table. If the caller unloads the DLL, this table 
	                                            //			should be copied back over the delay import address table so that subsequent calls 
	                                            //			to the DLL continue to use the thunking mechanism correctly.
	  
	  uint32_t	time_stamp;											//	28	The timestamp of the DLL to which this image has been bound.
	
	};
	
	static_assert(sizeof(PE_delay_import_directory_table) == 32);
	
	struct PE_export_directory_table
	{
	  uint32_t	export_flags;										//	0		Reserved, must be 0.
	
	  uint32_t	time_data_stamp;								//	4		The time and date that the export data was created.
	
	  uint16_t	major_version;									//	8		The major version number. The major and minor version numbers can be set by the user.
	
	  uint16_t	minor_version;									//	10	The minor version number.
	  
	  uint32_t	name_rva;												//	12	The address of the ASCII string that contains the name of the DLL. This address is 
	                                            //			relative to the image base.
	
	  uint32_t	ordinal_base;										//	16	The starting ordinal number for exports in this image. This field specifies the 
	                                            //			starting ordinal number for the export address table. It is usually set to 1.
	
	  uint32_t	address_table_entries;					//	20	The number of entries in the export address table.
	  
	  uint32_t	number_of_name_pointers;				//	24	The number of entries in the name pointer table. This is also the number of entries 
	                                            //			in the ordinal table.
	
	  uint32_t	export_address_table_rva;				//	28	The address of the export address table, relative to the image base.
	
	  uint32_t	name_pointer_rva;								//	32	The address of the export name pointer table, relative to the image base. The table 
	                                            //			size is given by the Number of Name Pointers field.
	
	  uint32_t	ordinal_table_rva;							//	36	The address of the ordinal table, relative to the image base.
	};
	
	static_assert(sizeof(PE_export_directory_table) == 40);

#pragma pack(pop)
}