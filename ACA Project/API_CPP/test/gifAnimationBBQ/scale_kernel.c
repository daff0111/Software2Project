/* Test kernel. 
 * Performs x2 scaling of a frame described as an array of bytes (Y*X*RGB)
 */
#include "mango_hn.h"
#include "debug.h"

typedef uint8_t Byte;

void kernel_function(Byte *out, Byte *in, int X, int Y){
	int X2=X*2;
	int Y2=Y*2;
	for(int x=0; x<X2; x++)
		for(int y=0; y<Y2; y++)
			for(int c=0; c<3; c++){
				out[y*X2*3+x*3+c]=in[y/2*X*3+x/2*3+c];
			}
}

/*
 * This main function performs the, init, close and argument decoding
 * functions. These should be performed in the kernel itself, but could be
 * easily wrapped in an auto-generated code.
 */
int main(int argc, char **argv){
	mango_event_t write_event;
	mango_event_t read_event;
	Byte *out = (Byte *)strtol(argv[5],NULL,16);
	Byte *in = (Byte *)strtol(argv[6],NULL,16);
	int X = (int)strtol(argv[7],NULL,16);
	int Y = (int)strtol(argv[8],NULL,16);
	read_event.vaddr=(uint32_t *)strtol(argv[9],NULL,16);
	write_event.vaddr=(uint32_t *)strtol(argv[10],NULL,16);
	mango_init(argv);
	dprint("Kernel %s start\n", argv[0]);
	mango_wait(&read_event,READ);
	kernel_function(out, in, X, Y);
	dprint("Kernel %s stop\n", argv[0]);
	mango_write_synchronization(&read_event,WRITE);
	mango_write_synchronization(&write_event,READ);
	mango_close(15);
}
