########################################################################
CC	:= mipsel-linux-gcc
CXX	:= mipsel-linux-g++
AR	:= mipsel-linux-ar rcsv
LD	:= mipsel-linux-ld
OBJCOPY	:= mipsel-linux-objcopy
NM	:= mipsel-linux-nm
OBJDUMP	:= mipsel-linux-objdump

CFLAGS	:= -mips32 -O2 -mno-abicalls -fno-pic -fno-builtin -Wall -Wno-unused-function\
	   -fno-exceptions -ffunction-sections \
	   -fomit-frame-pointer -fshort-wchar -G 0
CXXFLAGS := -mips32 -O2 -mno-abicalls -fno-pic -fno-builtin -Wall -Wno-unused-function\
           -ffunction-sections -finit-priority \
	   -fomit-frame-pointer -G 0 -nostdinc++

CFLAGS += -DENABLE_FPU
CXXFLAGS += -DENABLE_FPU
########################################################################
