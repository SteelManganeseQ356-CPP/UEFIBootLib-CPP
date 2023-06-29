/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#include "Elf.h"
namespace Boot::ELF64
{
    EFI_STATUS BootService_ELF::Relocate(IN EFI_HANDLE ImageHandle, IN CONST wchar_t *FileName)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        EFI_FILE_PROTOCOL *Kernel;
        Status = this->GetFileHandle(ImageHandle, (CHAR16 *)FileName, &Kernel);
        if (EFI_ERROR(Status))
        {
#ifdef LOG
            this->LogError(Status);
#endif
            return Status;
        }
#ifdef LOG
        this->LogTip("Kernel file handle is getted.\n");
#endif

        EFI_PHYSICAL_ADDRESS KernelBuffer;
        Status = this->ReadFile(Kernel, &KernelBuffer);

        if (EFI_ERROR(Status))
        {
#ifdef LOG
            this->LogError(Status);
#endif
            return Status;
        }
#ifdef LOG
        this->LogTip("Kernel is readed.\n");
#endif
        Status = CheckELF(KernelBuffer);
        if (EFI_ERROR(Status))
        {
            return Status;
        }
        Status = this->GetElfInfo(KernelBuffer);
        Status = this->LoadSegs(Kernel, &this->kerneData_.KernelAddress);
        Status = this->LoadSegments(KernelBuffer, &this->kerneData_.KernelAddress);
        this->kerneData_.KernelSize = this->putSize();
        Graphics::DrawStep(::Step++);
        return Status;
    }
    EFI_STATUS BootService_ELF::LoadSegments(IN EFI_PHYSICAL_ADDRESS KernelBufferBase, OUT EFI_PHYSICAL_ADDRESS *KernelEntry)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        ELF_HEADER_64 *ElfHeader = (ELF_HEADER_64 *)KernelBufferBase;
        PROGRAM_HEADER_64 *PHeader = (PROGRAM_HEADER_64 *)(KernelBufferBase + ElfHeader->Phoff);

        EFI_PHYSICAL_ADDRESS LowAddr = 0xFFFFFFFFFFFFFFFF;
        EFI_PHYSICAL_ADDRESS HighAddr = 0;

        for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
        {
            if (PHeader[i].Type == PT_LOAD)
            {
                if (LowAddr > PHeader[i].PAddress)
                {
                    LowAddr = PHeader[i].PAddress;
                }
                if (HighAddr < (PHeader[i].PAddress + PHeader[i].SizeInMemory))
                {
                    HighAddr = PHeader[i].PAddress + PHeader[i].SizeInMemory;
                }
            }
        }

        UINTN PageCount = ((HighAddr - LowAddr) >> 12) + 1;
        EFI_PHYSICAL_ADDRESS KernelRelocateBase;
        Status = gBS->AllocatePages(
            AllocateAnyPages,
            EfiLoaderCode,
            PageCount,
            &KernelRelocateBase);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)(L"Allocate pages for kernelrelocatebuffer error.\n"));
#ifdef LOG
            this->LogError(Status);
#endif
            return Status;
        }
#endif
        UINT64 RelocateOffset = KernelRelocateBase - LowAddr;
        UINT64 *ZeroStart = (UINT64 *)KernelRelocateBase;
        for (UINTN i = 0; i < (PageCount << 9); i++)
        {
            *ZeroStart = 0x000000000000;
            ZeroStart++;
        }

        for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
        {

            if (PHeader[i].Type == PT_LOAD)
            {
                UINT8 *SourceStart = (UINT8 *)KernelBufferBase + PHeader[i].Offset;
                UINT8 *DestStart = (UINT8 *)PHeader[i].VAddress + RelocateOffset;

                for (UINTN j = 0; j < PHeader[i].SizeInFile; j++)
                {
                    *DestStart = *SourceStart;
                    ++SourceStart;
                    ++DestStart;
                }
            }
        }
        *KernelEntry = ElfHeader->Entry + RelocateOffset;

#ifdef LOG
        if (EFI_ERROR(Status))
        {
            this->LogError(Status);
        }
        else
        {
            this->LogTip("SUCCESS:Segs are loaded.\n");
        }
#endif
        return Status;
    }
    EFI_STATUS BootService_ELF::CheckELF(IN EFI_PHYSICAL_ADDRESS KernelBuffer)
    {
        EFI_STATUS Status = EFI_SUCCESS;

        UINT32 Magic = 0;
        Magic += *(reinterpret_cast<UINT8 near*>(KernelBuffer) + 0) << 0 * 8;
        Magic += *(reinterpret_cast<UINT8 near*>(KernelBuffer) + 1) << 1 * 8;
        Magic += *(reinterpret_cast<UINT8 near*>(KernelBuffer) + 2) << 2 * 8;
        Magic += *(reinterpret_cast<UINT8 near*>(KernelBuffer) + 3) << 3 * 8;
        // Utils::GetValue(KernelBuffer, 0x00, 4);

        if (Magic != 0x464c457F)
        {
#ifdef LOG
            this->LogError(NOT_ELF);
#endif
            Status = NOT_ELF;
        }
        UINT8 Format = *(reinterpret_cast<UINT8 *>(KernelBuffer) + 4) << 0;
        if (Format == ELF_64)
        {
#ifdef LOG
            this->LogTip("SUCCESS: Elf file is 64-bit.\n");
#endif
        }
        else
        {
#ifdef LOG
            this->LogError(NOT_64_BIT);
#endif
            Status = NOT_64_BIT;
        }

        return Status;
    }

    EFI_STATUS BootService_ELF::LoadSegs(IN EFI_FILE_PROTOCOL *Kernel, OUT EFI_PHYSICAL_ADDRESS *KernelEntry)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        UINTN EHeaderSize = sizeof(ELF_HEADER_64);
        ELF_HEADER_64 *EHeader;
        Status = gBS->AllocatePool(EfiLoaderData, EHeaderSize, (VOID **)&EHeader);
        Kernel->SetPosition(Kernel, 0);
        Status = Kernel->Read(Kernel, &EHeaderSize, EHeader);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)(L"DEBUG:Error!Code=%d.\n"), Status);
