# ####################################################
# SPARC Simulator
# ####################################################

TARGET=sparc
INC_DIR := -I. -I$(ARCHC_PATH)/include/archc -I$(SYSTEMC)/include -I$(TLM_PATH) -I$(POWERSC_PATH)/include -I$(POWERSC_PATH)/src

SRCS := sparc.cpp  sparc_arch.cpp  sparc_arch_ref.cpp  sparc_isa.cpp sparc_syscall.cpp
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
all: 	sparc.cpp $(OBJS)
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
sparc.cpp:
	echo --- No simulator found, using acsim to generate one.
	$(ARCHC_PATH)/bin/acsim sparc_$(TRANSPORT)$(CACHE).ac -abi -pw
	$(MAKE) all
