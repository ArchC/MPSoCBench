CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -DLITTLE_ENDIAN -specs=archc
endif

LDFLAGS = -lm
INC_DIR := -I.
SRCS := acPthread_p.c multisoftware_p.c sha_multi/sha.c sha_multi/sha_driver.c basicmath_multi/parallel_basicmath.c basicmath_multi/rad2deg.c basicmath_multi/cubic.c basicmath_multi/isqrt.c stringsearch_multi/stringsearch.c stringsearch_multi/bmhasrch.c stringsearch_multi/bmhisrch.c stringsearch_multi/bmhsrch.c dijkstra_multi/dijkstra_allpair.c
OBJSAUX :=  acPthread_p.o multisoftware_p.o sha.o sha_driver.o parallel_basicmath.o rad2deg.o cubic.o isqrt.o stringsearch.o bmhasrch.o bmhisrch.o bmhsrch.o dijkstra_allpair.o
OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:
#------------------------------------------------------
.SUFFIXES: .o .c .h
#------------------------------------------------------
all: $(OBJS)
	$(CC) $(OBJSAUX) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ *.x 
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
bin:	
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
copy: 	
	echo ---copy $(TARGET).x to the appropriate rundir
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp stringsearch_multi/SearchString01.txt ../../rundir/$(RUNDIRNAME)/
	cp stringsearch_multi/testpattern5.txt ../../rundir/$(RUNDIRNAME)/
	cp dijkstra_multi/input_small.dat ../../rundir/$(RUNDIRNAME)/
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
	cp sha_multi/output.sha.$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output_sha.base
	cp stringsearch_multi/output.stringsearch.$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output_stringsearch.base
	cp basicmath_multi/output.basicmath.$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output_basicmath.base
	cp dijkstra_multi/output.dijkstra.$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output_dijkstra.base
check:
	echo "\n\nTESTING RESULTS"
	sort output_sha > output_sha.sorted
	sort output_stringsearch > output_stringsearch.sorted
	sort output_basicmath > output_basicmath.sorted
	sort output_dijkstra > output_dijkstra.sorted
	if diff output_sha.sorted output_sha.base > diff_sha.txt ; then echo "\n SHA Passed."; else echo "\nTest SHA Failed."; fi
	if diff output_stringsearch.sorted output_stringsearch.base > diff_strinsearch.txt ; then echo "\n Stringsearch Passed."; else echo "\n Stringsearch Failed."; fi
	if diff output_basicmath.sorted output_basicmath.base > diff_basicmath.txt ; then echo "\n Basicmath Passed."; else echo "\n Basicmath Failed."; fi
	if diff output_dijkstra.sorted output_dijkstra.base > diff_dijkstra.txt ; then echo "\n Dijkstra Passed."; else echo "\n Dijkstra Failed."; fi

