#include <mango_hn.h>
#include <stdlib.h>
#include <stdint.h>
#include "debug.h"

#define SIZE 4
#define N 4

char *shared_memory;
mango_event_t fifo;

void *task(task_args *a){
	int i;
	fprintf(stderr,"Thread %d started\n", tid(a));
	for(i=0; i<N; i++) {
		mango_barrier(a, &fifo);
		fprintf(stderr,"Thread %d reads %c\n", tid(a), shared_memory[tid(a)]);
	}
	mango_write_synchronization(&fifo, END_FIFO_OPERATION);
	fprintf(stderr,"Thread %d completed\n", tid(a));
	return mango_exit(a);
}

int main(int argc, char **argv){
	shared_memory=(char *)strtol(argv[5],NULL,16);
	fifo.vaddr=(uint32_t *)strtol(argv[6],NULL,16);
	mango_init(argv);
	dprint("Reader name: %s\n", argv[0]);
	dprint("Buffer address: %p\n", shared_memory);
	dprint("Event address: %p\n", fifo.vaddr);
	mango_event_t *e = mango_spawn(&task, SIZE);
	mango_join(e);
	mango_close(15);
}
