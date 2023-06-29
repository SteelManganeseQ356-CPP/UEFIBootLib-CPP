/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#include "Memory.h"

namespace QuantumNEC::Boot::Memory
{
    EFI_STATUS BootService_Memory::GetMemMap(VOID)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        Status = gBS->AllocatePool(EfiLoaderData, this->MemoryData_.MemorySize, &this->MemoryData_.Buffer);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR: Get Memory Map Error.\n\r");
            return Status;
        }
#endif

        gBS->GetMemoryMap(
            &this->MemoryData_.MemorySize,
            (EFI_MEMORY_DESCRIPTOR near *)this->MemoryData_.Buffer,
            &this->MemoryData_.MemoryKey,
            &this->MemoryData_.DescriptorSize,
            &this->MemoryData_.DescriptorVersion);
        Graphics::DrawStep(::Step++);
        return Status;
    }
}