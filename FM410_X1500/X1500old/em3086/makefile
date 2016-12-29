# ----------------------------------------------------------------------------
# 
# Copyright (C) 2016 Newland Auto-ID Tech Co., Ltd.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
# 
# Just a invoke entry, and will include each .mak file if needed
#
# Version,	Date, 		Engineer,	Mark
# v1.0, 	2016-03-23,	xujian		Init version
#
# ----------------------------------------------------------------------------

RELEASE := 1

# ----------------------------------------------------------------------------
# Chip Ship:
#	0. normal
#	1. With uart across debugging
# ----------------------------------------------------------------------------
SOC_ACROSS_DEBUG := 0
ifeq ($(SOC_ACROSS_DEBUG),1)
CFLAGS += -DSOC_ACROSS_DEBUG=$(SOC_ACROSS_DEBUG)
endif
# ----------------------------------------------------------------------------
# Ingenic SOC Generation
#	* 4760
#	* 4770
# 	* 4775
# ----------------------------------------------------------------------------
SOC_GENERATION := 4775
CFLAGS += -DSOC_GENERATION=$(SOC_GENERATION)

# ----------------------------------------------------------------------------
# The System Configuration, change the settings carefully, Thank you!
# ----------------------------------------------------------------------------
JZ4740_PMP16 := 0
NO_DRIVER := 0
CAMERA := 1
UARTMC := 1

TOP	:= ..
include $(TOP)/compiler.mak

EM3086_DIR := $(TOP)/em3086
PROGRAM := em3086
LIBS    := 
SOURCE :=
VPATH := 

MOBILE_TVMAINDIR := $(EM3086_DIR)/main
TARGETS := $(EM3086_DIR)/target
LDSFLAG := $(EM3086_DIR)/em3000_v5.xn

################################################################
#include $(EM3086_DIR)/drv.mak
################################################################
CAMERATYPE=9
DRVMEMADDR := 0x804f1000
DRVMEMLEN  := 0x00400000
RESERVEMEM := 0xDE0800

OSDIR    :=$(TOP)/src
SOCDIR = $(EM3086_DIR)/soc_src

DRVDIR = $(SOCDIR)/drv
MINICLIB := $(TOP)/miniclib

OSLIBS := $(OSDIR)/minios.a
STDCLIBS = -lm -lgcc
CFLAGS	+= -DEM3000_V5
CFLAGS += -DJZ4775
CFLAGS += -DDRVMEMADDR=$(DRVMEMADDR) -DDRVMEMLEN=$(DRVMEMLEN) -DRELEASE=$(RELEASE)

include $(SOCDIR)/x1000.mak

CFLAGS += -I$(OSDIR)
CFLAGS += -DDM=1

SOURCES	+= $(wildcard $(MINICLIB)/*.c)
CFLAGS  += -I$(MINICLIB)
VPATH   += $(MINICLIB)

DECODECTRLDIR := $(TOP)/MB_Decode_Ctrl

include $(DECODECTRLDIR)/decodectrl.mak
CFLAGS	+= -I$(DECODECTRLDIR)



UIMG4LIBS := $(EM3086_DIR)/decoder/x1000GXImg.a $(EM3086_DIR)/decoder/uIMG4.a

HEADS   := $(SOCDIR)/kernel/head.S $(wildcard $(MINICLIB)/*.S)
#VPATH   += $(SOCDIR)/common/kernel_lib
#========================================================================#

CFLAGS += -DRESERVEMEM=$(RESERVEMEM)
CFLAGS += -DJZ4740_PMP16=$(JZ4740_PMP16)

CFLAGS	+= -I$(EM3086_DIR)
SOURCES	+= $(MOBILE_TVMAINDIR)/em3000_main.c \
			$(MOBILE_TVMAINDIR)/rsa.c \
			$(MOBILE_TVMAINDIR)/download/dnld_bsp.c \
			$(MOBILE_TVMAINDIR)/download/dnld_comm.c \
			$(MOBILE_TVMAINDIR)/power.c \
			$(MOBILE_TVMAINDIR)/interface.c
#			$(MOBILE_TVMAINDIR)/uImgTest.c
			
CFLAGS	+= -I$(MOBILE_TVMAINDIR) -I$(MOBILE_TVMAINDIR)/download
VPATH	+= $(MOBILE_TVMAINDIR) $(MOBILE_TVMAINDIR)/download

CFLAGS += -I$(TOP)/inc
include $(EM3086_DIR)/common16.mak

#KENELLIB = $(SOCDIR)/LIBS/kernel_lib.a
PM_SLEEP = $(SOCDIR)/pm-sleep/core_sleep.hex

.PHONY:  build app build cleanlib cleanall rebuild  all


################ 编译应用程序及其依赖库 ###################################
build_minios: build_pm_sleep $(OSLIBS) $(TARGETS)  all
	@test $(OSLIBS)
	@test $(OSLIBS)
	@test $(PM_SLEEP)
	@test -d $(TARGETS)
	@test $(APP)
	@mkdir -p $(TOP)/em3086/out

################ 编译一级引导程序 #########################################	
build_boot:
	@test -e $(SOCDIR)/boot/sfcboot && $(MAKE) -C $(SOCDIR)/boot/sfcboot || exit

################ 编译二级引导程序 #########################################
build_loader:
	@test -e $(EM3086_DIR)/loader && $(MAKE) -C $(EM3086_DIR)/loader || exit 

################ 编译应用程序的依赖的核心库 ###############################
$(OSLIBS):
	$(MAKE) -C $(TOP)/os_src

################# 编译休眠唤醒核心操作代码  ###############################
build_pm_sleep: 
	@test $(PM_SLEEP) && $(MAKE) -C $(SOCDIR)/pm-sleep || exit 

################ clean编译文件 ############################################
clean_boot:
	@test -e $(SOCDIR)/boot/sfcboot && $(MAKE) -C $(SOCDIR)/boot/sfcboot clean || exit 

clean_loader:
	-@test -e $(EM3086_DIR)/loader && $(MAKE) -C $(EM3086_DIR)/loader clean || echo

cleanlib:
	$(MAKE) clean -C $(TOP)/os_src

clean_pm_sleep:
	@test -e $(SOCDIR)/pm-sleep && $(MAKE) -C $(SOCDIR)/pm-sleep clean || exit 


cleanall: clean cleanlib clean_boot  clean_loader clean_pm_sleep
	@rm -rf $(TOP)/em3086/out
	
############### 编译全部工程 #############################################	
buildall: build_minios build_boot  build_loader
rebuild: cleanall build 

