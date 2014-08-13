# ####################################################
# MIPS Simulator
# ####################################################

TARGET=mips
INC_DIR := -I. -I$(ARCHC_PATH)/include/archc -I$(SYSTEMC)/include -I$(TLM_PATH) -I$(POWERSC_PATH)/include -I$(POWERSC_PATH)/src

SRCS := mips.cpp  mips_arch.cpp  mips_arch_ref.cpp  mips_isa.cpp mips_syscall.cpp
OBJS := $(SRCS:.cpp=.o)

CFLAGS := $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG)

#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o
#-----------------------------------------------------
lib: all
	ar r lib$(TARGET).a $(OBJS)
#------------------------------------------------------
all: mips.cpp $(OBJS)
#------------------------------------------------------
clean:
	rm -f $(OBJS) *~ *.o *.a
#------------------------------------------------------
distclean: clean
	if test -f Makefile.archc; then $(MAKE) -f Makefile.archc distclean; fi;
#------------------------------------------------------
.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
mips.cpp:
	echo --- No simulator found, using acsim to generate one.
	$(ARCHC_PATH)/bin/acsim mips_$(TRANSPORT).ac -abi
	patch mips.cpp mips.cpp.patch
	patch mips.cpp mips.cpp.acsynth.patch
	patch mips.H mips.H.patch
	patch mips.H mips.H.acsynth.patch
	$(MAKE) all
