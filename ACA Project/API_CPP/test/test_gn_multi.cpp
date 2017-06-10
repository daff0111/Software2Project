/*
 * This application demonstrates the use of multiple kernels with
 * shared memory. In this case, synchronization is managed at host side.
 */
#include "mango.h"
#include "debug.h"
#include <stdlib.h>
#define K1 1
#define K2 2
#define B1 1
#define B2 2
#define B3 3
#define B4 4

/* function prototypes */
void init_matrix(int *matrix, int cols, int rows);

/* kernel function, reported here to allow checking the results 
 * obtained in the offloaded version 
 */
void kernel_function(int *A, int *B, int *C, int rows, int cols) {
  for (int r=0;r<rows;r++) {
    for (int c=0;c<cols;c++) {
      int v = 0;
      for (int p=0;p<rows;p++) {
        v = v + A[r * cols + p] * B[p * cols + c];
      }
      C[r * cols + c] = v;
    }
  }
	return;
}

int main(int argc, char**argv) {
  int *A;
  int *B;
  int *C;
  int *D;
  int rows;
  int columns;
	int out=0;

  if (argc < 3) {
    printf("error, bad arguments\n");
    printf("Arguments: <rows> <columns>\n");
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
  A = (int*)malloc(rows*columns*sizeof(int));
  B = (int*)malloc(rows*columns*sizeof(int));
  C = (int*)malloc(rows*columns*sizeof(int));
  D = (int*)malloc(rows*columns*sizeof(int));

	// Initialization
	auto mango_rt = mango::Context();
	auto k = mango::KernelFunction();
	k.load("./test_kernel_app", mango::UnitType::GN, mango::FileType::BINARY);

	// Registration of task graph
	auto k1 = mango_rt.register_kernel(K1, k, {B1,B2}, {B3});
	auto k2 = mango_rt.register_kernel(K2, k, {B1,B3}, {B4});

	auto b1 = mango_rt.register_buffer<mango::Buffer>(B1,
		 rows*columns*sizeof(int), {}, {K1,K2});
	auto b2 = mango_rt.register_buffer<mango::Buffer>(B2,
		 rows*columns*sizeof(int), {}, {K1});
	auto b3 = mango_rt.register_buffer<mango::Buffer>(B3,
		 rows*columns*sizeof(int), {K1}, {K2});
	auto b4 = mango_rt.register_buffer<mango::Buffer>(B4,
		 rows*columns*sizeof(int), {K2}, {});

	auto tg = mango::TaskGraph({ k1, k2 }, { b1, b2, b3, b4 });

	// Resource Allocation
	if (mango_rt.resource_allocation(tg)!=mango::ExitCode::SUCCESS)
		return -1;

	// Execution setup
	auto arg1 = mango::BufferArg( b1 );
	auto arg2 = mango::BufferArg( b2 );
	auto arg3 = mango::BufferArg( b3 );
	auto arg2b = mango::BufferArg( b3 );
	auto arg3b = mango::BufferArg( b4 );
	auto arg4 = mango::ScalarArg<int>( rows );
	auto arg5 = mango::ScalarArg<int>( columns );

	auto args1 = mango::KernelArguments(
		{ &arg1, &arg2, &arg3, &arg4, &arg5 }, k1);
	auto args2 = mango::KernelArguments(
		{ &arg1, &arg2b, &arg3b, &arg4, &arg5 }, k2);

	// Data transfer and kernel execution
	b1->write(A);
	b2->write(B);

	auto e1=mango_rt.start_kernel(k1, args1);
	e1->wait();

	auto e2=mango_rt.start_kernel(k2, args2);
	e2->wait();
	
	// Deallocation and teardown
	mango_rt.resource_deallocation(tg);

	return 0;
}


void init_matrix(int *matrix, int rows, int cols) {
  for (int r=0;r<rows;r++) 
    for (int c=0;c<cols;c++) 
      matrix[r*cols+c] = random() % 100;
}
