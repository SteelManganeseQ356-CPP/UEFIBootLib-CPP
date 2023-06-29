/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _BOOT_HPP_
#define _BOOT_HPP_
#include "Graphics.h"
#include "Elf.h"
#include "Memory.h"
#include "include.h"
#include "Log.h"
#include "Utils.h"
#pragma pack(1)
namespace Boot
{

    typedef struct
    {
        Graphics::GraphicsConfig GraphicsData;
        Graphics::BmpConfig BmpData;
        Memory::MemoryConfig MemoryData;
        ELF64::KernelConfig KernelData;

    } BootConfig; // boot阶段数据

    /* boot阶段服务 */
    class BootService : public Graphics::BootService_Graphics,
                        public Memory::BootService_Memory,
                        public ELF64::BootService_ELF
    {
    public:
        BootService(IN EFI_HANDLE ImageHandle, IN BootConfig &_BootData, IN CONST wchar_t far *LogFileName)
            : BootService_Graphics{_BootData.GraphicsData, _BootData.BmpData, ImageHandle, LogFileName},
              BootService_Memory{_BootData.MemoryData},
              BootService_ELF{_BootData.KernelData, ImageHandle, LogFileName} {}
        ~BootService(VOID){};

    public:
        /**
         * @brief 跳转内核
         */
        EFI_STATUS JumpToKernel(IN EFI_HANDLE ImageHandle);
    };
}
#pragma pack()

#endif // !_BOOT_HPP_