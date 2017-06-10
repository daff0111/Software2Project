.PHONY : all check clean

EXE=gifscale scale_kernel copy_kernel smooth_kernel

all : $(EXE)

check : all
	LD_LIBRARY_PATH=../../build ./gifscale

clean : 
	rm -f $(EXE) *.o device_memory.dat *.gif

gifscale : AnimatedGifSaver.cpp main.cpp
	g++ -g -std=c++14 -o $@ -I./ -I../../include $^ -lgif -L../../build/ -lmango-host -pthread

scale_kernel : scale_kernel.c
	gcc -o $@ $< -I../../include_dev -L../../build/ -lmango-dev -pthread

copy_kernel : copy_kernel.c
	gcc -o $@ $< -I../../include_dev -L../../build/ -lmango-dev -pthread

smooth_kernel : smooth_kernel.c
	gcc -o $@ $< -I../../include_dev -L../../build/ -lmango-dev -pthread

