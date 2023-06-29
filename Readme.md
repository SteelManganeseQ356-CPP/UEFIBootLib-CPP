<<<<<<< HEAD
# 使用我自制的UEFI-BootLibrary
# Use my homemade UEFI-BootLibrary

## 1-下载EDK2编译工具并配置
## 1- Download the EDK2 compilation tool and configure

- 不方便讲，自己搞
- It's not convenient to talk, do it yourself

## 2-主函数内使用
## 2 - Used within the main function

- 定义BootConfig类，如下定义：
- Definition struct BootConfig, as follows definition:

```cpp
  Boot::BootConfig bootConfig{
   {0},
   {0},
   {{4096 * 4},
    {nullptr},
    {0},
    {0},
    {0}},
   {{0},
    {0},
    {(CHAR8 *)"System Name"},
    {10000},
    {0ULL}}};
```

- 定义BootService类，如下定义：
- Definition class BootService, as follows definition:

```cpp
  /* 初始化屏幕和初始化日志系统 */
  Boot::BootService service(ImageHandle, bootConfig);
```

- 使用BootService类，如下
- Use class BootService, as follows:

```cpp
  /* 绘制logo */
  service.DrawLogo(ImageHandle, L"Your Logo");
  /* 读取内核 */
  service.Relocate(ImageHandle, L"Your Kernel");
  /* 获取MemoryMap */
  service.GetMemMap();
  /* 跳转内核 */
  service.JumpToKernel(ImageHandle);
```

- 示例

```cpp
#include "../Include/Boot/Boot.hpp"
extern "C"
{
 EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
 {
  EFI_STATUS Status = EFI_SUCCESS;

  QuantumNEC::Boot::BootConfig bootConfig{
   {0},
   {0},
   {{4096 * 4},
    {nullptr},
    {0},
    {0},
    {0}},
   {{0},
    {0},
    {(CHAR8 *)"QuantumNEC"},
    {10000},
    {0ULL}}};
  /* 初始化屏幕和初始化日志系统 */
  QuantumNEC::Boot::BootService service(ImageHandle, bootConfig);
  /* 绘制logo */
  Status = service.DrawLogo(ImageHandle, L"EFI\\Boot\\Logo.bmp");
  /* 读取内核 */
  Status = service.Relocate(ImageHandle, L"QuantumNEC\\ELFKernel\\Kernel.elf");
  /* 获取MemoryMap */
  Status = service.GetMemMap();
  /* 跳转内核 */
  Status = service.JumpToKernel(ImageHandle);

  return Status;
 }
}
```

## 版权
## Copyright

- 如有盗版，全家死🐎
- If there is piracy, the whole family dies mother
=======
一个东西，用CPP写的
