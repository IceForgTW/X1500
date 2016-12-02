# Microsoft Developer Studio Project File - Name="EM3000V5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EM3000V5 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EM3000V5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EM3000V5.mak" CFG="EM3000V5 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EM3000V5 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EM3000V5 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EM3000V5 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EM3000V5 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EM3000V5 - Win32 Release"
# Name "EM3000V5 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "driver"

# PROP Default_Filter ""
# Begin Group "usb"

# PROP Default_Filter ""
# Begin Group "class"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\class\cdc.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\class\datapipe.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\class\hid_sendkey.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\class\kbw.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\class\pos.c"
# End Source File
# End Group
# Begin Group "uart"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\uartmc\uartmc.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\uartmc\uartmc.mk"
# End Source File
# End Group
# Begin Group "flash"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\nor\nor_comm_lb.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\nor\nor_controller.c"
# End Source File
# End Group
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\compat.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\compat.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\dwc-bus.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\dwc-device.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\dwc-x1000.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\dwc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\dwc\dwc_debug.c"
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\misc\generate.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\misc\generate.h"
# End Source File
# End Group
# Begin Group "watchdog"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\watchdog\watchdog.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\watchdog\watchdog.h"
# End Source File
# End Group
# Begin Group "cmos"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\camera\cmos.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\camera\cmos.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\camera\MT9V022.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\camera\mt9v022.h"
# End Source File
# End Group
# Begin Group "i2c"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\i2c\i2c.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\i2c\i2c.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\drv\i2c\i2c.mk"
# End Source File
# End Group
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "PlatformCfg"

# PROP Default_Filter ""
# Begin Group "jz_mem_nand_configs"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\ddr.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_H5PS1G63EFR-G7C.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_H5PS1G63EFR-S6C.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_H5PS1G63EFR-Y5C.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_H5PS1G831CFP-S6.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_H5PS1G83EFR.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_H5PS5162FFR-S5C.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_HY5PS1G1631CLFP-Y5.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_V59C1G01168QB-J25.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_V59C1G02168QB-25A.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR2_V59C1G02168QB-3.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\DDR3_H5TQ2G63BFR.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MCP_H8BCS0UN0MCR-46M.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MCP_H8BCS0UN0MCR.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MCP_H8BCS0UN0MCR_4EM.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MCP_H8BES0UQ0MCR.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MDDR_H5MS1G62MFP-J3M.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MDDR_H5MS5122DFR-J3M.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\MDDR_MT46H128M16LF.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\NAND_H27U8G8T2BTR.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\NAND_K9G8G08U0A.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\NAND_K9GAG08U0M.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz_mem_nand_configs\SDRAM_.h"
# End Source File
# End Group
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\jz4775_common.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\PlatformCfg\mensa_cfg.h"
# End Source File
# End Group
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\ddr_timing.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\dm.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\loadcommon.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\mass_storage.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\mmc_api.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\mmc_core.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\mmc_protocol.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\udcbus.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\common\inc\usb.h"
# End Source File
# End Group
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\bsp.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\cacheops.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\dma_channel.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\jz_mips.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\nandcommand.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\types.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000aic.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000bch.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000cim.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000codec.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000cpm.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000ddrc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000efuse.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000emc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000gpio.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000i2c.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000intc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000lcdc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000mcu.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000me.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000misc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000msc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000nemc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000ost.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000otg_dwc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000otp.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000owi.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000pcm.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000pdma.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000rtc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000sadc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000scc.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000ssi.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000tcu.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000tssi.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000tve.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000uart.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\include\x1000wdt.h"
# End Source File
# End Group
# Begin Group "kernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\clock.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\codec_io.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\cpm.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\dm.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\dmaunit.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\DrvMemMgr.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\gpio.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\init.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\intc.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\os_pm.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\pm.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\timer.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\kernel\uart.c"
# End Source File
# End Group
# Begin Group "boot"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\cpu.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\debug.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\jz4775_ddr.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\jz_ddr3_init.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\makefile"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\msc_boot_jz4775.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\mscboot.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\start.S"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\boot\sfcboot\u-boot-nand.lds"
# End Source File
# End Group
# Begin Group "makefile"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\common16.mak"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\compiler.mak"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\em3000_v5.xn"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\makefile"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\soc_src\x1000\x1000.mak"
# End Source File
# End Group
# Begin Group "download"

# PROP Default_Filter ""
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\download\dnld_bsp.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\download\dnld_comm.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\download\sys_dnld.h"
# End Source File
# End Group
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\em3000_cfg.h"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\em3000_main.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\interface.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\power.c"
# End Source File
# Begin Source File

SOURCE="\x1000-ucosII\em3000-v5\main\uImgTest.c"
# End Source File
# End Group
# End Target
# End Project
