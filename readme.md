cc2541串口bootloader

用途：使用串口来给cc2541烧录程序。

app跟iar文件夹是\BLE-CC254x-1.5.2.0\Projects\ble\util\SBL
原本就有的，我对源码进行了如下修改：

我根据自身的蓝牙模块，添加了个配置，使用port1的串口，即P1.4 P1.5作为串口传输。

HAL_UART_SBL=2
xSBL_CALC_CRC
SBL_UART_PORT=HAL_UART_PORT_1
XOSC32K_INSTALLED=FALSE

改了下bootloader的加载方式：

重置后，led亮，会进入读取串口数据阶段，大概持续3s：

如果收到数据，并且第一个数据是FE或者00或者4D或者04或者49（跟官方的SerialBootTool握手帧一样，官方这个工具有点水），立即进入serial download模式，led快速闪3下；

如果收到其他数据会立即跳出bootloader。

如果超过3s，也会跳出bootloader。

如果没有可以加载的application，也会直接进入serial download模式；

一般烧录流程：芯片reset，看到led亮，3s内用SerialBootTool加载image。

prebuild文件夹是已经编译好的bootloader："SBL_CC254xF256_PORT1_UART.hex"，可以直接用（需要用烧录工具，可以用我的MyProgramer来烧录）;另一个"HostTestReleaseCC2541-SBL.bin"这个可以用官方的"SerialBootTool"来烧录已经烧录bootloader的cc2541芯片，HostTest其实也是官方BTool软件要用到的下位机软件。


由于芯片烧录bootloader，所以启动流程有点不一样，用户的project也要相应修改，详细看“编译可以使用serial bootloader烧录的工程.txt”或者“CC2540_ Serial Bootloading.mhtml”


演示：https://www.bilibili.com/video/BV1xF411m7ZZ