#ifdef LOG
            this->LogError(Status);
#endif
            return Status;
        }
        Print((CHAR16 *)(L"DEBUG:EHeader@0x%llx, EHeader.PHoff=0x%llx.\n"), EHeader, EHeader->Phoff);
#endif
        UINTN PHeaderTableSize = EHeader->PHeadCount * EHeader->PHeadSize;
        PROGRAM_HEADER_64 *PHeaderTable;
        Status = gBS->AllocatePool(EfiLoaderData, PHeaderTableSize, (VOID **)&PHeaderTable);
        Kernel->SetPosition(Kernel, EHeader->Phoff);
        Status = Kernel->Read(Kernel, &PHeaderTableSize, PHeaderTable);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)(L"DEBUG:Error!Code=%d.\n"), Status);
#ifdef LOG
            this->LogError(Status);
#endif
            return Status;
        }

        Print(
            (CHAR16 *)(L"DEBUG:PHeaderTable@0x%llx, PHeader ccount %d, PHeader size %d.\n"),
            PHeaderTable,
            EHeader->PHeadCount,
            EHeader->PHeadSize);
#endif
        EFI_PHYSICAL_ADDRESS LowAddr = 0xFFFFFFFFFFFFFFFF;
        EFI_PHYSICAL_ADDRESS HighAddr = 0;

        for (UINTN i = 0; i < EHeader->PHeadCount; i++)
        {
            if (PHeaderTable[i].Type == PT_LOAD)
            {
                if (LowAddr > PHeaderTable[i].PAddress)
                {
                    LowAddr = PHeaderTable[i].PAddress;
                }
                if (HighAddr < (PHeaderTable[i].PAddress + PHeaderTable[i].SizeInMemory))
                {
                    HighAddr = PHeaderTable[i].PAddress + PHeaderTable[i].SizeInMemory;
                }
            }
        }

        UINTN PtLoadPageSize = ((HighAddr - LowAddr) >> 12) + 1;
#ifdef DEBUG
        Print((CHAR16 *)L"DEBUG:LowAddr=0x%llx, HighAddr=0x%llx, Size=%d, PageSize=%d.\n", LowAddr, HighAddr, HighAddr - LowAddr, PtLoadPageSize);
#endif
        EFI_PHYSICAL_ADDRESS RelocatedBuffer;
        Status = gBS->AllocatePages(AllocateAnyPages, EfiLoaderCode, PtLoadPageSize, &RelocatedBuffer);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"DEBUG:Error!Code=%d.\n", Status);
#ifdef LOG
            this->LogError(Status);
#endif
            return Status;
        }
#endif
        UINT64 *ZeroStart = (UINT64 *)RelocatedBuffer;
        for (UINTN i = 0; i < (PtLoadPageSize << 9); i++)
        {
            *ZeroStart = 0x000000000000;
            ZeroStart++;
        }
        for (UINTN i = 0; i < EHeader->PHeadCount; i++)
        {
            if (PHeaderTable[i].Type == PT_LOAD)
            {
                UINTN SegmentSize = PHeaderTable[i].SizeInFile;
                Kernel->SetPosition(Kernel, PHeaderTable[i].Offset);
                Kernel->Read(Kernel, &SegmentSize, (VOID *)(RelocatedBuffer + PHeaderTable[i].PAddress - LowAddr));
            }
        }
        *KernelEntry = EHeader->Entry + RelocatedBuffer - LowAddr;
        return Status;
    }
    EFI_STATUS BootService_ELF::GetElfInfo(IN EFI_PHYSICAL_ADDRESS KernelAddress)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        ELF_HEADER_64 *ElfHeader = (ELF_HEADER_64 *)KernelAddress;
        PROGRAM_HEADER_64 *PHeader = (PROGRAM_HEADER_64 *)(KernelAddress + ElfHeader->Phoff);

        EFI_PHYSICAL_ADDRESS LowAddr = 0xFFFFFFFFFFFFFFFF;
        EFI_PHYSICAL_ADDRESS HighAddr = 0;
        for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
        {
            if (PHeader[i].Type == PT_LOAD)
            {
                if (LowAddr > PHeader[i].PAddress)
                {
                    LowAddr = PHeader[i].PAddress;
                }
                if (HighAddr < PHeader[i].PAddress + PHeader[i].SizeInMemory)
                {
                    HighAddr = PHeader[i].PAddress + PHeader[i].Offset;
                }
            }
        }
        this->kerneData_.PhysicsKernelAddress = LowAddr;
        return Status;
    }
}