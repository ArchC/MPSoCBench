CC = $(CROSS)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) -specs=archc
endif
LDFLAGS = -lm

TARGET = $(SOFTWARE).$(PROCESSOR)

INC_DIR := -I.

SRCS := multisoftware_office_telecomm.c ../acPthread.c pbm_stringsearch_s/pbmsrch_large.c bmh_stringsearch_s/bmhsrch.c fft_s/fft.c fft_s/fftmisc.c fft_s/fourierf.c adpcm_s/rawcaudio.c adpcm_s/adpcm.c 




OBJSAUX :=  multisoftware_office_telecomm.o acPthread.o pbmsrch_large.o   bmhsrch.o fft.o fftmisc.o  fourierf.o rawcaudio.o adpcm.o 

#crc_32.o

#bmhasrch.o bmhisrch.o bmhsrch.o dijkstra.o

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
	cp adpcm_s/inputencoder.pcm ../../rundir/$(RUNDIRNAME)
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
	cp adpcm_s/output_adpcm.base ../../rundir/$(RUNDIRNAME)
	cp bmh_stringsearch_s/output_bmh_stringsearch.base ../../rundir/$(RUNDIRNAME)
	cp pbm_stringsearch_s/output_pbm_stringsearch.base ../../rundir/$(RUNDIRNAME)
	cp fft_s/output_fft.base ../../rundir/$(RUNDIRNAME)

check:
	echo "\n\nTESTING RESULTS"
	if diff output_adpcm output_adpcm.base > diff.txt ; then echo "\n ADPCM Passed."; else echo "\n ADPCM Failed."; fi
	if diff output_bmh_stringsearch output_bmh_stringsearch.base > diff.txt ; then echo "\n BMH Stringsearch Passed."; else echo "\n BMH Stringsearch Failed."; fi
	if diff output_pbm_stringsearch output_pbm_stringsearch.base > diff.txt ; then echo "\n PBM Stringsearch Passed."; else echo "\n PBM Stringsearch Failed."; fi
	if diff output_fft output_fft.base > diff.txt ; then echo "\n FFT Passed."; else echo "\n FFT Failed."; fi

