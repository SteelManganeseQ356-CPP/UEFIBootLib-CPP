/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include "include.h"
#include "Log.h"
#include "File.h"
#include "Utils.h"
extern UINT8 Step;
#define GAP 1
namespace Boot::Graphics
{
    /**
     * @brief 绘制进度条的方块, 每次一格
     */
    EFI_STATUS DrawStep(IN UINTN Step);

    typedef struct
    {
        EFI_PHYSICAL_ADDRESS FrameBufferBase;
        UINTN FrameBufferSize;
        UINT32 HorizontalResolution;
        UINT32 VerticalResolution;
        UINT32 PixelsPerScanLine;
    } GraphicsConfig;

    typedef struct
    {
        UINTN Size;
        UINTN PageSize;
        UINTN Width;
        UINTN Height;
        UINTN Offset;
        UINT64 PixelStart;
        UINT64 BitsPerPx;
    } __attribute__((packed)) BmpConfig;
    typedef struct
    {
        UINT8 sig[2];
        UINT32 file_size;
        UINT16 reserved1;
        UINT16 reserved2;
        UINT32 header_size;
        UINT32 info_header_size;
        UINT32 width;
        UINT32 height;
        UINT16 plane_num;
        UINT16 color_bit;
        UINT32 compression_type;
        UINT32 compression_size;
        UINT32 horizontal_pixel;
        UINT32 vertical_pixel;
        UINT32 color_num;
        UINT32 essentail_num;
    } BmpHeader;
    class BootService_Graphics : public File::BootService_File,
                                 private Log::BootService_Log
    {
    public:
        BootService_Graphics(GraphicsConfig &_GraphicsData, BmpConfig &_BmpData, EFI_HANDLE ImageHandle, IN CONST wchar_t far* LogFileName)
            : BootService_Log{ImageHandle, LogFileName},
              GraphicsData_{_GraphicsData},
              BmpData_{_BmpData}

        {
            EFI_STATUS Status = EFI_SUCCESS;

            Status = this->GraphicsConfigInit(ImageHandle, &this->GraphicsData_);
            if (EFI_ERROR(Status))
                this->LogError(Status);
            else
                this->LogTip("Init Graphics Success.\n");
            DrawStep(::Step++);
        }
        ~BootService_Graphics(VOID) = default;

    public:
        /**
         * @brief 初始化图像
         * @param ImageHandle 屏幕参数
         * @param Gop 传入需要获取的Gop
         * @return EFI_STATUS
         */
        EFI_STATUS GetGopHandle(IN EFI_HANDLE ImageHandle, EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop);
        /**
         * @brief 设置显示模式
         * @param Gop 需要操作的Gop
         * @return EFI_STATUS
         */
        EFI_STATUS SetVideoMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop);
        /**
         * @brief 翻转图像
         * @param BmpBase
         * @param BmpConfig 位图信息
         * @return EFI_STATUS
         */
        EFI_STATUS BmpTransform(IN EFI_PHYSICAL_ADDRESS BmpBase);
        /**
         * @brief 在特定坐标绘制图像
         * @param Gop 需要操作的Gop
         * @return EFI_STATUS
         */
        EFI_STATUS DrawBmp(IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, IN UINTN X, IN UINTN Y);
        /**
         * @brief 绘制图标
         */
        EFI_STATUS DrawLogo(IN EFI_HANDLE ImageHandle, IN CONST wchar_t *LogoFileName);

    public:
        CONST GraphicsConfig &putGraphicsConfig(VOID) CONST
        {
            return this->GraphicsData_;
        }
        CONST BmpConfig &putBmpConfig(VOID) CONST
        {
            return this->BmpData_;
        }

    private:
        GraphicsConfig GraphicsData_;
        BmpConfig BmpData_;

    private:
        /**
         * @brief 初始化图像
         * @param ImageHandle 屏幕数据
         * @param graphicsConfig 要管理的数据
         */
        EFI_STATUS GraphicsConfigInit(IN EFI_HANDLE ImageHandle, OUT GraphicsConfig *graphicsConfig);
    };

}
#endif /* !_GRAPHICS_H_ */