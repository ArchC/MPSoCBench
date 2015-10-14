TARGET=tlm_router

SRCS := tlm_router.cpp 
OBJS := $(SRCS:.cpp=.o)



LOCALCFLAGS:= $(CFLAGS) $(POWER_SIM_FLAG) $(ENDIANESS) $(CFLAGS_AUX) $(WAIT_TRANSPORT_FLAG) -Wno-deprecated
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
