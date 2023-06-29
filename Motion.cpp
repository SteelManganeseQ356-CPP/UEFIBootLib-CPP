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
namespace Boot::Graphics
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Grey = {166, 166, 166, 0};

    EFI_STATUS BootService_Graphics::GraphicsConfigInit(IN EFI_HANDLE ImageHandle, OUT GraphicsConfig *graphicsConfig)
    {

        EFI_STATUS Status = EFI_SUCCESS;

        Status = this->GetGopHandle(ImageHandle, &Gop);

        Status = this->SetVideoMode(Gop);

        graphicsConfig->FrameBufferBase = Gop->Mode->FrameBufferBase;
        graphicsConfig->FrameBufferSize = Gop->Mode->FrameBufferSize;
        graphicsConfig->HorizontalResolution = Gop->Mode->Info->HorizontalResolution;
        graphicsConfig->VerticalResolution = Gop->Mode->Info->VerticalResolution;
        graphicsConfig->PixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;
        DrawStep(::Step++);
        return Status;
    }

    EFI_STATUS BootService_Graphics::DrawLogo(IN EFI_HANDLE ImageHandle, IN CONST wchar_t *LogoFileName)
    {

        EFI_STATUS Status = EFI_SUCCESS;

        CHAR16 *FileName = (CHAR16 *)LogoFileName;
        UINTN Hor = Gop->Mode->Info->HorizontalResolution;
        UINTN Ver = Gop->Mode->Info->VerticalResolution;
        if (Hor * 3 == Ver * 4)
        {
            FileName = (CHAR16 *)LogoFileName;
        }
        EFI_FILE_PROTOCOL *Logo;
        Status = this->GetFileHandle(ImageHandle, FileName, &Logo);

        EFI_PHYSICAL_ADDRESS LogoAddress;
        Status = this->ReadFile(Logo, &LogoAddress);

        Status = this->BmpTransform(LogoAddress);

        UINTN X = (Hor - this->BmpData_.Width) / 2;
        UINTN Y = (Ver - this->BmpData_.Height) / 2;

        Status = this->DrawBmp(Gop, X, Y);

        DrawStep(::Step++);

        return Status;
    }
    EFI_STATUS DrawStep(IN UINTN Step) // 绘制进度条
    {
        EFI_STATUS Status = EFI_SUCCESS;

        // 把分辨率除以16

        UINTN BlockWidth = Gop->Mode->Info->HorizontalResolution >> 6;
        UINTN BlockHeight = Gop->Mode->Info->VerticalResolution >> 6;
        UINTN StartX = (Gop->Mode->Info->HorizontalResolution - (BlockWidth + GAP) * 10 - GAP) / 2;
        UINTN StartY = (Gop->Mode->Info->VerticalResolution * 3) >> 2;

        UINTN X = StartX + (BlockWidth + GAP) * Step;

        Status = Gop->Blt(Gop, &Grey, EfiBltVideoFill, 0, 0, X, StartY, BlockWidth, BlockHeight, 0);

#ifdef DEBUG
        if (EFI_ERROR(Status))
        {
            Print((CHAR16 *)L"ERROR:Failed to Blt Block.\n");
            return Status;
        }
        Print((CHAR16 *)L"SUCCESS:DrawStep:%d.\n", Step);
#endif

        return Status;
    }
}