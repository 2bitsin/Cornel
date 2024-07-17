#pragma once

#include <cstdint>
#include <cstddef>

#include <string_view>

namespace vfsio::elf
{
	using namespace std::string_view_literals;

	/* 32-bit ELF base types. */
	using Elf32_Addr   = std::uint32_t;
	using Elf32_Half   = std::uint16_t;
	using Elf32_Off    = std::uint32_t;
	using Elf32_Sword  = std::int32_t;
	using Elf32_Word   = std::uint32_t;
	
	/* 64-bit ELF base types. */
	using Elf64_Addr   = std::uint64_t;
	using Elf64_Half   = std::uint16_t;
	using Elf64_SHalf  = std::int16_t;
	using Elf64_Off    = std::uint64_t;
	using Elf64_Sword  = std::int32_t;
	using Elf64_Word   = std::uint32_t;
	using Elf64_Xword  = std::uint64_t;
	using Elf64_Sxword = std::int64_t;
	
	static inline constexpr const auto EM_NONE					= 0u;
	static inline constexpr const auto EM_M32						= 1u;
	static inline constexpr const auto EM_SPARC					= 2u;
	static inline constexpr const auto EM_386						= 3u;
	static inline constexpr const auto EM_68K						= 4u;
	static inline constexpr const auto EM_88K						= 5u;
//static inline constexpr const auto reserved					= 6u;
	static inline constexpr const auto EM_860						= 7u;
	static inline constexpr const auto EM_MIPS					= 8u;
	static inline constexpr const auto EM_S370					= 9u;
	static inline constexpr const auto EM_MIPS_RS3_LE		= 10u;
//static inline constexpr const auto reserved					= 11u;
//static inline constexpr const auto reserved					= 12u;
//static inline constexpr const auto reserved					= 13u;
//static inline constexpr const auto reserved					= 14u;
  static inline constexpr const auto EM_PARISC				= 15u;
//static inline constexpr const auto reserved					= 16u;
	static inline constexpr const auto EM_VPP500				= 17u;
	static inline constexpr const auto EM_SPARC32PLUS		= 18u;
	static inline constexpr const auto EM_960						= 19u; //	Intel 80960
	static inline constexpr const auto EM_PPC						= 20u; //	PowerPC
	static inline constexpr const auto EM_PPC64					= 21u; //	64-bit PowerPC
	static inline constexpr const auto EM_S390					= 22u; //	IBM System/390 Processor
//static inline constexpr const auto reserved					= 23u; // Reserved for future use
//static inline constexpr const auto reserved					= 24u; // Reserved for future use
//static inline constexpr const auto reserved					= 25u; // Reserved for future use
//static inline constexpr const auto reserved					= 26u; // Reserved for future use
//static inline constexpr const auto reserved					= 27u; // Reserved for future use
//static inline constexpr const auto reserved					= 28u; // Reserved for future use
//static inline constexpr const auto reserved					= 29u; // Reserved for future use
//static inline constexpr const auto reserved					= 30u; // Reserved for future use
//static inline constexpr const auto reserved					= 31u; // Reserved for future use
//static inline constexpr const auto reserved					= 32u; // Reserved for future use
//static inline constexpr const auto reserved					= 33u; // Reserved for future use
//static inline constexpr const auto reserved					= 34u; // Reserved for future use
//static inline constexpr const auto reserved					= 35u; // Reserved for future use
	static inline constexpr const auto EM_V800					= 36u; //	NEC V800
	static inline constexpr const auto EM_FR20					= 37u; //	Fujitsu FR20
	static inline constexpr const auto EM_RH32					= 38u; //	TRW RH-32
	static inline constexpr const auto EM_RCE						= 39u; //	Motorola RCE
	static inline constexpr const auto EM_ARM						= 40u; //	Advanced RISC Machines ARM
	static inline constexpr const auto EM_ALPHA					= 41u; //	Digital Alpha
	static inline constexpr const auto EM_SH						= 42u; //	Hitachi SH
	static inline constexpr const auto EM_SPARCV9				= 43u; //	SPARC Version 9
	static inline constexpr const auto EM_TRICORE				= 44u; //	Siemens TriCore embedded processor
	static inline constexpr const auto EM_ARC						= 45u; //	Argonaut RISC Core, Argonaut Technologies Inc.
	static inline constexpr const auto EM_H8_300				= 46u; //	Hitachi H8/300
	static inline constexpr const auto EM_H8_300H				= 47u; //	Hitachi H8/300H
	static inline constexpr const auto EM_H8S						= 48u; //	Hitachi H8S
	static inline constexpr const auto EM_H8_500				= 49u; //	Hitachi H8/500
	static inline constexpr const auto EM_IA_64					= 50u; //	Intel IA-64 processor architecture
	static inline constexpr const auto EM_MIPS_X				= 51u; //	Stanford MIPS-X
	static inline constexpr const auto EM_COLDFIRE			= 52u; //	Motorola ColdFire
	static inline constexpr const auto EM_68HC12				= 53u; //	Motorola M68HC12
	static inline constexpr const auto EM_MMA						= 54u; //	Fujitsu MMA Multimedia Accelerator
	static inline constexpr const auto EM_PCP						= 55u; //	Siemens PCP
	static inline constexpr const auto EM_NCPU					= 56u; //	Sony nCPU embedded RISC processor
	static inline constexpr const auto EM_NDR1					= 57u; //	Denso NDR1 microprocessor
	static inline constexpr const auto EM_STARCORE			= 58u; //	Motorola Star*Core processor
	static inline constexpr const auto EM_ME16					= 59u; //	Toyota ME16 processor
	static inline constexpr const auto EM_ST100					= 60u; //	STMicroelectronics ST100 processor
	static inline constexpr const auto EM_TINYJ					= 61u; //	Advanced Logic Corp. TinyJ embedded processor family
	static inline constexpr const auto EM_X86_64				= 62u; //	AMD x86-64 architecture
	static inline constexpr const auto EM_PDSP					= 63u; //	Sony DSP Processor
	static inline constexpr const auto EM_PDP10					= 64u; //	Digital Equipment Corp. PDP-10
	static inline constexpr const auto EM_PDP11					= 65u; //	Digital Equipment Corp. PDP-11
	static inline constexpr const auto EM_FX66					= 66u; //	Siemens FX66 microcontroller
	static inline constexpr const auto EM_ST9PLUS				= 67u; //	STMicroelectronics ST9+ 8/16 bit microcontroller
	static inline constexpr const auto EM_ST7						= 68u; //	STMicroelectronics ST7 8-bit microcontroller
	static inline constexpr const auto EM_68HC16				= 69u; //	Motorola MC68HC16 Microcontroller
	static inline constexpr const auto EM_68HC11				= 70u; //	Motorola MC68HC11 Microcontroller
	static inline constexpr const auto EM_68HC08				= 71u; //	Motorola MC68HC08 Microcontroller
	static inline constexpr const auto EM_68HC05				= 72u; //	Motorola MC68HC05 Microcontroller
	static inline constexpr const auto EM_SVX						= 73u; //	Silicon Graphics SVx
	static inline constexpr const auto EM_ST19					= 74u; //	STMicroelectronics ST19 8-bit microcontroller
	static inline constexpr const auto EM_VAX						= 75u; //	Digital VAX
	static inline constexpr const auto EM_CRIS					= 76u; //	Axis Communications 32-bit embedded processor
	static inline constexpr const auto EM_JAVELIN				= 77u; //	Infineon Technologies 32-bit embedded processor
	static inline constexpr const auto EM_FIREPATH			= 78u; //	Element 14 64-bit DSP Processor
	static inline constexpr const auto EM_ZSP						= 79u; //	LSI Logic 16-bit DSP Processor
	static inline constexpr const auto EM_MMIX					= 80u; //	Donald Knuth's educational 64-bit processor
	static inline constexpr const auto EM_HUANY					= 81u; //	Harvard University machine-independent object files
	static inline constexpr const auto EM_PRISM					= 82u; //	SiTera Prism
	static inline constexpr const auto EM_AVR						= 83u; //	Atmel AVR 8-bit microcontroller
	static inline constexpr const auto EM_FR30					= 84u; //	Fujitsu FR30
	static inline constexpr const auto EM_D10V					= 85u; //	Mitsubishi D10V
	static inline constexpr const auto EM_D30V					= 86u; //	Mitsubishi D30V
	static inline constexpr const auto EM_V850					= 87u; //	NEC v850
	static inline constexpr const auto EM_M32R					= 88u; //	Mitsubishi M32R
	static inline constexpr const auto EM_MN10300				= 89u; //	Matsushita MN10300
	static inline constexpr const auto EM_MN10200				= 90u; //	Matsushita MN10200
	static inline constexpr const auto EM_PJ						= 91u; //	picoJava
	static inline constexpr const auto EM_OPENRISC			= 92u; //	OpenRISC 32-bit embedded processor
	static inline constexpr const auto EM_ARC_A5				= 93u; //	ARC Cores Tangent-A5
	static inline constexpr const auto EM_XTENSA				= 94u; //	Tensilica Xtensa Architecture
	static inline constexpr const auto EM_VIDEOCORE			= 95u; //	Alphamosaic VideoCore processor
	static inline constexpr const auto EM_TMM_GPP				= 96u; //	Thompson Multimedia General Purpose Processor
	static inline constexpr const auto EM_NS32K					= 97u; //	National Semiconductor 32000 series
	static inline constexpr const auto EM_TPC						= 98u; //	Tenor Network TPC processor
	static inline constexpr const auto EM_SNP1K					= 99u; //	Trebia SNP 1000 processor
	static inline constexpr const auto EM_ST200					= 100u; //0	STMicroelectronics (www.st.com) ST200 microcontroller


