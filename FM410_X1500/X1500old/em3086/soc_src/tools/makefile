# ----------------------------------------------------------------------------
#
# Copyright (C) 2013 Ingenic Semiconductor Co., Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# ----------------------------------------------------------------------------
TOP	:= ../..
SOCDIR	:= $(TOP)/soc_src
HOST_CC		:= gcc
HOST_STRIP	:= strip
HOST_CFLAGS	:= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -I$(SOCDIR)/include $(CFLAGS)

BIN_FILES = gen_ddr_timing

all: $(BIN_FILES)

gen_ddr_timing:  ddr_timing/gen_ddr_timing.c ddr_timing/dwc_ddr_publ.h
	$(HOST_CC) -D_GNU_SOURCE -Wall -lm $(HOST_CFLAGS) -o $@ $<
	@$(HOST_STRIP) $@
	@./gen_ddr_timing
	cp ./ddr_timing.h ../include/common/ddr_timing.h 
	
clean:
	rm -fr *.o */*.o				\
	gen_ddr_timing ../include/common/ddr_timing.h