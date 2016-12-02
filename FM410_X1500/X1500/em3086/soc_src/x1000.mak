
#echo "======================================================jz4775.mk"
################################################################
# 0 8bit nand
# 1 16bit nand
NANDBITMODE = 0
################################################################

KERNELDIR = $(SOCDIR)/kernel

#CFLAGS += -DTF_SD_CARD=1

SOURCES += $(wildcard $(KERNELDIR)/*.c) $(KERNELDIR)/sleep.s)  $(KERNELDIR)/regs_save_restore.S)
CFLAGS += -I$(KERNELDIR) -I$(SOCDIR)/include/common -I$(SOCDIR)/include  -I$(TOP)/os_src -I$(TOP)/os_src/mips 
VPATH += $(KERNELDIR)

#LIBS += $(SOCDIR)/LIBS/kernel_lib.a
#VPATH += $(SOCDIR)/LIBS


RTCDIR = $(DRVDIR)/rtc
include  $(RTCDIR)/rtc.mk

NORDIR = $(DRVDIR)/nor
include  $(NORDIR)/nor.mk



ifeq ($(CAMERA),1)
DWCDIR = $(DRVDIR)/dwc
include $(DWCDIR)/dwc.mk


I2CDIR = $(DRVDIR)/i2c
include $(I2CDIR)/i2c.mk

CAMERADIR = $(DRVDIR)/camera
include $(CAMERADIR)/camera.mk

WDDIR = $(DRVDIR)/watchdog
include $(WDDIR)/watchdog.mk

EFUSEDIR = $(DRVDIR)/efuse
include $(EFUSEDIR)/efuse.mk
endif

ifeq ($(UARTMC), 1)
UARTMCDIR = $(DRVDIR)/uartmc
include $(UARTMCDIR)/uartmc.mk
endif

MISCDIR = $(DRVDIR)/misc
include $(MISCDIR)/generate.mk

