# ----------------------------------------------------------------------------
# 
# Copyright (C) 2008 Ingenic Semiconductor Co., Ltd.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
# 
# Just a invoke entry, and will include each .mak file if needed
#
# Version,	Date, 		Engineer,	Mark
# v1.0, 	2008-10-27,	whxu,		Init version
#
# ----------------------------------------------------------------------------
TOP	:= ../../../..
include $(TOP)/compiler.mak

LIBS	:= -lstdc++ -lc -lm -lgcc

ARCHDIR	:= $(TOP)/src
SOCDIR	:= $(TOP)/em3086/soc_src
NANDBOOT:= $(SOCDIR)/boot/sfcboot
TARGETS := $(NANDBOOT)/tagets

LIBDIR	:= 

SOURCES	:= $(wildcard $(NANDBOOT)/*.c)

HEADS	+= $(NANDBOOT)/start.S 


CFLAGS	+= -I$(SOCDIR)/include -I$(ARCHDIR) -I$(NANDBOOT) 

VPATH	:= $(ARCHDIR) $(NANDBOOT)

OBJS	:= $(addsuffix .o , $(basename $(notdir $(SOURCES))))
HEADO	:= $(addsuffix .o , $(basename $(notdir $(HEADS))))

HEADTAGET = $(addprefix $(TARGETS)/,$(notdir $(HEADO)))
OBJSTAGET = $(addprefix $(TARGETS)/,$(notdir $(OBJS)))

APP	:= $(TARGETS)/boot.elf
TOOLS	:= tools

all:	$(TOOLS) $(TARGETS) $(APP)
	$(OBJCOPY) -O binary $(APP) $(TARGETS)/nand.bin
	$(OBJDUMP) -d $(APP) > $(TARGETS)/boot.dump
	$(NM) $(APP) | sort > $(TARGETS)/boot.sym
	$(OBJDUMP) -h $(APP) > $(TARGETS)/boot.map
	cat $(TARGETS)/nand.bin > $(TARGETS)/boot.bin
	./spi_checksum $(TARGETS)/boot.bin
	@rm $(TARGETS)/nand.bin
	@chmod 775 $(TARGETS)/boot.bin 
	@mkdir -p $(TOP)/em3086/out
	cp $(TARGETS)/boot.bin $(TOP)/em3086/out

$(TOOLS):
	@make -C $(SOCDIR)/tools/ all

$(APP): $(HEADO) $(OBJS) $(EXTLIBS) u-boot-nand.lds makefile
	$(CC) -nostdlib -T u-boot-nand.lds -o $@ $(HEADTAGET) $(OBJSTAGET) $(EXTLIBS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -o $(TARGETS)/$@ -c $<
.cpp.o:
	$(CC) $(CFLAGS) -fno-rtti -fvtable-gc -o $(TARGETS)/$@ -c $<
.S.o:
	$(CC) $(CFLAGS) -D_ASSEMBLER_ -D__ASSEMBLY__ -o $(TARGETS)/$@ -c $<

clean:
	rm -fr $(TARGETS) 
#	rm -fr *.o $(APP) $(OBJS) core $(OTHER) *.sym *.map *.dump  *.bin *.lib
	@make -C $(SOCDIR)/tools/ clean
	rm -rf $(TOP)/em3086/out/boot.bin
	
	
$(TARGETS): 
	-@test -d $(TARGETS) || mkdir $(TARGETS)
