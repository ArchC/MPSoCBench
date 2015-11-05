CC = $(CROSS)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc
endif
LDFLAGS = -lm

TARGET = $(SOFTWARE).$(PROCESSOR)

INC_DIR := -I.

SRCS := multisoftware_security.c ../acPthread.c sha_s/sha_driver.c sha_s/sha.c rijndael_s/aesxam.c rijndael_s/aes.c  blowfish_s/bf.c blowfish_s/bf_cfb64.c blowfish_s/bf_skey.c blowfish_s/bf_enc.c  


OBJSAUX :=  multisoftware_security.o acPthread.o sha_driver.o sha.o aesxam.o aes.o  bf.o bf_skey.o bf_cfb64.o bf_enc.o 


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
	rm -f *.x *.o *~ ../acPthread.o sha_s/output.dat rijndael_s/outputencoder.enc rijndael_s/outputdecoder.dec blowfish_s/outputencoder.enc


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
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
	cp sha_s/output_sha.base ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/output_encoder_rijndael.base ../../rundir/$(RUNDIRNAME)
	cp rijndael_s/output_decoder_rijndael.base ../../rundir/$(RUNDIRNAME)
	cp blowfish_s/output_blowfish.base ../../rundir/$(RUNDIRNAME)

check:
	echo "\n\nTESTING RESULTS"
	if diff output_sha output_sha.base > diff.txt ; then echo "\n SHA Passed."; else echo "\n SHA Failed."; fi
	if diff output_encoder_rijndael output_encoder_rijndael.base > diff.txt ; then echo "\n Rijndael Encoder Passed."; else echo "\n Rijndael Encoder Failed."; fi
	if diff output_decoder_rijndael output_decoder_rijndael.base > diff.txt ; then echo "\n Rijndael Decoder Passed."; else echo "\n Rijndael Decoder Failed."; fi
	if diff output_blowfish output_blowfish.base > diff.txt ; then echo "\n Blowfish Passed."; else echo "\n Blowfish Failed."; fi

