# ####################################################
# powerpc Simulator
# ####################################################

TARGET=powerpc
INC_DIR := -I. -I$(ARCHC_PATH)/include/ -I$(SYSTEMC)/include -I$(TLM_PATH)

SRCS := powerpc.cpp  powerpc_arch.cpp  powerpc_arch_ref.cpp  powerpc_isa.cpp powerpc_syscall.cpp
OBJS := $(SRCS:.cpp=.o)

CFLAGS := -g $(ENDIANESS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG)  

#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o
#-----------------------------------------------------
lib: all
	ar r lib$(TARGET).a $(OBJS)
#------------------------------------------------------
all: powerpc.cpp $(OBJS)
#------------------------------------------------------
clean:
	rm -f $(OBJS) *~ *.o *.a
#------------------------------------------------------
distclean: clean
	if test -f Makefile.archc; then $(MAKE) -f Makefile.archc distclean; fi;
	rm -f $(OBJS) *~ $(EXE) core *.o 
#------------------------------------------------------
.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
powerpc.cpp:
	echo --- No simulator found, using acsim to generate one.
	$(ARCHC_PATH)/bin/acsim powerpc_$(TRANSPORT).ac -abi -ndc
	$(MAKE) all
