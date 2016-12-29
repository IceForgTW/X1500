SOURCEOBJS := $(addsuffix .o ,$(basename $(notdir $(SOURCES))))
TARGEOBJS := $(SOURCEOBJS)
HEADO	:= $(addsuffix .o ,$(basename $(notdir $(HEADS))))
HEADTAGET = $(addprefix $(TARGETS)/,$(notdir $(HEADO)))
OBJSTAGET = $(addprefix $(TARGETS)/,$(notdir $(TARGEOBJS)))
APP	:= $(TARGETS)/$(PROGRAM).elf
VPATH += $(TARGETS) $(VPATH)

APPDEP = $(HEADO) $(TARGEOBJS)  $(LDSFLAG) $(MAINLDS) makefile  $(LIBS) $(OSLIBS) $(UIMG4LIBS)

all:	$(TARGETS) $(APP)
	$(OBJCOPY) -O binary $(APP) $(TARGETS)/$(PROGRAM).bin
	mkdir -p $(TOP)/em3086/out 
	cp $(TARGETS)/$(PROGRAM).bin $(TOP)/em3086/out


$(APP):	$(APPDEP)
	$(LD) -r -o $@.obj $(HEADTAGET) $(OBJSTAGET) $(LIBS) $(OSLIBS) $(UIMG4LIBS)
	$(CC) --static -nostdlib -T $(LDSFLAG) -o $@ $@.obj $(STDCLIBS) $(LDFLAGS) 

.c.o:
	$(CC) $(CFLAGS) -o $(TARGETS)/$@ -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -fno-rtti -o $(TARGETS)/$@ -c $<
	
.S.o:
	$(CC) $(CFLAGS) -D_ASSEMBLER_ -D__ASSEMBLY__ -o  $(TARGETS)/$@ -c $<	

dump: $(TARGETS) $(APP)
	$(OBJCOPY) -O binary $(APP) $(TARGETS)/$(PROGRAM).bin
	$(OBJDUMP) -d $(APP) > $(TARGETS)/$(PROGRAM).dump
	$(NM) $(APP) | sort > $(TARGETS)/$(PROGRAM).sym 
	$(OBJDUMP) -h $(APP) > $(TARGETS)/$(PROGRAM).map

clean: $(SUBCLEAN)
	rm -fr $(TARGETS) mobile_tv.lds
ifeq ($(CP_TO_OUT),1)
	rm -rf $(TOP)/em3086/out/$(PROGRAM).bin
endif

$(TARGETS): 
	-@test -d $(TARGETS) || mkdir $(TARGETS)
