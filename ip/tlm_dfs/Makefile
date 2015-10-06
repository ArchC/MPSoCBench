# ####################################################
# TLM memory with TLM interface (ArchC 2x compliant)
# ####################################################

TARGET=tlm_dfs
INC_DIR := -I. -I$(ARCHC_PATH)/include -I$(SYSTEMC)/include -I$(TLM_PATH) 

SRCS := tlm_dfs.cpp
OBJS := $(SRCS:.cpp=.o)

LOCALCFLAGS := $(CFLAGS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG) $(CFLAGS_AUX) $(ENDIANESS)   -Wno-deprecated 
#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o
#------------------------------------------------------
lib: all
	ar r lib$(TARGET).a $(OBJS)
#------------------------------------------------------
all: $(OBJS)
#------------------------------------------------------
clean:
	rm -f $(OBJS) *~ *.o *.a
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.cpp.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c -g $<
#------------------------------------------------------
.cc.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c -g $<