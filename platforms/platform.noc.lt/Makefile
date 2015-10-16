ECHO := /bin/echo -e

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

ifeq ($(PROCESSOR),powerpc)
CFLAGS_AUX = -DPROCPOWERPC
endif

LOCALCFLAGS := $(CFLAGS) $(ENDIANESS) $(CFLAGS) $(POWER_SIM_FLAG) $(WAIT_TRANSPORT_FLAG)  $(MEM_SIZE_DEFAULT) $(CFLAGS_AUX)

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)

#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.SILENT:
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.SUFFIXES: .cc .cpp .o .x
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
$(EXE): $(OBJS)
	$(CC) $(LOCALCFLAGS) $(INC_DIR) $(LIB_DIR) -o $(EXE) $(OBJS) $(LIBS)
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
main.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c main.cpp
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
all: $(EXE)
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
run: all
	./$(EXE) $(SOFTWARE).$(PROCESSOR).x $(NUMPROCESSORS)
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
clean:
	rm -f $(OBJS) $(EXE) *~ *.o
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
distclean: clean
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.cpp.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.cc.o:
	$(CC) $(LOCALCFLAGS) $(INC_DIR) -c $<
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
copy:
	$(ECHO) "---copy $(EXE) to the appropriate rundir"
	cp $(EXE) ../../rundir/$(RUNDIRNAME)/
