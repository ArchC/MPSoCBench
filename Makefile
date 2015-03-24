#FILE GENERATED AUTOMAGICALLY - DO NOT EDIT
export PROCESSOR := mips
export NUMPROCESSORS := 8
export SOFTWARE := dijkstra
export PLATFORM := platform.noc.at

export CROSS := mips-newlib-elf-gcc
export POWER_SIM_FLAG := -DPOWER_SIM
export WAIT_TRANSPORT_FLAG := -DWAIT_TRANSPORT
export TRANSPORT := nonblock
export MEM_SIZE_DEFAULT := -DMEM_SIZE=536870912
export RUNDIRNAME := mips.noc.at.pw.8.dijkstra
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
