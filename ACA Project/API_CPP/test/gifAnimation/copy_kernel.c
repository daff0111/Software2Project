/* Test kernel. 
 * Performs copy of a frame described as an array of bytes (Y*X*RGB)
 */
#include "mango_hn.h"
#include "debug.h"

typedef uint8_t Byte;

void kernel_function(Byte *out, Byte *in, int X, int Y){
	for(int x=0; x<X; x++)
		for(int y=0; y<Y; y++)
			for(int c=0; c<3; c++)
				out[y*X*3+x*3+c] =	in[y*X*3+x*3+c];
}

/*
 * This main function performs the, init, close and argument decoding
 * functions. These should be performed in the kernel itself, but could be
 * easily wrapped in an auto-generated code.
 */
int main(int argc, char **argv){
	Byte *out = (Byte *)strtol(argv[5],NULL,16);
	Byte *in = (Byte *)strtol(argv[6],NULL,16);
	int X = (int)strtol(argv[7],NULL,16);
	int Y = (int)strtol(argv[8],NULL,16);
	mango_init(argv);
	dprint("Reader name: %s\n", argv[0]);
	kernel_function(out, in, X, Y);
	dprint("Kernel completed\n");
	mango_close(15);
}