	/* These constants are for the segment types stored in the image headers */
	static inline constexpr const auto PT_NULL          = 0u;
	static inline constexpr const auto PT_LOAD          = 1u;
	static inline constexpr const auto PT_DYNAMIC       = 2u;
	static inline constexpr const auto PT_INTERP        = 3u;
	static inline constexpr const auto PT_NOTE          = 4u;
	static inline constexpr const auto PT_SHLIB         = 5u;
	static inline constexpr const auto PT_PHDR          = 6u;
	static inline constexpr const auto PT_TLS           = 7u;               /* Thread local storage segment */
	static inline constexpr const auto PT_LOOS          = 0x60000000u;      /* OS-specific */
	static inline constexpr const auto PT_HIOS          = 0x6fffffffu;      /* OS-specific */
	static inline constexpr const auto PT_LOPROC        = 0x70000000u;
	static inline constexpr const auto PT_HIPROC        = 0x7fffffffu;
	static inline constexpr const auto PT_GNU_EH_FRAME	= (PT_LOOS + 0x474e550u);
	static inline constexpr const auto PT_GNU_STACK	    = (PT_LOOS + 0x474e551u);
	static inline constexpr const auto PT_GNU_RELRO	    = (PT_LOOS + 0x474e552u);
	static inline constexpr const auto PT_GNU_PROPERTY	= (PT_LOOS + 0x474e553u);


