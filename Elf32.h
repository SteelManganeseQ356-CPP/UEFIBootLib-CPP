/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _ELF32_H_
#define _ELF32_H_
#include "include.h"

#include "File.h"
#include "Log.h"
#include "Utils.h"

#define NOT_ELF -1
#define ELF_32 1
namespace Boot::ELF32
{
    typedef struct
    {
        UINT32 Magic;      // 0x00
        UINT8 Format;      // 0x04,32 or 64 bits format 1=32, 2=64
        UINT8 Endianness;  // 0x05
        UINT8 Version;     // 0x06
        UINT8 OSAbi;       // 0x07
        UINT8 AbiVersion;  // 0x08
        UINT8 Rserved[7];  // 0x09
        UINT16 Type;       // 0x10
        UINT16 Machine;    // 0x12
        UINT32 ElfVersion; // 0x14
        UINT32 Entry;      // 0x18
        UINT32 Phoff;      // 0x1C
        UINT32 Shoff;      // 0x20
        UINT32 Flags;      // 0x24
        UINT16 HeadSize;   // 0x28,size of elf head
        UINT16 PHeadSize;  // 0x2A, size of a program header table entry
        UINT16 PHeadCount; // 0x2C, count of entries in the program header table
        UINT16 SHeadSize;  // 0x2E, size of a section header table entry
        UINT16 SHeadCount; // 0x30, count of entries in the section header table
        UINT16 SNameIndex; // 0x32, index of entry that contains the section names

    } ELF_HEADER_32;

    typedef struct
    {
        UINT32 Type;         // 0x00, type of segment
        UINT32 Offset;       // 0x04, Offset of the segment in the file image
        UINT32 VAddress;     // 0x08, Virtual address of the segment in memory
        UINT32 PAddress;     // 0x0C, reserved for segment`s physical address
        UINT32 SizeInFile;   // 0x10, size in bytes of the segment in the file image
        UINT32 SizeInMemory; // 0x14, size in bytes of the segment in memory
        UINT32 Flags;        // 0x18
        UINT32 Align;        // 0x1C, 0 and 1
    } PROGRAM_HEADER_32;

}
#endif /* !_ELF32_H_ */