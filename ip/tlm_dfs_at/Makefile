# ####################################################
# TLM memory with TLM interface (ArchC 2x compliant)
# ####################################################

TARGET=tlm_dfs_at


SRCS := tlm_dfs.cpp
OBJS := $(SRCS:.cpp=.o)

LOCALCFLAGS := $(CFLAGS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG) $(ENDIANESS) $(CFLAGS_AUX) -Wno-deprecated
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
