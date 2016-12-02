#SOURCES	+= $(wildcard $(CAMERADIR)/*.c)

#SOURCES	+= $(CAMERADIR)/camera.c \

SOURCES 	+= 	$(CAMERADIR)/cmos.c \
	$(CAMERADIR)/gc0308.c
	
#SOURCES	+= $(CAMERADIR)/test/cameratest.c \
#	$(CAMERADIR)/test/preview.c 
#	$(CAMERADIR)/test/bmp.c

#CFLAGS += -I$(LCDDIR)

CFLAGS += -I$(CAMERADIR) -DCAMERATYPE=$(CAMERATYPE)
VPATH  += $(CAMERADIR)

CFLAGS += -I$(CAMERADIR)/test/
VPATH  += $(CAMERADIR)/test/
