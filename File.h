/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _FILE_H_
#define _FILE_H_
#include "include.h"
#include "Log.h"
namespace Boot::File
{

    class BootService_File
    {
    public:
        BootService_File(VOID) {};
        ~BootService_File(VOID){};
        /**
         * @brief 获取文件头
         * @param ImageHandle 屏幕数据
         */
        EFI_STATUS GetFileHandle(IN EFI_HANDLE ImageHandle, IN CHAR16 *FileName, OUT EFI_FILE_PROTOCOL **FileHandle);

        /**
         * @brief 读取文件
         */
        EFI_STATUS ReadFile(IN EFI_FILE_PROTOCOL *File, OUT EFI_PHYSICAL_ADDRESS *FileBase);

        UINT32 putSize(VOID) CONST
        {
            return this->KernelSize;
        }

    private:
        UINT32 KernelSize;
    };
}
#endif // !_FILE_H_