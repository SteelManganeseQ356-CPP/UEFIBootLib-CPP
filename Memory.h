/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "include.h"
#include "Utils.h"
#include "Graphics.h"
#include "Log.h"
namespace Boot::Memory
{
    typedef struct
    {
        UINT64 MemorySize;
        VOID huge *Buffer;
        UINT64 MemoryKey;
        UINT64 DescriptorSize;
        UINT32 DescriptorVersion;
    } MemoryConfig;

    class BootService_Memory
    {
    public:
        /**
         * @brief 获取MemoryMap
         * @return EFI_STATUS
         */
        EFI_STATUS GetMemMap(VOID);

    public:
        BootService_Memory(VOID)
            : MemoryData_{{0}} { Graphics::DrawStep(::Step++); }
        BootService_Memory(IN MemoryConfig &_MemoryData)
            : MemoryData_{_MemoryData} { Graphics::DrawStep(::Step++); }
        ~BootService_Memory(VOID){};

    public:
        /**
         * @brief 获取内存信息
         * @return 引导内存信息分布图
         */
        CONST MemoryConfig &putMemoryConfig(VOID) CONST
        {
            return this->MemoryData_;
        }

    private:
        MemoryConfig MemoryData_;
    };
}
#endif /* _MEMORY_H_ */
