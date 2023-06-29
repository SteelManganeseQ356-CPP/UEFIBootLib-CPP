/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#include "Graphics.h"

UINT8 Step = 1;

namespace Boot::Graphics
{

    EFI_STATUS BootService_Graphics::GetGopHandle(IN EFI_HANDLE ImageHandle, OUT EFI_GRAPHICS_OUTPUT_PROTOCOL far **Gop)
    {

        EFI_STATUS Status = EFI_SUCCESS;
        UINTN HandleCount = 0;
        EFI_HANDLE *HandleBuffer;
        Status = gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiGraphicsOutputProtocolGuid,
            NULL,
            &HandleCount,
            &HandleBuffer);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to LocateHanleBuffer of GraphicsOutputProtocol.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:Get %d handles that supported GraphicsOutputProtocol.\n", HandleCount);

#endif

        Status = gBS->OpenProtocol(
            HandleBuffer[0],
            &gEfiGraphicsOutputProtocolGuid,
            (VOID **)Gop,
            ImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to open first handle that supported GraphicsOutputProtocol.\n");
            return Status;
        }

        Print((CHAR16 *)L"SUCCESS:GraphicsOutputProtocol is opened with first handle.\n");

#endif

        return Status;
    }

    EFI_STATUS BootService_Graphics::SetVideoMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop)
    {
        EFI_STATUS Status = EFI_SUCCESS;

        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
        UINTN ModeInfoSize = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
        UINTN H = 0;
        UINTN V = 0;
        UINTN ModeIndex = 0;

        for (UINTN i = 0; i < Gop->Mode->MaxMode; i++)
        {
            Status = Gop->QueryMode(Gop, i, &ModeInfoSize, &ModeInfo);
            H = ModeInfo->HorizontalResolution;
            V = ModeInfo->VerticalResolution;
            if (
                ((H == 1024) && (V == 768)) ||
                ((H == 1440) && (V == 900)) ||
                ((H == 1920) && (V == 1080)))
            {
                ModeIndex = i;
            }
        }

        Status = Gop->SetMode(Gop, ModeIndex);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to SetMode.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:SetMode to Index:%d.\n", ModeIndex);

#endif
        DrawStep(::Step++);
        return Status;
    }
    EFI_STATUS BootService_Graphics::BmpTransform(IN EFI_PHYSICAL_ADDRESS BmpBase)
    {
        EFI_STATUS Status = EFI_SUCCESS;
#ifdef DEBUG
        BmpHeader *bheader = (BmpHeader *)BmpBase;
        Print((CHAR16 *)L"File size = %08lx.\n", bheader->file_size);
#endif
        auto conversion = [&](UINTN Offset, UINTN Size) -> UINTN
        {
            UINTN Result = 0;
            for (UINTN i = 0; i < Size; ++i)
            {
                Result += *(reinterpret_cast<UINT8 near *>(BmpBase) + Offset + i) << i * 8;
            }
            return Result;
        };
        this->BmpData_.Size = conversion(2, 4);

        this->BmpData_.PageSize = (this->BmpData_.Size >> 12) + 1;
        this->BmpData_.Offset = conversion(10, 4);

        this->BmpData_.Width = conversion(18, 4);
        this->BmpData_.Height = conversion(22, 4);

        EFI_PHYSICAL_ADDRESS PixelStart;
        Status = gBS->AllocatePages(
            AllocateAnyPages,
            EfiLoaderData,
            this->BmpData_.PageSize,
            &PixelStart);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to AllocatePages for PixelArea.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:Memory for PixelArea is ready.\n");
#endif
        DrawStep(::Step++);
        EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelFromFile =
            (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(BmpBase +
                                              this->BmpData_.Offset +
                                              this->BmpData_.Width *
                                                  this->BmpData_.Height *
                                                  4);
        EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelToBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)PixelStart;
        for (UINTN i = 0; i < this->BmpData_.Height; i++)
        {
            PixelFromFile -= this->BmpData_.Width;
            for (UINTN j = 0; j < this->BmpData_.Width; j++)
            {
                *PixelToBuffer = *PixelFromFile;
                PixelToBuffer++;
                PixelFromFile++;
            }
            PixelFromFile -= this->BmpData_.Width;
        }

        this->BmpData_.PixelStart = PixelStart;

        return Status;
    }

    EFI_STATUS BootService_Graphics::DrawBmp(IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, IN UINTN X, IN UINTN Y)
    {
        EFI_STATUS Status = EFI_SUCCESS;

        Status = Gop->Blt(
            Gop,
            (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)this->BmpData_.PixelStart,
            EfiBltBufferToVideo,
            0, 0,
            X, Y,
            this->BmpData_.Width, this->BmpData_.Height, 0);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to Blt Logo.bmp, code=%d.\n", Status);
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:You should see the Logo.\n");
#endif
        DrawStep(::Step++);
        return Status;
    }
}