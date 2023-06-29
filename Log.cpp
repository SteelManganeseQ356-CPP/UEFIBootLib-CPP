/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#include "Log.h"
namespaceBoot::Log
{
    BootService_Log::BootService_Log(IN EFI_HANDLE ImageHandle, IN CONST wchar_t far *LogFileName)
    {
        /* 初始化日志 */
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem = NULL;
        UINTN HandleCount = 0;
        EFI_HANDLE *Buffer = NULL;

        this->ErrorCode = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiSimpleFileSystemProtocolGuid,
            NULL,
            &HandleCount,
            &Buffer);
#ifdef DEBUG
        if (EFI_ERROR(this->ErrorCode))
        {
            Print((CHAR16 near *)L"ERROR: Failed to LocateHanleBuffer of SimpleFileSystemProtocol.\n");
        }
        else
            Print((CHAR16 near *)L"SUCCESS: Get %d handles that supported SimpleFileSystemProtocol.\n", HandleCount);
#endif

        this->ErrorCode = gBS->OpenProtocol(
            Buffer[0],
            &gEfiSimpleFileSystemProtocolGuid,
            (VOID near **)&FileSystem,
            ImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL);

#ifdef DEBUG
        if (EFI_ERROR(this->ErrorCode))
        {
            Print((CHAR16 near *)L"ERROR: Failed to open first handle that supported SimpleFileSystemProtocol.\n");
        }
        else
            Print((CHAR16 near *)L"SUCCESS: SimpleFileSystemProtocol is opened with first handle.\n");
#endif

        EFI_FILE_PROTOCOL near *File = NULL;
        this->ErrorCode = FileSystem->OpenVolume(
            FileSystem,
            &File);

#ifdef DEBUG
        if (EFI_ERROR(this->ErrorCode))
        {
            Print((CHAR16 *)L"ERROR: Failed to open volume.\n");
        }
        else
            Print((CHAR16 *)L"SUCCESS: Volume is opened.\n");
#endif
        if (LogFileName)
        {
            File->Open(
                File,
                &LogFile,
                (CHAR16 near *)L"EFI\\Log\\SystemLog.log",
                EFI_FILE_MODE_READ |
                    EFI_FILE_MODE_WRITE |
                    EFI_FILE_MODE_CREATE,
                EFI_FILE_ARCHIVE);
        }
        else
        {
            File->Open(
                File,
                &LogFile,
                (CHAR16 near *)LogFileName,
                EFI_FILE_MODE_READ |
                    EFI_FILE_MODE_WRITE |
                    EFI_FILE_MODE_CREATE,
                EFI_FILE_ARCHIVE);
        }
#ifdef DEBUG
        if (EFI_ERROR(this->ErrorCode))
        {
            Print((CHAR16 near *)L"ERROR: Failed to open file");
        }
        else
            Print((CHAR16 near *)L"SUCCESS: File is opened.\n");
#endif
        if (EFI_ERROR(this->ErrorCode))
            this->LogError(this->ErrorCode);
        else
            this->LogTip("Init Log Success.\n");
    }
    EFI_STATUS BootService_Log::LogError(IN CONST EFI_STATUS Code)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        switch (Code)
        {
        case EFI_INVALID_PARAMETER:
            Status = this->LogTip("ERROR: EFI_INVALID_PARAMETER.\n");
            break;
        case EFI_NOT_FOUND:
            Status = this->LogTip("ERROR: EFI_NOT_FOUND.\n");
            break;
        case EFI_OUT_OF_RESOURCES:
            Status = this->LogTip("ERROR: EFI_OUT_OF_RESOURCES.\n");
            break;
        default: // 最后一种情况
            Status = this->LogTip("ERROR: Something must be wrong.\n");
            break;
        }
        return Status;
    }
    EFI_STATUS BootService_Log::LogTip(IN CONST CHAR8 far *Message)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        UINTN MsgLen = AsciiStrLen(Message);
        Status = LogFile->Write(LogFile, &MsgLen, (VOID near *)Message); // 写入文件

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 near *)L"ERROR: Failed to write LogFile.\n");
            return Status;
        }
#endif

        return Status;
    }
    EFI_STATUS BootService_Log::Close(VOID)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        Status = LogFile->Close(LogFile);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 near *)L"ERROR:Failed to Close LogFile.\n");
            return Status;
        }
        Print((CHAR16 near *)L"SUCCESS:LogFile is closed.\n");
#endif

        return Status;
    }
}