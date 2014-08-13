include defs.arp

TARGET=$(PLATFORM)
EXE = $(TARGET).x

ifeq ($(PROCESSOR),arm)
CFLAGS_AUX = -DPROCARM
endif

ifeq ($(PROCESSOR),mips)
CFLAGS_AUX = -DPROCMIPS
endif

ifeq ($(PROCESSOR),sparc)
CFLAGS_AUX = -DPROCSPARC
endif

ifeq ($(PROCESSOR),sparc_cache)
CFLAGS_AUX = -DPROCSPARC
endif

ifeq ($(PROCESSOR),powerpc)
CFLAGS_AUX = -DPROCPOWERPC
endif

CFLAGS := $(CFLAGS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG) $(MEM_SIZE_DEFAULT) $(CFLAGS_AUX)

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)

INC_DIR := $(INC_DIR) -I$(POWERSC_PATH)/include -I$(POWERSC_PATH)/src 
LIB_DIR := $(LIB_DIR) -L$(POWERSC_PATH)/lib      



#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .cc .cpp .o .x
#------------------------------------------------------
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(INC_DIR) $(LIB_DIR) -o $(EXE) $(OBJS) $(LIBS)
#------------------------------------------------------
main.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c main.cpp
#------------------------------------------------------
all: $(EXE)
#------------------------------------------------------
run: all
	./$(EXE) $(SOFTWARE).$(PROCESSOR).x $(NUMPROCESSORS)        
#------------------------------------------------------
clean:
	rm -f $(OBJS) $(EXE) *~ *.o
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<
#------------------------------------------------------
.cc.o:
	$(CC) $(CFLAGS) $(INC_DIR) -c $<

copy:
	echo ---copy $(EXE) to the appropriate rundir 
	cp $(EXE) ../../rundir/$(RUNDIRNAME)/
	cp ../../Makefile.conf ../../rundir/$(RUNDIRNAME)/