	/* ARM MTE memory tag segment type */
	static inline constexpr const auto PT_AARCH64_MEMTAG_MTE = (PT_LOPROC + 0x2u);

	/*
	 * Extended Numbering
	 *
	 * If the real number of program header table entries is larger than
	 * or equal to PN_XNUM(0xffff), it is set to sh_info field of the
	 * section header at index 0, and PN_XNUM is set to e_phnum
	 * field. Otherwise, the section header at index 0 is zero
	 * initialized, if it exists.
	 *
	 * Specifications are available in:
	 *
	 * - Oracle: Linker and Libraries.
	 *   Part No: 817–1984–19, August 2011.
	 *   https://docs.oracle.com/cd/E18752_01/pdf/817-1984.pdf
	 *
	 * - System V ABI AMD64 Architecture Processor Supplement
	 *   Draft Version 0.99.4,
	 *   January 13, 2010.
	 *   http://www.cs.washington.edu/education/courses/cse351/12wi/supp-docs/abi.pdf
	 */

	static inline constexpr const auto PN_XNUM	 = 0xffffu;

	/* These constants define the different elf file types */
	static inline constexpr const auto ET_NONE   = 0u;
	static inline constexpr const auto ET_REL    = 1u;
	static inline constexpr const auto ET_EXEC   = 2u;
	static inline constexpr const auto ET_DYN    = 3u;
	static inline constexpr const auto ET_CORE   = 4u;
	static inline constexpr const auto ET_LOPROC = 0xff00u;
	static inline constexpr const auto ET_HIPROC = 0xffffu;

