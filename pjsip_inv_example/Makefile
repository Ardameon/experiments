BASE_DIR=`pwd`
PJDIR = $(BASE_DIR)/pjproject

ifndef COMPILER
COMPILER = x86_64
endif

SRC = auto_answer.c
OBJ = $(SRC:%.c=/%.o)
EXE = auto_answer

ifeq ($(COMPILER),arm)
CC = aarch64-linux-gnu-gcc
TARGET = aarch64-unknown-linux-gnu
else
CC = gcc 
TARGET = x86_64-unknown-linux-gnu
endif

LDFLAGS = -L$(PJDIR)/pjlib/lib \
	  -L$(PJDIR)/pjlib-util/lib \
	  -L$(PJDIR)/pjnath/lib \
	  -L$(PJDIR)/pjmedia/lib \
	  -L$(PJDIR)/pjsip/lib \
	  -L$(PJDIR)/third_party/lib

PJLIB_LDLIB = -lpj-$(TARGET)
PJLIB_UTIL_LDLIB = -lpjlib-util-$(TARGET)
PJNATH_LDLIB = -lpjnath-$(TARGET)
PJMEDIA_AUDIODEV_LDLIB = -lpjmedia-audiodev-$(TARGET)
PJMEDIA_VIDEODEV_LDLIB = -lpjmedia-videodev-$(TARGET)
PJMEDIA_LDLIB = -lpjmedia-$(TARGET)
PJMEDIA_CODEC_LDLIB = -lpjmedia-codec-$(TARGET)
PJSIP_LDLIB = -lpjsip-$(TARGET)
PJSIP_SIMPLE_LDLIB = -lpjsip-simple-$(TARGET)
PJSIP_UA_LDLIB = -lpjsip-ua-$(TARGET)
PJSUA_LIB_LDLIB = -lpjsua-$(TARGET)
PJSUA2_LIB_LDLIB = -lpjsua2-$(TARGET)

APP_THIRD_PARTY_LIBS = -lsrtp-$(TARGET)
APP_THIRD_PARTY_LIBS += -lresample-$(TARGET)
APP_THIRD_PARTY_LIBS += -lgsmcodec-$(TARGET)
APP_THIRD_PARTY_LIBS += -lspeex-$(TARGET)
APP_THIRD_PARTY_LIBS += -lilbccodec-$(TARGET)
APP_THIRD_PARTY_LIBS += -lg7221codec-$(TARGET)

LDLIBS = $(PJSUA_LIB_LDLIB) \
	 $(PJSIP_UA_LDLIB) \
	 $(PJSIP_SIMPLE_LDLIB) \
	 $(PJSIP_LDLIB) \
	 $(PJMEDIA_CODEC_LDLIB) \
	 $(PJMEDIA_LDLIB) \
	 $(PJMEDIA_VIDEODEV_LDLIB) \
	 $(PJMEDIA_AUDIODEV_LDLIB) \
	 $(PJMEDIA_LDLIB) \
	 $(PJNATH_LDLIB) \
	 $(PJLIB_UTIL_LDLIB) \
	 $(APP_THIRD_PARTY_LIBS)\
	 $(PJLIB_LDLIB) \
         -lm -lrt -lpthread 
			 
CFLAGS = -DPJ_AUTOCONF=1 \
     	 -O2 -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1 \
	 -I$(PJDIR)/pjlib/include \
	 -I$(PJDIR)/pjlib-util/include \
	 -I$(PJDIR)/pjnath/include \
	 -I$(PJDIR)/pjmedia/include \
	 -I$(PJDIR)/pjsip/include	
	
CPPFLAGS= ${CFLAGS}

all: $(EXE)

$EXE: $(SRC)
	$(CC) -o $@ $< $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ) $(EXE)

