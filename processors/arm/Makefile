# ####################################################
# arm Simulator
# ####################################################

TARGET=arm
INC_DIR := -I. -I$(ARCHC_PATH)/include -I$(SYSTEMC)/include -I$(TLM_PATH)

SRCS := arm.cpp  arm_arch.cpp  arm_arch_ref.cpp  arm_isa.cpp arm_syscall.cpp arm_intr_handlers.cpp
OBJS := $(SRCS:.cpp=.o)

LOCALCFLAGS := $(CFLAGS) -g $(ENDIANESS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG)

#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o
#-----------------------------------------------------
lib: all
	ar r lib$(TARGET).a $(OBJS)
#------------------------------------------------------
all: arm.cpp $(OBJS)
#------------------------------------------------------
clean:
	rm -f $(OBJS) *~ *.o *.a
#------------------------------------------------------
distclean: clean
	if test -f Makefile.archc; then $(MAKE) -f Makefile.archc distclean; fi;
	rm -f $(OBJS) *~ $(EXE) core *.o 
#------------------------------------------------------
.cpp.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
arm.cpp:
	echo --- No simulator found, using acsim to generate one.
	$(ARCHC_PATH)/bin/acsim arm_$(TRANSPORT).ac -abi -ndc 
	$(MAKE) all