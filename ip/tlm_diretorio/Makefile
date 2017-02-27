TARGET=tlm_diretorio

SRCS := tlm_diretorio.cpp Diretorio.cpp cacheMem.cpp cacheBlock.cpp
OBJS := $(SRCS:.cpp=.o)

LOCALCFLAGS := $(CFLAGS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG) $(CFLAGS_AUX) $(ENDIANESS) -Wno-deprecated
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
