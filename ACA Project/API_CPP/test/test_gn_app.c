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
 * This application demonstrates read & write communication between
 * kernel and host application.
 */

#include <stdlib.h>
#include <stdio.h>

#include "mango.h"

#define KID 1
#define B1 1
#define B2 2
#define B3 3

/* function prototypes */
void init_matrix(int *matrix, int rows, int cols);

/* kernel function, reported here to allow checking the results 
 * obtained in the offloaded version 
 */
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


void main(int argc, char**argv) {
  int *A;
  int *B;
  int *C;
  int *D;
  int rows;
  int columns;
  int out=0;
  int i, j;

  if (argc < 3) {
    printf("error, bad arguments\n");
    printf("Arguments: %s <rows> <columns>\n");
    exit(1);
  }

  rows = atoi(argv[1]);
  columns = atoi(argv[2]);

  if ((rows == 0) || (columns == 0)) {
    printf("error, rows and columns should be not zero\n");
		printf("rows = %d; columns = %d\n", rows, columns);
    exit(1);
  }
 
  /* matrix allocation */
  A = malloc(rows*columns*sizeof(int));
  B = malloc(rows*columns*sizeof(int));
  C = malloc(rows*rows*sizeof(int));
  D = malloc(rows*rows*sizeof(int));

  /* input matrices initialization */
  init_matrix(A, rows, columns);
  init_matrix(B, rows, columns);

  /* initialization of the mango context */
  mango_init(); 

  /* Kernel setup and registration */
  kernelfunction *k = mango_kernelfunction_init();
  mango_load_kernel("./test_kernel_app", k, GN, BINARY);
  mango_kernel_t k1 = mango_register_kernel(KID, k, 2, 1, B1, B2, B3);  

  /* Registration of buffers */
  mango_buffer_t b1 = mango_register_memory(B1, rows*columns*sizeof(int), BUFFER, 0, 1, k1);
  mango_buffer_t b2 = mango_register_memory(B2, rows*columns*sizeof(int), BUFFER, 0, 1, k1);
  mango_buffer_t b3 = mango_register_memory(B3, rows*rows*sizeof(int), BUFFER, 1, 0, k1);
  
  /* Registration of task graph */
  mango_task_graph_t *tg = mango_task_graph_create(1, 3, 0, k1, b1, b2, b3);

  printf("before resource allocation...\n");

  /* resource allocation */
  mango_resource_allocation(tg);

  printf("after resource allocation...\n");

  /* Execution preparation */
  mango_arg_t *arg1 = mango_arg( (void *)b1, sizeof(uint64_t), BUFFER );
  mango_arg_t *arg2 = mango_arg( (void *)b2, sizeof(uint64_t), BUFFER );
  mango_arg_t *arg3 = mango_arg( (void *)b3, sizeof(uint64_t), BUFFER );
  mango_arg_t *arg4 = mango_arg( rows, sizeof(uint32_t), SCALAR );
  mango_arg_t *arg5 = mango_arg( columns, sizeof(uint32_t), SCALAR );
	mango_args_t *args=mango_set_args(k1, 5, arg1, arg2, arg3, arg4, arg5);

  /* Data transfer host->device */
  mango_write(A, b1, DIRECT, 0);
  mango_write(B, b2, DIRECT, 0);

  /* spawn kernel */
  mango_event_t ev = mango_start_kernel(k1, args, NULL);

  /* wait for kernel completion */
  mango_wait(ev);

  /* reading results */
  mango_read(C, b3, DIRECT, 0);

  /* shut down the mango infrastructure */
	mango_resource_deallocation(tg);
	mango_task_graph_destroy_all(tg);
	mango_release();
	
	/* check results */
	kernel_function(A, B, D, rows, columns);
	
	for(i=0; i<rows; i++)
		for(j=0; j<rows; j++)
			if(D[i*rows+j]!=C[i*rows+j]) {
				printf("Incorrect value at %d, %d: %d vs %d\n", i, j, D[i*rows+j], C[i*rows+j]);
				out++;
			} else {
				printf("Correct value at %d, %d: %d vs %d\n", i, j, D[i*rows+j], C[i*rows+j]);
			}
	
	if (out) {
		printf("Detected %d errors in the computation\n", out);
		exit(out);
	} else {
		printf("Matrix multiplication correctly performed\n");
		exit(0);
	}
}	

void init_matrix(int *matrix, int rows, int cols)
{
  int r, c;

  for (r=0;r<rows;r++) {
    for (c=0;c<cols;c++) {
      matrix[r*cols+c] = r*cols+c; // random() % 100;
    }
  }
}
