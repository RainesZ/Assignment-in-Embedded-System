SET PATH=E:\Keil\ARM\ARMCC\Bin;E:\PythonInstall\Scripts\;E:\PythonInstall\;E:\Xshell\;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;E:\LenovoDriver\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;E:\LenovoDriver\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;E:\lua;H:\Matlab\runtime\win64;H:\Matlab\bin;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Windows\System32\OpenSSH\;E:\Git\cmd;E:\node\;E:\mingw64\bin;E:\yosys-win32-mxebin-0.9;E:\graphviz-2.38\release\bin;E:\PythonInstall\Scripts\;E:\PythonInstall\;E:\Microcontrollers\jlnk_bin_ÉÕÂ¼\sam-ba_2.14\drv\;E:\Microcontrollers\jlnk_bin_ÉÕÂ¼\sam-ba_2.14;C:\Users\HITzyc\AppData\Local\Microsoft\WindowsApps;E:\BandiZip\;E:\wget-1.20.3-win64;E:\WebStorm 2019.3.2\bin;E:\Fiddler;C:\Users\HITzyc\AppData\Roaming\npm;E:\PyCharm 2019.3.3\bin;;E:\mingw64\bin;E:\modelsim\win64;E:\Microsoft VS Code\bin
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=LPC1114FBD48/302
SET CPU_VENDOR=NXP
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x007A1200
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\uart_api.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\gpio_api.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\adc_api.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_lpc11xx._ia"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_lpc11xx.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\s1.lnp"
"E:\Keil\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\s1.axf" --i32combined --output ".\Objects\s1.hex"
