# ----------------------------------------------------------------------------
#
# Copyright (C) 2009 Ingenic Semiconductor Co., Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# Just a invoke entry, and will include each .mak file if needed
#
# v1.0, 	2009-03-21,	Rocky,		Init version
#
# ----------------------------------------------------------------------------

TOP	:= ../..
PROGRAM := loader
VPATH := 
LDSFLAG := link.xn
JZ4740_PMP16 := 1
RELEASE := 1
CAMERA := 0
UARTMC := 0

include $(TOP)/compiler.mak
# ----------------------------------------------------------------------------
#CAMERATYPE :                                  
#           0. no camera
#           1. ov2640           200     
#           2. 0v3640           300
#           3. MI2020           200 
#           4. BF3620           200
#           5. BF3603           30
#           6. ZTPP30-G1        30
#	        7. LFTG02F		    30
#	        8. RXWBG0323	    30
#	        9. ov5640		    30
# ----------------------------------------------------------------------------
CAMERATYPE=9

################################################################
DRVMEMADDR := 0x80832000
DRVMEMLEN := 0x400000

CP_TO_OUT := 1
LOADER_DIR = $(TOP)/em3086/loader
TARGETS := $(LOADER_DIR)/target

OSDIR    :=$(TOP)/src
SOCDIR = $(TOP)/em3086/soc_src
DRVDIR = $(SOCDIR)/drv
MINICLIB := $(TOP)/miniclib

OSLIBS := $(OSDIR)/minios.a
STDCLIBS = -lm -lgcc
MAINLDS := $(LOADER_DIR)/link.xn

CFLAGS += -DJZ4775
CFLAGS += -DDRVMEMADDR=$(DRVMEMADDR) -DDRVMEMLEN=$(DRVMEMLEN)
CFLAGS += -DRELEASE=$(RELEASE) -DJZ4740_PMP16=$(JZ4740_PMP16)
CFLAGS  +=-DDM=1
#CFLAGS  +=-DNAND_DRV_NO_LB

include $(SOCDIR)/x1000.mak

CFLAGS += -I$(OSDIR)
VPATH += $(OSDIR)

CFLAGS += -I$(TOP)/inc

SOURCES	+= $(wildcard $(MINICLIB)/*.c)
CFLAGS  += -I$(MINICLIB)
VPATH   += $(MINICLIB)

HEADS   := $(SOCDIR)/kernel/head.S $(wildcard $(MINICLIB)/*.S)
#VPATH   += $(SOCDIR)/common/kernel_lib

SOURCES	+= $(LOADER_DIR)/loader.c\
                 $(LOADER_DIR)/download.c\
                 $(LOADER_DIR)/uartlr.c
CFLAGS	+= -I$(LOADER_DIR)
VPATH	+= $(LOADER_DIR)

include ../../em3086/common16.mak


