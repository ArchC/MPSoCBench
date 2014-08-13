CC = $(CROSS)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) -specs=archc
endif
LDFLAGS = -lm

TARGET = $(SOFTWARE).$(PROCESSOR)

INC_DIR := -I.

SRCS := multisoftware_network_automotive.c ../acPthread.c basicmath_s/basicmath.c basicmath_s/rad2deg.c basicmath_s/cubic.c basicmath_s/isqrt.c  qsort_s/qsort_small.c susancorners_s/susancorners.c  dijkstra_s/dijkstra_static.c 


#bitcount_s/bitcnt_1.c bitcount_s/bitcnt_2.c bitcount_s/bitcnt_3.c bitcount_s/bitcnts.c  bitcount_s/bitstrng.c bitcount_s/bstr_i.c

#susanedges_s/susanedges.c

OBJSAUX :=  multisoftware_network_automotive.o acPthread.o basicmath.o rad2deg.o cubic.o isqrt.o qsort_small.o  susancorners.o  dijkstra_static.o

#susanedges.o
#bitcnt_1.o bitcnt_2.o bitcnt_3.o bitcnts.o bitstrng.o bstr_i.o


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
	rm -f *.x *.o *~ ../acPthread.o


#------------------------------------------------------
distclean: clean
#------------------------------------------------------
bin:
	
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
copy:
	echo ---copy $(TARGET).x to the appropriate rundir
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)
	cp susancorners_s/input_large2.pgm ../../rundir/$(RUNDIRNAME)
	cp dijkstra_s/input.dat ../../rundir/$(RUNDIRNAME)
	cp qsort_s/input_small.dat ../../rundir/$(RUNDIRNAME)
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
	cp basicmath_s/output_basicmath.base ../../rundir/$(RUNDIRNAME)
	cp susancorners_s/output_susancorners.base ../../rundir/$(RUNDIRNAME)
	cp dijkstra_s/output_dijkstra.base ../../rundir/$(RUNDIRNAME)
	cp qsort_s/output_qsort.base ../../rundir/$(RUNDIRNAME)

check:
	echo "\n\nTESTING RESULTS"
	if diff output_basicmath output_basicmath.base > diff.txt ; then echo "\n Basicmath Passed."; else echo "\n Basicmath Failed."; fi
	if diff output_susancorners output_susancorners.base > diff.txt ; then echo "\n Susancorners Passed."; else echo "\n Susancorners Failed."; fi
	if diff output_qsort output_qsort.base > diff.txt ; then echo "\n Qsort Passed."; else echo "\n Qsort Failed."; fi
	if diff output_dijkstra output_dijkstra.base > diff.txt ; then echo "\n Dijkstra Passed."; else echo "\n Dijkstra Failed."; fi
