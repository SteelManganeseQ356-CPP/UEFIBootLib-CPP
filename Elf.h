/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _ELF_H_
#define _ELF_H_

#include "File.h"
#include "Log.h"
#include "Utils.h"
#include "Graphics.h"
#define ELF_64 2
#define NOT_64_BIT -2

#define PT_LOAD 1

#pragma pack(1)

#include "Elf32.h" // 对32bits操作系统的支持
namespace Boot::ELF64
{
    typedef struct
    {
        UINT32 Magic;      // 0x00
        UINT8 Format;      // 0x04,32 or 64 bits format
        UINT8 Endianness;  // 0x05
        UINT8 Version;     // 0x06
        UINT8 OSAbi;       // 0x07
        UINT8 AbiVersion;  // 0x08
        UINT8 Rserved[7];  // 0x09
        UINT16 Type;       // 0x10
        UINT16 Machine;    // 0x12
        UINT32 ElfVersion; // 0x14
        UINT64 Entry;      // 0x18
        UINT64 Phoff;      // 0x20
        UINT64 Shoff;      // 0x28
        UINT32 Flags;      // 0x30
        UINT16 HeadSize;   // 0x34,size of elf head
        UINT16 PHeadSize;  // 0x36, size of a program header table entry
        UINT16 PHeadCount; // 0x38, count of entries in the program header table
        UINT16 SHeadSize;  // 0x3A, size of a section header table entry
        UINT16 SHeadCount; // 0x3C, count of entries in the section header table
        UINT16 SNameIndex; // 0x3E, index of entry that contains the section names
    } ELF_HEADER_64;

    typedef struct
    {
        UINT32 Type;         // 0x00, type of segment
        UINT32 Flags;        // 0x04 Segment-dependent flags
        UINT64 Offset;       // 0x08
        UINT64 VAddress;     // 0x10
        UINT64 PAddress;     // 0x18
        UINT64 SizeInFile;   // 0x20
        UINT64 SizeInMemory; // 0x28
        UINT64 Align;        // 0x30

    } PROGRAM_HEADER_64;
#pragma pack()

    typedef struct
    {
        EFI_PHYSICAL_ADDRESS KernelAddress;
        EFI_PHYSICAL_ADDRESS PhysicsKernelAddress;
        CHAR8 *SystemName;
        UINT32 BuildNumber;
        UINT64 KernelSize;
        /* data */
    } KernelConfig;

    class BootService_ELF
        : public File::BootService_File,
          private Log::BootService_Log
    {
    private:
    public:
        BootService_ELF(VOID) {}
        BootService_ELF(IN KernelConfig &_kerneData, IN EFI_HANDLE ImageHandle, IN CONST wchar_t far *LogFileName)
            : BootService_Log{ImageHandle, LogFileName},
              kerneData_{_kerneData} { Graphics::DrawStep(::Step++); }

        ~BootService_ELF(VOID) {}

    public:
        EFI_STATUS CheckELF(IN EFI_PHYSICAL_ADDRESS KernelBuffer);

        EFI_STATUS LoadSegments(IN EFI_PHYSICAL_ADDRESS KernelBufferBase, OUT EFI_PHYSICAL_ADDRESS *KernelEntry);

        EFI_STATUS LoadSegs(IN EFI_FILE_PROTOCOL *Kernel, OUT EFI_PHYSICAL_ADDRESS *KernelEntry);

        EFI_STATUS Relocate(IN EFI_HANDLE ImageHandle, IN CONST wchar_t *FileName);

        EFI_STATUS GetElfInfo(IN EFI_PHYSICAL_ADDRESS KernelAddress);

    public:
        CONST KernelConfig &putKernelConfig(VOID) CONST
        {
            return this->kerneData_;
        }

    private:
        KernelConfig kerneData_;
    };
}
#endif // !_ELF_H_