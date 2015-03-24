# ####################################################
# Water for dual processor platform
# ####################################################

CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc
endif
LDFLAGS = -lm

INC_DIR := -I.

OBJS := barrier.o bndry.o cnstnt.o cshift.o initia.o interf.o intraf.o kineti.o mdmain.o poteng.o predcor.o syscons.o water.o
MACROS := c.m4.null
M4 := m4 -s -Ulen -Uindex

x = *
#------------------------------------------------------
(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)

clean:
	rm -rf *.c *.h *.o *.x 

distclean: clean

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

bin:
	

.H.h:
	$(M4) $(MACROS) $*.H > $*.h

.C.c:
	$(M4) $(MACROS) $*.C > $*.c

.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $< 
	
.C.o:
	$(M4) $(MACROS) $*.C > $*.c
	$(CC) -c $(CFLAGS) $*.c

bndry.o: bndry.C split.h mdvar.h parameters.h mddata.h global.h
cnstnt.o: cnstnt.C water.h wwpot.h cnst.h frcnst.h fileio.h parameters.h global.h
cshift.o: cshift.C water.h global.h
initia.o: initia.C split.h mdvar.h water.h cnst.h fileio.h parameters.h mddata.h global.h
interf.o: interf.C mdvar.h water.h wwpot.h cnst.h global.h split.h parameters.h mddata.h
intraf.o: intraf.C frcnst.h mdvar.h water.h wwpot.h parameters.h mddata.h global.h split.h
kineti.o: kineti.C global.h split.h mdvar.h parameters.h mddata.h
mdmain.o: mdmain.C  parameters.h mdvar.h water.h wwpot.h cnst.h mddata.h fileio.h frcnst.h randno.h global.h split.h
poteng.o: poteng.C global.h split.h mdvar.h frcnst.h water.h wwpot.h parameters.h mddata.h
predcor.o: predcor.C split.h mdvar.h parameters.h mddata.h global.h
syscons.o: syscons.C parameters.h mdvar.h water.h wwpot.h cnst.h mddata.h global.h
water.o: water.C parameters.h mdvar.h water.h wwpot.h cnst.h mddata.h fileio.h frcnst.h randno.h global.h split.h


copy:
	echo ---copy $(TARGET).x to the appropriate rundir	
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
check:
	echo "\n\nTESTING RESULTS"
	sort output > output.sorted 
	if diff output.sorted output.base > diff.txt ; then echo "\nTest Passed."; else echo "\nTest Failed."; fi
