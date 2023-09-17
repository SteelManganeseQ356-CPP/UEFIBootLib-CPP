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
  QuantumNEC::Boot::BootConfig bootConfig { };
```

- 定义BootService类，如下定义：
- Definition class BootService, as follows definition:

```cpp
  /* 初始化屏幕和初始化日志系统 */
  QuantumNEC::Boot::BootServiceMain bootService { &bootConfig };
```

- 使用BootService类，如下
- Use class BootService, as follows:

```cpp
  /// 读取Logo并显示
  Status = bootService.getBmpConfig( &bootConfig.BmpData ).displayLogo( L"\\EFI\\Boot\\Logo.BMP" );
  // 读取ACPI表
  Status = bootService.getApicTable( );
  // 获取内存布局
  Status = bootService.getMemoryMap( );
  // 读取并装载内核
  Status = bootService.loadKernel( L"\\QuantumNEC\\microKernel.elf" );
  // 设置内核页表
  Status = bootService.setPageTable( );
  // 读取Unicode字体并弹出
  Status = bootService.setUnicodeTTF( );
  // 跳转内核
  Status = bootService.jumpToKernel( &bootConfig );
```

- 示例

```cpp
#include <Boot/Boot.hpp>
#include <Boot/Include.hpp>
auto initializeGlobalData( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) -> VOID {     // 初始化全局变量
    QuantumNEC::Boot::GlobalImageHandle = ImageHandle;
    QuantumNEC::Boot::GlobalSystemTable = SystemTable;
    return;
}
extern "C" EFI_STATUS EFIAPI UefiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) {
    EFI_STATUS Status { EFI_SUCCESS };
    initializeGlobalData( ImageHandle, SystemTable );
    QuantumNEC::Boot::BootConfig bootConfig { };
    QuantumNEC::Boot::BootServiceMain bootService { &bootConfig };
    // 读取Logo并显示
    Status = bootService.getBmpConfig( &bootConfig.BmpData ).displayLogo( L"\\EFI\\Boot\\Logo.BMP" );
    // 读取ACPI表
    Status = bootService.getApicTable( );
    // 获取内存布局
    Status = bootService.getMemoryMap( );
    // 读取并装载内核
    Status = bootService.loadKernel( L"\\QuantumNEC\\microKernel.elf" );
    // 设置内核页表
    Status = bootService.setPageTable( );
    // 读取Unicode字体并弹出
    Status = bootService.setUnicodeTTF( );
    // 跳转内核
    Status = bootService.jumpToKernel( &bootConfig );

    return EFI_SUCCESS;
}

```

## 版权
## Copyright

- 如有盗版，全家死🐎
- If there is piracy, the whole family dies mother
