#include "mango_hn.h"
#include "mango_hn_gn.h"
#include <errno.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "debug.h"


#ifndef BASETLB
#define BASETLB 0x1000000
#endif
#define SIZE 1048576

mango_context_t ctx;

mango_exit_t mango_gn_init(char **argv) {
	ctx.semaphore=sem_open("sem_mango",0);
	if (ctx.semaphore==SEM_FAILED){ 
		dprint("%s\n", strerror(errno));
		return ERR_SEM_FAILED;
	}
	ctx.event_exit.vaddr=(uint32_t *)strtol(argv[1], NULL, 16);
	ctx.event_a.vaddr=(uint32_t *)strtol(argv[2], NULL, 16);
	ctx.event_b.vaddr=(uint32_t *)strtol(argv[3], NULL, 16);
	ctx.event_r.vaddr=(uint32_t *)strtol(argv[4], NULL, 16);
	dprint("Return event address: %p\n", ctx.event_exit);
	dprint("Task event address: %p\n", ctx.event_a);
	dprint("Barrier event address: %p\n", ctx.event_b);
	dprint("Release event address: %p\n", ctx.event_r);
	ctx.tf=open("device_memory.dat",O_RDWR);
	if (ctx.tf==-1) {
		dprint("%s\n", strerror(errno));
		return ERR_FOPEN;		
	}
	ctx.memory = (uint8_t*) mmap ((void *)BASETLB, SIZE,
		PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, ctx.tf,0);
	dprint("Memory address: %p\n", ctx.memory);
	if (ctx.memory==MAP_FAILED) {
		dprint("%s\n", strerror(errno));
		return ERR_MMAP_FAILED;
	}
	return SUCCESS;
}

void mango_gn_close(int status){
        mango_gn_write_synchronization(&ctx.event_exit,1);
        munmap(ctx.memory, SIZE);
        close(ctx.tf);
        exit(status); 
}

void mango_gn_write_synchronization(mango_event_t *e, uint32_t value){
	msync(ctx.memory, SIZE, MS_SYNC);
	sem_wait(ctx.semaphore);
	*(e->vaddr)=value;
	sem_post(ctx.semaphore);
}

uint32_t mango_gn_read_synchronization(mango_event_t *e){
	msync(ctx.memory, SIZE, MS_SYNC);
	sem_wait(ctx.semaphore);
	uint32_t value=*(e->vaddr);
	*(e->vaddr)=0;
	sem_post(ctx.semaphore);
	return value;
}

mango_event_t *mango_gn_spawn(void *(*task)(task_args *), uint32_t range) {
        int i;

	*(ctx.event_a.vaddr)=range+1;
	*(ctx.event_b.vaddr)=range+1;
	*(ctx.event_r.vaddr)=2;
	mango_event_t *e=&ctx.event_a; 
	mango_event_t *b=&ctx.event_b; 
	mango_event_t *r=&ctx.event_r;
	for(i=0; i<range; i++){
		task_args *s = (task_args *)malloc(sizeof(task_args));
		s->event=e;
		s->tid=i;
		s->barrier=b;
		s->release=r;
		s->ntasks=range;
		pthread_t *thread=(pthread_t *)malloc(sizeof(pthread_t));
		pthread_create(thread, NULL, ((void *(*)(void*))task), s);
dbg();
	}
	return e;
}

void mango_gn_task_arg_free(task_args *a){
	free(a);
}

void mango_gn_suspend(){
	usleep(1);
}
