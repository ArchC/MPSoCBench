CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)

CFLAGS = -msoft-float -specs=archc
LDFLAGS = -lm

INC_DIR := -I.

SRCS := fft.c fftmisc.c  fourierf.c

OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ $(TARGET).x
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
bin:
	
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<

