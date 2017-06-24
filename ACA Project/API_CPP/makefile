.PHONY : all doc clean check

CC  = gcc
CXX = g++

BUILD_DIR = $(PWD)/build
DEBUG = 1

#BOSP  = /home/agosta/Documents/Docs/Horizon2020/MANGO/BOSP
ifdef BOSP
IBOSP = -I$(BOSP)/out/include -I$(BOSP)/out/include/bbque
LBOSP = -L$(BOSP)/out/lib/bbque
BOSP_LIBS = -lbbque_pms
endif

SRC_HOST = src/process.c
SRC_CPP_HOST = src/event.cpp src/mango.cpp src/opencl_wrap.cpp

ifdef DEBUG
SRC_HOST += src/debug.c
endif

O_HOST   = $(subst src,$(BUILD_DIR),$(SRC_HOST))
OBJ_HOST = $(O_HOST:.c=.o)
O_CPP_HOST   = $(subst src,$(BUILD_DIR),$(SRC_CPP_HOST))
OBJ_CPP_HOST += $(O_CPP_HOST:.cpp=.o)

INCLUDE = -I./include $(IBOSP)
CFLAGS  = -Wall
ifdef DEBUG
CFLAGS += -g -DDEBUG 
endif
LDFLAGS = -lrt -pthread $(LBOSP) $(BOSP_LIBS)

all : test mango-dev

clean :
	rm -rf latex/ html/
	rm -r build
	cd test && make clean
	cd src_dev && make clean

doc : 
	doxygen doxy.conf

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)%.o: src/%.c  $(BUILD_DIR)
	$(CC) -c $< -fPIC $(CFLAGS) $(INCLUDE) $(LDFLAGS) -o $@

$(BUILD_DIR)%.o: src/%.cpp  $(BUILD_DIR)
	$(CXX) -std=c++14 -fpermissive -c $< -fPIC $(CFLAGS) $(INCLUDE) $(LDFLAGS) -o $@

mango-host: $(OBJ_HOST) $(OBJ_CPP_HOST)
	$(CXX) -shared -Wl,-soname,lib$@.so -o $(BUILD_DIR)/lib$@.so $(OBJ_HOST) $(OBJ_CPP_HOST)

mango-dev :
	cd src_dev && BUILD_DIR=$(BUILD_DIR) make

test: mango-host mango-dev
	cd test && BUILD_DIR=$(BUILD_DIR) make

check : test 
	cd test && make check