	/* This is the info that is needed to parse the dynamic section of the file */
	static inline constexpr const auto DT_NULL			= 0u;
	static inline constexpr const auto DT_NEEDED		= 1u;
	static inline constexpr const auto DT_PLTRELSZ	= 2u;
	static inline constexpr const auto DT_PLTGOT		= 3u;
	static inline constexpr const auto DT_HASH			= 4u;
	static inline constexpr const auto DT_STRTAB		= 5u;
	static inline constexpr const auto DT_SYMTAB		= 6u;
	static inline constexpr const auto DT_RELA			= 7u;
	static inline constexpr const auto DT_RELASZ		= 8u;
	static inline constexpr const auto DT_RELAENT		= 9u;
	static inline constexpr const auto DT_STRSZ			= 10u;
	static inline constexpr const auto DT_SYMENT		= 11u;
	static inline constexpr const auto DT_INIT			= 12u;
	static inline constexpr const auto DT_FINI			= 13u;
	static inline constexpr const auto DT_SONAME		= 14u;
	static inline constexpr const auto DT_RPATH 		= 15u;
	static inline constexpr const auto DT_SYMBOLIC	= 16u;
	static inline constexpr const auto DT_REL	      = 17u;
	static inline constexpr const auto DT_RELSZ			= 18u;
	static inline constexpr const auto DT_RELENT		= 19u;
	static inline constexpr const auto DT_PLTREL		= 20u;
	static inline constexpr const auto DT_DEBUG			= 21u;
	static inline constexpr const auto DT_TEXTREL		= 22u;
	static inline constexpr const auto DT_JMPREL		= 23u;
	static inline constexpr const auto DT_ENCODING	= 32u;
	static inline constexpr const auto OLD_DT_LOOS	= 0x60000000u;
	static inline constexpr const auto DT_LOOS			= 0x6000000du;
	static inline constexpr const auto DT_HIOS			= 0x6ffff000u;
	static inline constexpr const auto DT_VALRNGLO	= 0x6ffffd00u;
	static inline constexpr const auto DT_VALRNGHI	= 0x6ffffdffu;
	static inline constexpr const auto DT_ADDRRNGLO	= 0x6ffffe00u;
	static inline constexpr const auto DT_ADDRRNGHI	= 0x6ffffeffu;
	static inline constexpr const auto DT_VERSYM		= 0x6ffffff0u;
	static inline constexpr const auto DT_RELACOUNT	= 0x6ffffff9u;
	static inline constexpr const auto DT_RELCOUNT	= 0x6ffffffau;
	static inline constexpr const auto DT_FLAGS_1		= 0x6ffffffbu;
	static inline constexpr const auto DT_VERDEF		= 0x6ffffffcu;
	static inline constexpr const auto DT_VERDEFNUM	= 0x6ffffffdu;
	static inline constexpr const auto DT_VERNEED		= 0x6ffffffeu;
	static inline constexpr const auto DT_VERNEEDNUM= 0x6fffffffu;
	static inline constexpr const auto OLD_DT_HIOS  = 0x6fffffffu;
	static inline constexpr const auto DT_LOPROC		= 0x70000000u;
	static inline constexpr const auto DT_HIPROC		= 0x7fffffffu;
	
	/* This info is needed when parsing the symbol table */
	static inline constexpr const auto STB_LOCAL    = 0u;
	static inline constexpr const auto STB_GLOBAL   = 1u;
	static inline constexpr const auto STB_WEAK     = 2u;
	
	static inline constexpr const auto STT_NOTYPE   = 0u;
	static inline constexpr const auto STT_OBJECT   = 1u;
	static inline constexpr const auto STT_FUNC     = 2u;
	static inline constexpr const auto STT_SECTION  = 3u;
	static inline constexpr const auto STT_FILE     = 4u;
	static inline constexpr const auto STT_COMMON   = 5u;
	static inline constexpr const auto STT_TLS      = 6u;
	
	static inline constexpr const auto ELF_ST_BIND   = [](auto&& x) constexpr { return ((x) >> 4);     };
	static inline constexpr const auto ELF_ST_TYPE   = [](auto&& x) constexpr { return ((x) & 0xf);    };
	static inline constexpr const auto ELF32_ST_BIND = [](auto&& x) constexpr { return ELF_ST_BIND(x); };
	static inline constexpr const auto ELF32_ST_TYPE = [](auto&& x) constexpr { return ELF_ST_TYPE(x); };
	static inline constexpr const auto ELF64_ST_BIND = [](auto&& x) constexpr { return ELF_ST_BIND(x); };
	static inline constexpr const auto ELF64_ST_TYPE = [](auto&& x) constexpr { return ELF_ST_TYPE(x); };

	struct Elf32_Dyn
	{
	  Elf32_Sword d_tag;
	  union{
	    Elf32_Sword	d_val;
	    Elf32_Addr	d_ptr;
	  } d_un;
	};
	
	struct Elf64_Dyn 
	{
	  Elf64_Sxword d_tag;		/* entry tag value */
	  union {
	    Elf64_Xword d_val;
	    Elf64_Addr d_ptr;
	  } d_un;
	};
	
	/* The following are used with relocations */
	static inline constexpr const auto ELF32_R_SYM  = [] (auto&& x) constexpr { return ((x) >> 8) ; };
	static inline constexpr const auto ELF32_R_TYPE = [] (auto&& x) constexpr { return ((x) & 0xff) ; };

	static inline constexpr const auto ELF64_R_SYM  = [] (auto&& i) constexpr { return (i) >> 32 ; };
	static inline constexpr const auto ELF64_R_TYPE = [] (auto&& i) constexpr { return (i) & 0xffffffff ; };

	struct Elf32_Rel 
	{
	  Elf32_Addr	r_offset;
	  Elf32_Word	r_info;
	} ;
	
	struct Elf64_Rel 
	{
	  Elf64_Addr r_offset;		/* Location at which to apply the action */
	  Elf64_Xword r_info;			/* index and type of relocation */
	} ;
	
