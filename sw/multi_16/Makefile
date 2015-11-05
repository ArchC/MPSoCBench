CC = $(CROSS)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc
endif
LDFLAGS = -lm

TARGET = $(SOFTWARE).$(PROCESSOR)

INC_DIR := -I.

SRCS := multisoftware_16.c ../acPthread.c sha_s/sha_driver.c sha_s/sha.c rijndael_s/aesxam.c rijndael_s/aes.c  blowfish_s/bf.c blowfish_s/bf_cfb64.c blowfish_s/bf_skey.c blowfish_s/bf_enc.c pbm_stringsearch_s/pbmsrch_large.c bmh_stringsearch_s/bmhsrch.c fft_s/fft.c fft_s/fftmisc.c fft_s/fourierf.c adpcm_s/rawcaudio.c adpcm_s/adpcm.c adpcm_s/rawdaudio.c basicmath_sqrt/basicmath_sqrt.c basicmath_sqrt/isqrt.c basicmath_cubic/basicmath_cubic.c basicmath_cubic/cubic.c basicmath_conversion/rad2deg.c basicmath_conversion/basicmath_conversion.c susancorners_s/susancorners.c  dijkstra_s/dijkstra_static.c bitcount_s/bitcnt_1.c bitcount_s/bitcnt_2.c bitcount_s/bitcnt_3.c bitcount_s/bitcnt_4.c bitcount_s/bitcnts.c  bitcount_s/bitstrng.c bitcount_s/bstr_i.c
#qsort_s/qsort_small.c
OBJSAUX :=  multisoftware_16.o acPthread.o sha_driver.o sha.o aesxam.o aes.o  bf.o bf_skey.o bf_cfb64.o bf_enc.o pbmsrch_large.o   bmhsrch.o fft.o fftmisc.o  fourierf.o rawcaudio.o adpcm.o rawdaudio.o basicmath_cubic.o cubic.o rad2deg.o basicmath_conversion.o susancorners.o  dijkstra_static.o bitcnt_1.o bitcnt_2.o bitcnt_3.o bitcnt_4.o bitcnts.o bitstrng.o bstr_i.o isqrt.o basicmath_sqrt.o

#qsort_small.o

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
	cp sha_s/input_small.asc ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/inputencoder.asc ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/inputdecoder.enc ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/inputencoder_bl.asc ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/inputdecoder_bl.enc ../../rundir/$(RUNDIRNAME)
	cp adpcm_s/inputencoder.pcm ../../rundir/$(RUNDIRNAME)	
	cp adpcm_s/inputdecoder.adpcm ../../rundir/$(RUNDIRNAME)
	cp susancorners_s/input_large2.pgm ../../rundir/$(RUNDIRNAME)
	cp susancorners_s/input_small.pgm ../../rundir/$(RUNDIRNAME)
	cp dijkstra_s/input.dat ../../rundir/$(RUNDIRNAME)
	#cp qsort_s/input_small.dat ../../rundir/$(RUNDIRNAME)
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
	cp sha_s/output_sha.base  ../../rundir/$(RUNDIRNAME)	
	cp rijndael_s/output_encoder_rijndael.base ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/output_decoder_rijndael.base ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/output_blowfishenc.base ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/output_blowfishdec.base ../../rundir/$(RUNDIRNAME)
	cp pbm_stringsearch_s/output_pbm_stringsearch.base ../../rundir/$(RUNDIRNAME)
	cp bmh_stringsearch_s/output_bmh_stringsearch.base ../../rundir/$(RUNDIRNAME)
	#cp fft_s/output_fft.base ../../rundir/$(RUNDIRNAME)
	cp adpcm_s/output_encoder_adpcm.base ../../rundir/$(RUNDIRNAME)
	cp adpcm_s/output_decoder_adpcm.base ../../rundir/$(RUNDIRNAME)
	cp basicmath_cubic/output_basicmath_cubic.base ../../rundir/$(RUNDIRNAME)
	cp basicmath_conversion/output_basicmath_conversion.base ../../rundir/$(RUNDIRNAME)
	cp susancorners_s/output_susancorners.base ../../rundir/$(RUNDIRNAME)
	#cp qsort_s/output_qsort.base ../../rundir/$(RUNDIRNAME)
	cp dijkstra_s/output_dijkstra.base ../../rundir/$(RUNDIRNAME)
	#cp bitcount_s/output_bitcount.base ../../rundir/$(RUNDIRNAME)

