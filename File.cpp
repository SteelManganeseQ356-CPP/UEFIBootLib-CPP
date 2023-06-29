/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#include "File.h"
namespace Boot::File
{
    EFI_STATUS BootService_File::GetFileHandle(IN EFI_HANDLE ImageHandle, IN CHAR16 *FileName, OUT EFI_FILE_PROTOCOL **FileHandle)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        UINTN HandleCount = 0;
        EFI_HANDLE *HandleBuffer;
        Status = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiSimpleFileSystemProtocolGuid,
            NULL,
            &HandleCount,
            &HandleBuffer);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to LocateHanleBuffer of SimpleFileSystemProtocol.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:Get %d handles that supported SimpleFileSystemProtocol.\n", HandleCount);
#endif

        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
        Status = gBS->OpenProtocol(
            HandleBuffer[0],
            &gEfiSimpleFileSystemProtocolGuid,
            (VOID **)&FileSystem,
            ImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to open first handle that supported SimpleFileSystemProtocol.\n");
            return Status;
        }

        Print((CHAR16 *)L"SUCCESS:SimpleFileSystemProtocol is opened with first handle.\n");
#endif
        EFI_FILE_PROTOCOL *Root;
        Status = FileSystem->OpenVolume(
            FileSystem,
            &Root);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to open volume.\n");
            return Status;
        }

        Print((CHAR16 *)L"SUCCESS:Volume is opened.\n");
#endif
        Status = Root->Open(
            Root,
            FileHandle,
            FileName,
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to open file.\n");
            return Status;
        }

        Print((CHAR16 *)L"SUCCESS:File is opened.\n");
#endif

        return Status;
    }
    EFI_STATUS BootService_File::ReadFile(IN EFI_FILE_PROTOCOL *File, OUT EFI_PHYSICAL_ADDRESS *FileBase)
    {
        EFI_STATUS Status = EFI_SUCCESS;
        EFI_FILE_INFO *FileInfo;

        UINTN InfoSize = sizeof(EFI_FILE_INFO) + 128;
        Status = gBS->AllocatePool(
            EfiLoaderData,
            InfoSize,
            (VOID **)&FileInfo);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to AllocatePool for FileInfo.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:Memory for FileInfo is ready.\n");
#endif

        Status = File->GetInfo(
            File,
            &gEfiFileInfoGuid,
            &InfoSize,
            FileInfo);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to GetInfo of Bmp.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:FileInfo is getted.\n");
#endif

        UINTN FilePageSize = (FileInfo->FileSize >> 12) + 1;

        EFI_PHYSICAL_ADDRESS FileBufferAddress;
        Status = gBS->AllocatePages(
            AllocateAnyPages,
            EfiLoaderData,
            FilePageSize,
            &FileBufferAddress);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to AllocatePages for File.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:Memory for File is ready.\n");
#endif

        UINTN ReadSize = FileInfo->FileSize;
        Status = File->Read(
            File,
            &ReadSize,
            (VOID *)FileBufferAddress);
#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to Read File.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:File is readed,size=%d.\n", ReadSize);
#endif
        KernelSize = ReadSize;
        gBS->FreePool(FileInfo);
        *FileBase = FileBufferAddress;
        return Status;
    }
}