	struct Elf32_Rela
	{
	  Elf32_Addr	r_offset;
	  Elf32_Word	r_info;
	  Elf32_Sword	r_addend;
	} ;
	
	struct Elf64_Rela
	{
	  Elf64_Addr r_offset;		/* Location at which to apply the action */
	  Elf64_Xword r_info;			/* index and type of relocation */
	  Elf64_Sxword r_addend;	/* Constant addend used to compute value */
	};
	
	struct Elf32_Sym
	{
	  Elf32_Word	st_name;
	  Elf32_Addr	st_value;
	  Elf32_Word	st_size;
	  unsigned char	st_info;
	  unsigned char	st_other;
	  Elf32_Half	st_shndx;
	};
	
	struct Elf64_Sym 
	{
	  Elf64_Word st_name;		/* Symbol name, index in string tbl */
	  unsigned char	st_info;	/* Type and binding attributes */
	  unsigned char	st_other;	/* No defined meaning, 0 */
	  Elf64_Half st_shndx;		/* Associated section index */
	  Elf64_Addr st_value;		/* Value of the symbol */
	  Elf64_Xword st_size;		/* Associated symbol size */
	};

	static inline constexpr const auto EI_NIDENT = 16u;

	struct Elf32_Ehdr
	{
	  unsigned char	e_ident[EI_NIDENT];
	  Elf32_Half	e_type;
	  Elf32_Half	e_machine;
	  Elf32_Word	e_version;
	  Elf32_Addr	e_entry;  /* Entry point */
	  Elf32_Off		e_phoff;
	  Elf32_Off		e_shoff;
	  Elf32_Word	e_flags;
	  Elf32_Half	e_ehsize;
	  Elf32_Half	e_phentsize;
	  Elf32_Half	e_phnum;
	  Elf32_Half	e_shentsize;
	  Elf32_Half	e_shnum;
	  Elf32_Half	e_shstrndx;
	};
	
	struct Elf64_Ehdr
	{
	  unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
	  Elf64_Half	e_type;
	  Elf64_Half	e_machine;
	  Elf64_Word	e_version;
	  Elf64_Addr	e_entry;		/* Entry point virtual address */
	  Elf64_Off		e_phoff;		/* Program header table file offset */
	  Elf64_Off		e_shoff;		/* Section header table file offset */
	  Elf64_Word	e_flags;
	  Elf64_Half	e_ehsize;
	  Elf64_Half	e_phentsize;
	  Elf64_Half	e_phnum;
	  Elf64_Half	e_shentsize;
	  Elf64_Half	e_shnum;
	  Elf64_Half	e_shstrndx;
	};

	/* These constants define the permissions on sections in the program
	   header, p_flags. */
	static inline constexpr const auto PF_R	= 0x4u;
	static inline constexpr const auto PF_W	= 0x2u;
	static inline constexpr const auto PF_X	= 0x1u;
	
	struct Elf32_Phdr 
	{
	  Elf32_Word	p_type;
	  Elf32_Off	p_offset;
	  Elf32_Addr	p_vaddr;
	  Elf32_Addr	p_paddr;
	  Elf32_Word	p_filesz;
	  Elf32_Word	p_memsz;
	  Elf32_Word	p_flags;
	  Elf32_Word	p_align;
	} ;
	
	struct Elf64_Phdr
	{
	  Elf64_Word	p_type;
	  Elf64_Word	p_flags;
	  Elf64_Off		p_offset;		/* Segment file offset */
	  Elf64_Addr	p_vaddr;		/* Segment virtual address */
	  Elf64_Addr	p_paddr;		/* Segment physical address */
	  Elf64_Xword p_filesz;		/* Segment size in file */
	  Elf64_Xword p_memsz;		/* Segment size in memory */
	  Elf64_Xword p_align;		/* Segment alignment, file & memory */
	} Elf64_Phdr;
	
	/* sh_type */
	static inline constexpr const auto SHT_NULL			=	0u;
	static inline constexpr const auto SHT_PROGBITS	=	1u;
	static inline constexpr const auto SHT_SYMTAB		=	2u;
	static inline constexpr const auto SHT_STRTAB		=	3u;
	static inline constexpr const auto SHT_RELA			=	4u;
	static inline constexpr const auto SHT_HASH			=	5u;
	static inline constexpr const auto SHT_DYNAMIC	=	6u;
	static inline constexpr const auto SHT_NOTE			=	7u;
	static inline constexpr const auto SHT_NOBITS		=	8u;
	static inline constexpr const auto SHT_REL			=	9u;
	static inline constexpr const auto SHT_SHLIB		=	10u;
	static inline constexpr const auto SHT_DYNSYM		=	11u;
	static inline constexpr const auto SHT_NUM			=	12u;
	static inline constexpr const auto SHT_LOPROC		=	0x70000000u;
	static inline constexpr const auto SHT_HIPROC		=	0x7fffffffu;
	static inline constexpr const auto SHT_LOUSER		=	0x80000000u;
	static inline constexpr const auto SHT_HIUSER		=	0xffffffffu;
	
