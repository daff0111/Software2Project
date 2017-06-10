CC  = gcc
CXX = g++

DEBUG = 1

SRC_DEV = $(wildcard mango*.c)

ifdef DEBUG
SRC_DEV += debug.c
endif

OBJ_DEV = $(SRC_DEV:.c=.o)

INCLUDE=-I../include_dev

CFLAGS  = -Wall
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

LDFLAGS = -lrt -pthread

.PHONY : all clean

all : mango-dev

%.o: %.c  $(BUILD_DIR)
	$(CC) -c $< -fPIC $(CFLAGS) $(INCLUDE) $(LDFLAGS) -o $@
	
mango-dev : $(OBJ_DEV)
	echo $(OBJ_DEV)
	$(CC) -shared -Wl,-soname,lib$@.so -o $(BUILD_DIR)/lib$@.so $(OBJ_DEV)

clean :
	rm -f *.o

