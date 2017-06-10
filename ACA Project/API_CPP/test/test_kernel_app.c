/* 
 *
 * Test application. This application
 * will launch a kernel in PEAK. The kernel will just
 * compute a matrix multiplication. The size of the matrix 
 * and the matrices pointers will be passed as parameters
 *
 * After executing the kernel, the application shows
 * the resulting output matrix
 *
*/
#include "mango_hn.h"
#include "debug.h"
#include <stdlib.h>

void kernel_function(int *A, int *B, int *C, int rows, int cols) {
  int r, c, p;
  for (r=0;r<rows;r++) {
    for (c=0;c<rows;c++) {
      int v = 0;
      for (p=0;p<cols;p++) {
        v = v + A[r * cols + p] * B[c * cols + p];
      }
      C[r * rows + c] = v;
    }
  }
	return;
}

/*
 * This main function performs the, init, close and argument decoding
 * functions. These should be performed in the kernel itself, but could be
 * easily wrapped in an auto-generated code.
 */
int main(int argc, char **argv){
	int *A = (int *)strtol(argv[5],NULL,16);
	int *B = (int *)strtol(argv[6],NULL,16);
	int *C = (int *)strtol(argv[7],NULL,16);
	int rows = (int)strtol(argv[8],NULL,10);
	int cols = (int)strtol(argv[9],NULL,10);
	mango_init(argv);
	dprint("cols: %s\n", argv[9]);
	dprint("Reader name: %s\n", argv[0]);
	dprint("A address: %p\n", A);
	dprint("B address: %p\n", B);
	dprint("C address: %p\n", C);
	dprint("rows: %d\n", rows);
	dprint("cols: %d\n", cols);
	kernel_function(A, B, C, rows, cols);
	dprint("Kernel completed\n");
	mango_close(15);
}
