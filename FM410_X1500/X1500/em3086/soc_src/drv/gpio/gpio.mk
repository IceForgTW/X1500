SOURCES	+= $(wildcard $(GPIODIR)/*.c)
CFLAGS	+= -I$(GPIODIR)
VPATH   += $(GPIODIR)