	/* sh_flags */
	static inline constexpr const auto SHF_WRITE					= 0x1u;
	static inline constexpr const auto SHF_ALLOC					= 0x2u;
	static inline constexpr const auto SHF_EXECINSTR			= 0x4u;
	static inline constexpr const auto SHF_RELA_LIVEPATCH	= 0x00100000u;
	static inline constexpr const auto SHF_RO_AFTER_INIT	= 0x00200000u;
	static inline constexpr const auto SHF_MASKPROC				= 0xf0000000u;
	
	/* special section indexes */
	static inline constexpr const auto SHN_UNDEF			= 0u;
	static inline constexpr const auto SHN_LORESERVE	= 0xff00u;
	static inline constexpr const auto SHN_LOPROC			= 0xff00u;
	static inline constexpr const auto SHN_HIPROC			= 0xff1fu;
	static inline constexpr const auto SHN_LIVEPATCH	= 0xff20u;
	static inline constexpr const auto SHN_ABS				= 0xfff1u;
	static inline constexpr const auto SHN_COMMON			= 0xfff2u;
	static inline constexpr const auto SHN_HIRESERVE	= 0xffffu;
	 
	struct Elf32_Shdr 
	{
	  Elf32_Word	sh_name;
	  Elf32_Word	sh_type;
	  Elf32_Word	sh_flags;
	  Elf32_Addr	sh_addr;
	  Elf32_Off	sh_offset;
	  Elf32_Word	sh_size;
	  Elf32_Word	sh_link;
	  Elf32_Word	sh_info;
	  Elf32_Word	sh_addralign;
	  Elf32_Word	sh_entsize;
	};
	
	struct Elf64_Shdr
	{
	  Elf64_Word sh_name;		/* Section name, index in string tbl */
	  Elf64_Word sh_type;		/* Type of section */
	  Elf64_Xword sh_flags;		/* Miscellaneous section attributes */
	  Elf64_Addr sh_addr;		/* Section virtual addr at execution */
	  Elf64_Off sh_offset;		/* Section file offset */
	  Elf64_Xword sh_size;		/* Size of section in bytes */
	  Elf64_Word sh_link;		/* Index of another section */
	  Elf64_Word sh_info;		/* Additional section information */
	  Elf64_Xword sh_addralign;	/* Section alignment */
	  Elf64_Xword sh_entsize;	/* Entry size if section holds table */
	};
	
	static inline constexpr const auto EI_MAG0		= 0u;		/* e_ident[] indexes */
	static inline constexpr const auto EI_MAG1		= 1u;
	static inline constexpr const auto EI_MAG2		= 2u;
	static inline constexpr const auto EI_MAG3		= 3u;
	static inline constexpr const auto EI_CLASS		= 4u;
	static inline constexpr const auto EI_DATA		= 5u;
	static inline constexpr const auto EI_VERSION	= 6u;
	static inline constexpr const auto EI_OSABI		= 7u;
	static inline constexpr const auto EI_PAD			= 8u;
	
	static inline constexpr const auto ELFMAG0		= 0x7fu;		/* EI_MAG */
	static inline constexpr const auto ELFMAG1		= 'E';
	static inline constexpr const auto ELFMAG2		= 'L';
	static inline constexpr const auto ELFMAG3		= 'F';
	static inline constexpr const auto ELFMAG			= "\177ELF"sv;
	static inline constexpr const auto SELFMAG		= 4u;
	
	static inline constexpr const auto ELFCLASSNONE	= 0u;		/* EI_CLASS */
	static inline constexpr const auto ELFCLASS32		= 1u;
	static inline constexpr const auto ELFCLASS64		= 2u;
	static inline constexpr const auto ELFCLASSNUM	= 3u;
	
	static inline constexpr const auto ELFDATANONE	= 0u;		/* e_ident[EI_DATA] */
	static inline constexpr const auto ELFDATA2LSB	= 1u;
	static inline constexpr const auto ELFDATA2MSB	= 2u;
	
	static inline constexpr const auto EV_NONE			= 0u;		/* e_version, EI_VERSION */
	static inline constexpr const auto EV_CURRENT		= 1u;
	static inline constexpr const auto EV_NUM				= 2u;
	
