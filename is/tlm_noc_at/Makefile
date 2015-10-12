TARGET=tlm_noc_at

SRCS := tlm_noc.cpp tlm_node.cpp routing_table.cpp wrappers_noc.cpp local_dfs.cpp

OBJS := $(SRCS:.cpp=.o)

LOCALCFLAGS := $(CFLAGS) $(POWER_SIM_FLAG) $(ENDIANESS) $(CFLAGS_AUX) $(MEM_SIZE_DEFAULT) $(WAIT_TRANSPORT_FLAG) 
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
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
copy:
	cp bounds.txt ../../rundir/$(RUNDIRNAME)/
