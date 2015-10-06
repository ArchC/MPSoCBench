CXXFLAGS=-DNO_STORAGE -Wall -DDEBUG_BUILD 
OPTFLAGS=-O3 

ifdef DEBUG
ifeq ($(DEBUG), 1)
OPTFLAGS= -O0 -g
endif
endif
CXXFLAGS+=$(OPTFLAGS)

# ####################################################
# TLM memory with TLM interface (ArchC 2x compliant)
# ####################################################

TARGET=tlm_memory
INC_DIR :=  -I. -I$(ARCHC_PATH)/include -I$(SYSTEMC)/include -I$(TLM_PATH) 

SRCS := tlm_memory.cpp $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o) $(addsuffix .o, $(basename $(SRC)))

LOCALCFLAGS := $(CXXFLAGS) $(CFLAGS) $(ENDIANESS) $(WAIT_TRANSPORT_FLAG) $(MEM_SIZE_DEFAULT) 

#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o .c
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
	#echo Successfully compiled $<
#------------------------------------------------------
.cc.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<

.c.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
