#FILE GENERATED AUTOMAGICALLY - DO NOT EDIT
export PROCESSOR := arm
export NUMPROCESSORS := 2
export SOFTWARE := sha
export PLATFORM := platform.noc.at

export CROSS := arm-newlib-eabi-gcc
export POWER_SIM_FLAG := 
export WAIT_TRANSPORT_FLAG := -DWAIT_TRANSPORT
export TRANSPORT := nonblock
export MEM_SIZE_DEFAULT := -DMEM_SIZE=536870912
export RUNDIRNAME := arm.noc.at.2.sha
export ENDIANESS := 
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
