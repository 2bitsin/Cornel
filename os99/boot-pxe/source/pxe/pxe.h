#pragma once

#include <stdint.h>
#include <stddef.h>

#pragma pack(__push, 1)

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

struct PXEbang
{
  char Signature[4];
  uint8_t StructLength;
  uint8_t StructCksum;
  uint8_t StructRev;
  uint8_t _reserved_0;
  SEGOFFS16 UNDIROMID;
  SEGOFFS16 BaseROMID;
  SEGOFFS16 EntryPointSP;
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
  PXEbang far* PXEPtr;
};

#pragma pack(__pop)

uint16_t PXE_init();