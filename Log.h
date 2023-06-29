/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _LOG_H_
#define _LOG_H_
#include "include.h"
#include "Utils.h"
namespace Boot::Log
{
    /**
     * @brief 启动时服务——日志(Logger)
     */
    class BootService_Log
    {
    public:
        BootService_Log(VOID) {}

    public:
        /**
         * @brief 初始化日志
         * @param ImageHandle 屏幕参数
         */
        BootService_Log(IN EFI_HANDLE ImageHandle, IN CONST wchar_t far* LogFileName = nullptr);
        ~BootService_Log(VOID){};

    public:
        /**
         * @brief 保存一般提示信息
         * @param Message 一般提示信息
         */
        EFI_STATUS LogTip(IN CONST CHAR8 *Message);
        /**
         * @brief 根据错误码保存日志信息
         * @param Code 错误码
         */
        EFI_STATUS LogError(IN CONST EFI_STATUS Code);
        /**
         * @brief 关闭日志文件
         */
        EFI_STATUS Close(VOID);

    private: // data
        /*
            两个参数要来存储数据
        */

        EFI_STATUS ErrorCode = EFI_SUCCESS;
        EFI_FILE_PROTOCOL *LogFile = nullptr;
    };

}
#endif // !_LOG_H_