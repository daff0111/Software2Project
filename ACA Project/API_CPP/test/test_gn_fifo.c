/*
 * This application demonstrates fifo write communication from
 * host application to the kernel, using the C API.
 */
#include "mango.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#define KID 1
#define BID 1

int main(int argc, char **argv){
	if (argc<2) { 
		fprintf(stderr, "Not enough arguments\n");
		return 1;
	}
	/* Initialization */
	mango_init();
	kernelfunction *k=mango_kernelfunction_init();
	mango_load_kernel("./test_kernel_fifo", k, GN, BINARY);

	/* Registration of task graph */
	mango_kernel_t k1=mango_register_kernel(KID, k, 1, 1, BID, BID);
	mango_buffer_t b1=mango_register_memory(BID, 4, FIFO, 1, 1, k1, k1);
	mango_task_graph_t *tg = mango_task_graph_create(1,1,0,k1,b1);

	/* Resource Allocation */
	mango_resource_allocation(tg);

	/* Execution setup */
	mango_arg_t *arg1 = mango_arg((void *)b1, sizeof(uint64_t), FIFO);
	mango_arg_t *arg2 = mango_arg((void *)mango_get_buffer_event(b1), sizeof(uint32_t), EVENT);
	mango_args_t *args=mango_set_args(k1, 2, arg1, arg2);

	/* Data transfer and kernel execution */
	mango_event_t e3=mango_write(argv[1], b1, BURST,strlen(argv[1]));
	mango_event_t e2=mango_start_kernel(k1, args, NULL);
	mango_wait(e3);
	mango_wait(e2);
	
	/* Deallocation and teardown */
	mango_resource_deallocation(tg);
	mango_task_graph_destroy_all(tg);
	mango_release();
	return 0;
}
