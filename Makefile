#FILE GENERATED AUTOMAGICALLY - DO NOT EDIT
export PROCESSOR := powerpc
export NUMPROCESSORS := 2
export SOFTWARE := dijkstra
export PLATFORM := platform.noc.at

export CROSS := powerpc-newlib-elf-gcc
export POWER_SIM_FLAG := 
export WAIT_TRANSPORT_FLAG := -DWAIT_TRANSPORT
export TRANSPORT := nonblock
export MEM_SIZE_DEFAULT := -DMEM_SIZE=536870912
export RUNDIRNAME := powerpc.noc.at.2.dijkstra
export ENDIANESS := -DAC_GUEST_BIG_ENDIAN
ifeq ($(PROCESSOR),arm)
export CFLAGS_AUX := -DPROCARM
endif
ifeq ($(PROCESSOR),mips)
export CFLAGS_AUX := -DPROCMIPS
endif
ifeq ($(PROCESSOR),sparc)
export CFLAGS_AUX := -DPROCSPARC
endif
ifeq ($(PROCESSOR),powerpc)
export CFLAGS_AUX := -DPROCPOWERPC
endif
include Makefile.conf
include Makefile.rules
