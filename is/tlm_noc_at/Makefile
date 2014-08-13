TARGET=tlm_noc_at
INC_DIR := -I. -I$(ARCHC_PATH)/include/archc -I$(SYSTEMC)/include -I$(TLM_PATH) 

SRCS := tlm_noc.cpp tlm_node.cpp routing_table.cpp wrappers_noc.cpp

OBJS := $(SRCS:.cpp=.o)

$(CFLAGS) :=  $(MEM_SIZE_DEFAULT)

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
