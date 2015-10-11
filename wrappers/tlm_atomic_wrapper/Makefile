TARGET=tlm_atomic_wrapper
INC_DIR := -I. `pkg-config --cflags systemc` `pkg-config --cflags archc` 

SRCS := tlm_atomic_wrapper.cpp
OBJS := $(SRCS:.cpp=.o)

CFLAGS:= $(WAIT_TRANSPORT_FLAG)

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
