.PHONY : all check clean

EXE=gifscale scale_kernel copy_kernel smooth_kernel


BOSP  = /home/agosta/Documents/Docs/Horizon2020/MANGO/BOSP
ifdef BOSP
IBOSP = -I$(BOSP)/out/include -I$(BOSP)/out/include/bbque
LBOSP = -L$(BOSP)/out/lib/bbque
BOSP_LIBS = -lbbque_pms -lbbque_rtlib -lbbque_tg
BOSP_PATH =$(BOSP)/out/lib/bbque 
endif



all : $(EXE)

check : all
	LD_LIBRARY_PATH=../../build:$(BOSP_PATH) gdb ./gifscale

clean : 
	rm -f $(EXE) *.o device_memory.dat *.gif

gifscale : AnimatedGifSaver.cpp main.cpp
	g++ -g -DBOSP -std=c++14 -o $@ -I./ -I../../include $(IBOSP) $^ -lgif -L../../build/ $(LBOSP)  -lmango-host -pthread  $(BOSP_LIBS) 

scale_kernel : scale_kernel.c
	gcc -o $@ $< -I../../include_dev -L../../build/ -lmango-dev -pthread

copy_kernel : copy_kernel.c
	gcc -o $@ $< -I../../include_dev -L../../build/ -lmango-dev -pthread

smooth_kernel : smooth_kernel.c
	gcc -o $@ $< -I../../include_dev -L../../build/ -lmango-dev -pthread

