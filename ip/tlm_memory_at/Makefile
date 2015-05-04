# ####################################################
# TLM memory with TLM interface (ArchC 2x compliant)
# ####################################################

TARGET=tlm_memory_at
INC_DIR :=  -I. -I$(ARCHC_PATH)/include -I$(SYSTEMC)/include -I$(TLM_PATH) 

SRCS := tlm_memory.cpp
OBJS := $(SRCS:.cpp=.o)

CFLAGS := $(ENDIANESS) $(WAIT_TRANSPORT_FLAG) $(MEM_SIZE_DEFAULT)

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
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
