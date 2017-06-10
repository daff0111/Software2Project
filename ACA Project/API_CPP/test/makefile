.PHONY : all check
EXE = test_gn_simple test_kernel_simple test_kernel_fifo test_gn_fifo_cpp test_gn_app test_kernel_app
EXE+= test_gn_fifo test_gn_multi #test_gn_bbq test_kernel_bbq
 
ifeq (x$(BUILD_DIR), x)
BUILD_DIR=../build
endif

#BOSP  = /home/agosta/Documents/Docs/Horizon2020/MANGO/BOSP
ifdef BOSP
IBOSP = -I$(BOSP)/out/include -I$(BOSP)/out/include/bbque
LBOSP = -L$(BOSP)/out/lib/bbque
BOSP_LIBS = -lbbque_pms -lbbque_rtlib -lbbque_tg
BOSP_PATH =$(BOSP)/out/lib/bbque 
endif


all : $(EXE)
#	cd gifAnimation && make

test_kernel_app : test_kernel_app.c
	gcc -g -DDEBUG -o $@ $< -I../include_dev -pthread -lmango-dev -L$(BUILD_DIR)

test_gn_app : test_gn_app.c
	gcc -g -DDEBUG -o $@ $< -I../include -pthread -lmango-host -L$(BUILD_DIR)

test_kernel_simple : test_kernel_simple.c
	gcc -g -DDEBUG -o $@ $< -I../include_dev -pthread -lmango-dev -L$(BUILD_DIR)

test_gn_simple : test_gn_simple.cpp
	g++ -std=c++14 -g -DDEBUG -o $@ $< -I../include -pthread -lmango-host -L$(BUILD_DIR)

test_kernel_fifo : test_kernel_fifo.c
	gcc -g -DDEBUG -o $@ $< -I../include_dev -pthread -lmango-dev -L$(BUILD_DIR)

test_gn_fifo_cpp : test_gn_fifo.cpp
	g++ -std=c++14 -g -DDEBUG -o $@ $< -I../include -pthread -lmango-host -L$(BUILD_DIR)

test_gn_fifo : test_gn_fifo.c
	gcc -g -DDEBUG -o $@ $< -I../include -pthread -lmango-host -L$(BUILD_DIR)

test_gn_multi : test_gn_multi.cpp
	g++ -std=c++14 -g -DDEBUG -o $@ $< -I../include -pthread -lmango-host -L$(BUILD_DIR)

test_kernel_bbq : test_kernel_bbq.c
	gcc -g -DDEBUG -o $@ $< -I../include_dev -pthread -lmango-dev -L$(BUILD_DIR)

test_gn_bbq : test_gn_bbq.cpp
	g++ -std=c++14 -g -DBOSP -DDEBUG -o $@ $< -I../include $(IBOSP) -pthread -lmango-host $(BOSP_LIBS) -L$(BUILD_DIR) $(LBOSP)

check :
	LD_LIBRARY_PATH=../build ./test_gn_simple args 2> test_simple.log
	LD_LIBRARY_PATH=../build ./test_gn_fifo argsargsargsargs 2> test_fifo.log
	LD_LIBRARY_PATH=../build ./test_gn_fifo_cpp abcdefghijklmnop 2> test_fifo_cpp.log
	LD_LIBRARY_PATH=../build ./test_gn_app 4 5 2> test_app.log
	LD_LIBRARY_PATH=../build ./test_gn_multi 4 5 2> test_app.log
	cd gifAnimation && make check
	#cd gifAnimationBBQ && make check
	#LD_LIBRARY_PATH=../build:$(BOSP_PATH) ./test_gn_bbq args 
	echo "MANGO API tests successfully passed!"

clean :
	rm -f $(EXE) *.o device_memory.dat *.log
	cd gifAnimation && make clean