check:
	#It is not possible to check results for all softwares because severalof them have randomic inputs. 
	#IF NECESSARY, USE THE COMMANDS BELOW
	#echo "\n\nTESTING RESULTS"
	#if diff output_sha output_sha.base > diff_sha.txt ; then echo "\n SHA Passed."; else echo "\nTest SHA Failed."; fi
	#if diff output_encoder_rijndael output_encoder_rijndael.base > diff_rijndael_encoder.txt ; then echo "\n Rijndael Encoder Passed."; else echo "\n Rijndael Encoder Failed."; fi
	#if diff output_decoder_rijndael output_decoder_rijndael.base > diff_rijndael_decoder.txt ; then echo "\n Rijndael Decoder Passed."; else echo "\n Rijndael Decoder Failed."; fi
	#if diff output_blowfishenc output_blowfishenc.base > diff_blowfishenc.txt ; then echo "\n Blowfish Encoder Passed."; else echo "\n Blowfish Encoder Failed."; fi
	#if diff output_blowfishdec output_blowfishdec.base > diff_blowfishdec.txt ; then echo "\n Blowfish Decoder Passed."; else echo "\n Blowfish Decoder Failed."; fi
	#if diff output_pbm_stringsearch output_pbm_stringsearch.base > diff_pbm_strinsearch.txt ; then echo "\n PBM Stringsearch Passed."; else echo "\n PBM Stringsearch Failed."; fi
	#if diff output_bmh_stringsearch output_bmh_stringsearch.base > diff_bmh_strinsearch.txt ; then echo "\n BMH Stringsearch Passed."; else echo "\n BMH Stringsearch Failed."; fi
	#if diff output_fft output_fft.base > diff_fft.txt ; then echo "\n FFT Passed."; else echo "\n FFT Failed."; fi
	#if diff output_encoder_adpcm output_encoder_adpcm.base > diff_encoder_adpcm.txt ; then echo "\n ADPCM Encoder Passed."; else echo "\n ADPCM Encoder Failed."; fi
	#if diff output_decoder_adpcm output_decoder_adpcm.base > diff_decoder_adpcm.txt ; then echo "\n ADPCM Decoder Passed."; else echo "\n ADPCM Decoder Failed."; fi
	#if diff output_basicmath_cubic output_basicmath_cubic.base > diff_basicmath_cubic.txt ; then echo "\n Basicmath Cubic Passed."; else echo "\n Basicmath Cubic Failed."; fi
	#if diff output_basicmath_conversion output_basicmath_conversion.base > diff_basicmath_conversion.txt ; then echo "\n Basicmath Conversion Passed."; else echo "\n Basicmath Conversion Failed."; fi
	#if diff output_susancorners output_susancorners.base > diff_susancorners.txt ; then echo "\n Susancorners Passed."; else echo "\n Susancorners Failed."; fi
	#if diff output_qsort output_qsort.base > diff_qsort.txt ; then echo "\n Qsort Passed."; else echo "\n Qsort Failed."; fi
	#if diff output_dijkstra output_dijkstra.base > diff_dijkstra.txt ; then echo "\n Dijkstra Passed."; else echo "\n Dijkstra Failed."; fi
	#if diff output_bitcount output_bitcount.base > diff_bitcount.txt ; then echo "\n Bitcount Passed."; else echo "\n Bitcount Failed."; fi



