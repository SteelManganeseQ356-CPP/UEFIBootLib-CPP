/***************************************************************************************************************************************************************************************************************************************************************************
*  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
*  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
*  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
*  You may obtain a copy of Mulan PSL v2 at:																																																							   *
*            http://license.coscl.org.cn/MulanPSL2																																																						   *
*   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
*   See the Mulan PSL v2 for more details.  																																																							   *
***************************************************************************************************************************************************************************************************************************************************************************/
#include "Boot.hpp"
namespace Boot
{
	EFI_STATUS BootService::JumpToKernel(EFI_HANDLE ImageHandle)
	{
		EFI_STATUS Status = EFI_SUCCESS;
		// Data
		BootConfig BootData;
		BootData.BmpData = this->putBmpConfig();
		BootData.GraphicsData = this->putGraphicsConfig();
		BootData.MemoryData = this->putMemoryConfig();
		BootData.KernelData = this->putKernelConfig();
		/* 退出启动时服务并跳转内核 */
		Status = gBS->ExitBootServices(ImageHandle, BootData.MemoryData.MemoryKey);
		Status = reinterpret_cast<EFI_STATUS (*)(BootConfig huge *)>(BootData.KernelData.KernelAddress)(&BootData);
		return Status;
	}
}
