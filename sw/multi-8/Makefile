CC = $(CROSS)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) -specs=archc
endif
LDFLAGS = -lm

TARGET = $(SOFTWARE).$(PROCESSOR)

INC_DIR := -I.

SRCS := multisoftware_8.c ../acPthread.c sha_s/sha_driver.c sha_s/sha.c rijndael_s/aesxam.c rijndael_s/aes.c  blowfish_s/bf.c blowfish_s/bf_cfb64.c blowfish_s/bf_skey.c blowfish_s/bf_enc.c pbm_stringsearch_s/pbmsrch_large.c bmh_stringsearch_s/bmhsrch.c fft_s/fft.c fft_s/fftmisc.c fft_s/fourierf.c adpcm_s/rawcaudio.c adpcm_s/adpcm.c 

OBJSAUX :=  multisoftware_8.o acPthread.o sha_driver.o sha.o aesxam.o aes.o  bf.o bf_skey.o bf_cfb64.o bf_enc.o pbmsrch_large.o   bmhsrch.o fft.o fftmisc.o  fourierf.o rawcaudio.o adpcm.o 

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
	rm -f *.x *.o *~  
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
	cp sha_s/input_large.asc ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/inputencoder.asc ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/inputdecoder.enc ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/inputencoder_large.asc ../../rundir/$(RUNDIRNAME)
	cp adpcm_s/inputencoder.pcm ../../rundir/$(RUNDIRNAME)	
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
	cp sha_s/output_sha.base ../../rundir/$(RUNDIRNAME)	
	cp rijndael_s/output_encoder_rijndael.base ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/output_decoder_rijndael.base ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/output_blowfishenc.base ../../rundir/$(RUNDIRNAME)
	cp pbm_stringsearch_s/output_pbm_stringsearch.base ../../rundir/$(RUNDIRNAME)
	cp bmh_stringsearch_s/output_bmh_stringsearch.base ../../rundir/$(RUNDIRNAME)
	cp fft_s/output_fft.base ../../rundir/$(RUNDIRNAME)
	cp adpcm_s/output_encoder_adpcm.base ../../rundir/$(RUNDIRNAME)
	
check:
	#It is not possible to check results for all softwares because severalof them have randomic inputs. 
	#IF NECESSARY, USE THE COMMANDS BELOW

	#echo "\n\nTESTING RESULTS"
	#if diff output_sha output_sha.base > diff_sha.txt ; then echo "\n SHA Passed."; else echo "\n SHA Failed."; fi
	#if diff output_encoder_rijndael output_encoder_rijndael.base > diff_rijndael_encoder.txt ; then echo "\n Rijndael Encoder Passed."; else echo "\n Rijndael Encoder Failed."; fi
	#if diff output_decoder_rijndael output_decoder_rijndael.base > diff_rijndael_decoder.txt ; then echo "\n Rijndael Decoder Passed."; else echo "\n Rijndael Decoder Failed."; fi
	#if diff output_blowfishenc output_blowfishenc.base > diff_blowfishenc.txt ; then echo "\n Blowfish Passed."; else echo "\n Blowfish Failed."; fi
	#if diff output_pbm_stringsearch output_pbm_stringsearch.base > diff_pbm_strinsearch.txt ; then echo "\n PBM Stringsearch Passed."; else echo "\n PBM Stringsearch Failed."; fi
	#if diff output_bmh_stringsearch output_bmh_stringsearch.base > diff_bmh_strinsearch.txt ; then echo "\n BMH Stringsearch Passed."; else echo "\n BMH Stringsearch Failed."; fi
	#if diff output_fft output_fft.base > diff_fft.txt ; then echo "\n FFT Passed."; else echo "\n FFT Failed."; fi
	#if diff output_encoder_adpcm output_encoder_adpcm.base > diff_adpcm.txt ; then echo "\n ADPCM Passed."; else echo "\n ADPCM Failed."; fi