	static inline constexpr const auto ELFOSABI_NONE	= 0u;
	static inline constexpr const auto ELFOSABI_LINUX	= 3u;
	
#ifndef ELF_OSABI
	static inline constexpr const auto ELF_OSABI = ELFOSABI_NONE;
#endif
	
	/*
	 * Notes used in ET_CORE. Architectures export some of the arch register sets
	 * using the corresponding note types via the PTRACE_GETREGSET and
	 * PTRACE_SETREGSET requests.
	 * The note name for all these is "LINUX".
	 */
	static inline constexpr const auto NT_PRSTATUS		= 1u;
	static inline constexpr const auto NT_PRFPREG			= 2u;
	static inline constexpr const auto NT_PRPSINFO		= 3u;
	static inline constexpr const auto NT_TASKSTRUCT	= 4u;
	static inline constexpr const auto NT_AUXV				= 6u;
	/*
	 * Note to userspace developers: size of NT_SIGINFO note may increase
	 * in the future to accomodate more fields, don't assume it is fixed!
	 */
	static inline constexpr const auto NT_SIGINFO								= 0x53494749u;
	static inline constexpr const auto NT_FILE									= 0x46494c45u;
	static inline constexpr const auto NT_PRXFPREG							= 0x46e62b7fu;      /* copied from gdb5.1/include/elf/common.h */
	static inline constexpr const auto NT_PPC_VMX								= 0x100u;		/* PowerPC Altivec/VMX registers */
	static inline constexpr const auto NT_PPC_SPE								= 0x101u;		/* PowerPC SPE/EVR registers */
	static inline constexpr const auto NT_PPC_VSX								= 0x102u;		/* PowerPC VSX registers */
	static inline constexpr const auto NT_PPC_TAR								= 0x103u;		/* Target Address Register */
	static inline constexpr const auto NT_PPC_PPR								= 0x104u;		/* Program Priority Register */
	static inline constexpr const auto NT_PPC_DSCR							= 0x105u;		/* Data Stream Control Register */
	static inline constexpr const auto NT_PPC_EBB								= 0x106u;		/* Event Based Branch Registers */
	static inline constexpr const auto NT_PPC_PMU								= 0x107u;		/* Performance Monitor Registers */
	static inline constexpr const auto NT_PPC_TM_CGPR						= 0x108u;		/* TM checkpointed GPR Registers */
	static inline constexpr const auto NT_PPC_TM_CFPR						= 0x109u;		/* TM checkpointed FPR Registers */
	static inline constexpr const auto NT_PPC_TM_CVMX						= 0x10au;		/* TM checkpointed VMX Registers */
	static inline constexpr const auto NT_PPC_TM_CVSX						= 0x10bu;		/* TM checkpointed VSX Registers */
	static inline constexpr const auto NT_PPC_TM_SPR						= 0x10cu;		/* TM Special Purpose Registers */
	static inline constexpr const auto NT_PPC_TM_CTAR						= 0x10du;		/* TM checkpointed Target Address Register */
	static inline constexpr const auto NT_PPC_TM_CPPR						= 0x10eu;		/* TM checkpointed Program Priority Register */
	static inline constexpr const auto NT_PPC_TM_CDSCR					= 0x10fu;		/* TM checkpointed Data Stream Control Register */
	static inline constexpr const auto NT_PPC_PKEY							= 0x110u;		/* Memory Protection Keys registers */
	static inline constexpr const auto NT_386_TLS								= 0x200u;		/* i386 TLS slots (struct user_desc) */
	static inline constexpr const auto NT_386_IOPERM						= 0x201u;		/* x86 io permission bitmap (1=deny) */
	static inline constexpr const auto NT_X86_XSTATE						= 0x202u;		/* x86 extended state using xsave */
	static inline constexpr const auto NT_S390_HIGH_GPRS				= 0x300u;	/* s390 upper register halves */
	static inline constexpr const auto NT_S390_TIMER						= 0x301u;		/* s390 timer register */
	static inline constexpr const auto NT_S390_TODCMP						= 0x302u;		/* s390 TOD clock comparator register */
	static inline constexpr const auto NT_S390_TODPREG					= 0x303u;		/* s390 TOD programmable register */
	static inline constexpr const auto NT_S390_CTRS							= 0x304u;		/* s390 control registers */
	static inline constexpr const auto NT_S390_PREFIX						= 0x305u;		/* s390 prefix register */
	static inline constexpr const auto NT_S390_LAST_BREAK				= 0x306u;	/* s390 breaking event address */
	static inline constexpr const auto NT_S390_SYSTEM_CALL			= 0x307u;	/* s390 system call restart data */
	static inline constexpr const auto NT_S390_TDB							= 0x308u;		/* s390 transaction diagnostic block */
	static inline constexpr const auto NT_S390_VXRS_LOW					= 0x309u;	/* s390 vector registers 0-15 upper half */
	static inline constexpr const auto NT_S390_VXRS_HIGH				= 0x30au;	/* s390 vector registers 16-31 */
	static inline constexpr const auto NT_S390_GS_CB						= 0x30bu;		/* s390 guarded storage registers */
	static inline constexpr const auto NT_S390_GS_BC						= 0x30cu;		/* s390 guarded storage broadcast control block */
	static inline constexpr const auto NT_S390_RI_CB						= 0x30du;		/* s390 runtime instrumentation */
	static inline constexpr const auto NT_S390_PV_CPU_DATA			= 0x30eu;	/* s390 protvirt cpu dump data */
	static inline constexpr const auto NT_ARM_VFP								= 0x400u;		/* ARM VFP/NEON registers */
	static inline constexpr const auto NT_ARM_TLS								= 0x401u;		/* ARM TLS register */
	static inline constexpr const auto NT_ARM_HW_BREAK					= 0x402u;		/* ARM hardware breakpoint registers */
	static inline constexpr const auto NT_ARM_HW_WATCH					= 0x403u;		/* ARM hardware watchpoint registers */
	static inline constexpr const auto NT_ARM_SYSTEM_CALL				= 0x404u;	/* ARM system call number */
	static inline constexpr const auto NT_ARM_SVE								= 0x405u;		/* ARM Scalable Vector Extension registers */
	static inline constexpr const auto NT_ARM_PAC_MASK					= 0x406u;	/* ARM pointer authentication code masks */
	static inline constexpr const auto NT_ARM_PACA_KEYS					= 0x407u;	/* ARM pointer authentication address keys */
	static inline constexpr const auto NT_ARM_PACG_KEYS					= 0x408u;	/* ARM pointer authentication generic key */
	static inline constexpr const auto NT_ARM_TAGGED_ADDR_CTRL	= 0x409u;	/* arm64 tagged address control (prctl()) */
	static inline constexpr const auto NT_ARM_PAC_ENABLED_KEYS	= 0x40au;	/* arm64 ptr auth enabled keys (prctl()) */
	static inline constexpr const auto NT_ARM_SSVE							= 0x40bu;		/* ARM Streaming SVE registers */
	static inline constexpr const auto NT_ARM_ZA								= 0x40cu;		/* ARM SME ZA registers */
	static inline constexpr const auto NT_ARC_V2								= 0x600u;		/* ARCv2 accumulator/extra registers */
	static inline constexpr const auto NT_VMCOREDD							= 0x700u;		/* Vmcore Device Dump Note */
	static inline constexpr const auto NT_MIPS_DSP							= 0x800u;		/* MIPS DSP ASE registers */
	static inline constexpr const auto NT_MIPS_FP_MODE					= 0x801u;		/* MIPS floating-point mode */
	static inline constexpr const auto NT_MIPS_MSA							= 0x802u;		/* MIPS SIMD registers */
	static inline constexpr const auto NT_LOONGARCH_CPUCFG			= 0xa00u;	/* LoongArch CPU config registers */
	static inline constexpr const auto NT_LOONGARCH_CSR					= 0xa01u;	/* LoongArch control and status registers */
	static inline constexpr const auto NT_LOONGARCH_LSX					= 0xa02u;	/* LoongArch Loongson SIMD Extension registers */
	static inline constexpr const auto NT_LOONGARCH_LASX				= 0xa03u;	/* LoongArch Loongson Advanced SIMD Extension registers */
	static inline constexpr const auto NT_LOONGARCH_LBT					= 0xa04u;	/* LoongArch Loongson Binary Translation registers */
	
	/* Note types with note name "GNU" */
	static inline constexpr const auto NT_GNU_PROPERTY_TYPE_0	  = 5u;
	
	/* Note header in a PT_NOTE section */
	struct Elf32_Nhdr 
	{
	  Elf32_Word	n_namesz;	/* Name size */
	  Elf32_Word	n_descsz;	/* Content size */
	  Elf32_Word	n_type;		/* Content type */
	};
	
	/* Note header in a PT_NOTE section */
	struct Elf64_Nhdr 
	{
	  Elf64_Word n_namesz;	/* Name size */
	  Elf64_Word n_descsz;	/* Content size */
	  Elf64_Word n_type;	/* Content type */
	} ;
	
	/* .note.gnu.property types for EM_AARCH64: */
	static inline constexpr const auto GNU_PROPERTY_AARCH64_FEATURE_1_AND =	0xc0000000u;
	
	/* Bits for GNU_PROPERTY_AARCH64_FEATURE_1_BTI */
	static inline constexpr const auto GNU_PROPERTY_AARCH64_FEATURE_1_BTI	= (1U << 0u);
	
}