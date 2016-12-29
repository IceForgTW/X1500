
TOP     := ..
include $(TOP)/compiler.mak

LIBDIR = $(TOP)/src
OSDIR = $(TOP)/os_src
MIPSDIR = $(OSDIR)/mips
LIBNAME := minios.a

SOURCES = jzos.c os_cpu_c.c os_dbg.c os_cpu_a.S

CFLAGS += -I$(MIPSDIR) -I$(OSDIR) -I$(LIBDIR)
VPATH = $(MIPSDIR) $(OSDIR)
OBJS	:= $(addsuffix .o , $(basename $(notdir $(SOURCES))))

.SUFFIXES: .c .o .cpp

all: $(OBJS)
	$(AR) $(LIBDIR)/$(LIBNAME) $(OBJS)
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -fno-rtti -fvtable-gc -o $@ -c $<
.S.o:
	$(CC) $(CFLAGS) -D_ASSEMBLER_ -D__ASSEMBLY__ -o $@ -c $<
clean:
	rm -f  *~ *.o $(LIBDIR)/$(LIBNAME) depend

dep:    depend
depend:
	$(CC) -MM $(CFLAGS) $(SOURCES) 1>.depend

filelist:
	$(OBJS) $(SOURCES)