#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace vfsio::vfat
{
#pragma pack(push, 1)
	
	struct small_bpb
	{
	  std::uint8_t  BS_jmpBoot[3];
	  std::uint8_t  BS_OEMName[8];
	  std::uint16_t BPB_BytsPerSec;
	  std::uint8_t  BPB_SecPerClus;
	  std::uint16_t BPB_RsvdSecCnt;
	  std::uint8_t  BPB_NumFATs;
	  std::uint16_t BPB_RootEntCnt;
	  std::uint16_t BPB_TotSec16;
	  std::uint8_t  BPB_Media;
	  std::uint16_t BPB_FATSz16;
	  std::uint16_t BPB_SecPerTrk;
	  std::uint16_t BPB_NumHeads;
	  std::uint32_t BPB_HiddSec;
	  std::uint32_t BPB_TotSec32;
	  std::uint8_t  BS_DrvNum;
	  std::uint8_t  BS_Reserved1;
	  std::uint8_t  BS_BootSig;
	  std::uint32_t BS_VolID;
	  std::uint8_t  BS_VolLab[11];
	  std::uint8_t  BS_FilSysType[8];
	  std::uint8_t  Reserved[448];
	  std::uint16_t Signature;
	};
	
	struct large_bpb
	{
	  std::uint8_t  BS_jmpBoot[3];
	  std::uint8_t  BS_OEMName[8];
	  std::uint16_t BPB_BytsPerSec;
	  std::uint8_t  BPB_SecPerClus;
	  std::uint16_t BPB_RsvdSecCnt;
	  std::uint8_t  BPB_NumFATs;
	  std::uint16_t BPB_RootEntCnt;
	  std::uint16_t BPB_TotSec16;
	  std::uint8_t  BPB_Media;
	  std::uint16_t BPB_FATSz16;
	  std::uint16_t BPB_SecPerTrk;
	  std::uint16_t BPB_NumHeads;
	  std::uint32_t BPB_HiddSec;
	  std::uint32_t BPB_TotSec32;
	  std::uint32_t BPB_FATSz32;
	  std::uint16_t BPB_ExtFlags;
	  std::uint16_t BPB_FSVer;
	  std::uint32_t BPB_RootClus;
	  std::uint16_t BPB_FSInfo;
	  std::uint16_t BPB_BkBootSec;
	  std::uint8_t  BPB_Reserved[12];
	  std::uint8_t  BS_DrvNum;
	  std::uint8_t  BS_Reserved1;
	  std::uint8_t  BS_BootSig;
	  std::uint32_t BS_VolID;
	  std::uint8_t  BS_VolLab[11];
	  std::uint8_t  BS_FilSysType[8];
	  std::uint8_t  Reserved[420];
	  std::uint16_t Signature;
	};
	
	struct directory_entry
	{
	  char					DIR_Name[11];
	  std::uint8_t  DIR_Attr;
	  std::uint8_t  DIR_NTRes;
	  std::uint8_t  DIR_CrtTimeTenth;
	  std::uint16_t DIR_CrtTime;
	  std::uint16_t DIR_CrtDate;
	  std::uint16_t DIR_LstAccDate;
	  std::uint16_t DIR_FstClusHI;
	  std::uint16_t DIR_WrtTime;
	  std::uint16_t DIR_WrtDate;
	  std::uint16_t DIR_FstClusLO;
	  std::uint32_t DIR_FileSize;
	};
	
#pragma pack(pop)
	
	using namespace std::string_view_literals;
	
	static inline constexpr const auto fat_max_fat12_cluster = std::uint32_t (0xFF5u ) ;
	static inline constexpr const auto fat_max_fat16_cluster = std::uint32_t (0xFFF5u) ;
	static inline constexpr const auto fat_bs_signature      = std::uint16_t (0xAA55u) ;
	static inline constexpr const auto fat_bs_bootsig        = std::uint8_t  (0x29u  ) ;
	static inline constexpr const auto fat_bs_jmp1           = std::uint8_t  (0xEBu  ) ;
	static inline constexpr const auto fat_bs_jmp2           = std::uint8_t  (0xE9u  ) ;
	static inline constexpr const auto fat_filsystype        = "FAT     "sv;
	static inline constexpr const auto fat12_filsystype      = "FAT12   "sv;
	static inline constexpr const auto fat16_filsystype      = "FAT16   "sv;
	static inline constexpr const auto fat32_filsystype      = "FAT32   "sv;	

}