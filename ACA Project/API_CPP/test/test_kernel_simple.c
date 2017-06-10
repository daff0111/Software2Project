#include <stdio.h>
#include "debug.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "mango_hn.h"

#define BASETLB 0x1000000
#define SIZE 1048576

int main(int argc, char **argv){
	char *addr=(char *)strtol(argv[5],NULL,16);
	void *e2=(char *)strtol(argv[6],NULL,16);
	mango_init(argv);
	dprint("Reader name: %s\n", argv[0]);
	dprint("Buffer address: %p\n", addr);
	dprint("Event address: %p\n", e2);
	printf("Input: %s\n", addr);
dbg();
	mango_close(15);
}
