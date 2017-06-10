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
	mango_event_t write_event;
	char *addr_in=(char *)strtol(argv[5],NULL,16);
	char *addr_out=(char *)strtol(argv[6],NULL,16);
	write_event.vaddr=(uint32_t *)strtol(argv[7],NULL,16);
	int i;
	mango_init(argv);
	dprint("Reader name: %s\n", argv[0]);
	dprint("Buffer in address: %p\n", addr_in);
	dprint("Buffer out address: %p\n", addr_out);
	printf("Input: %s\n", addr_in);
	for(i=0; addr_in[i]!='\0'; i++)
		addr_out[i]=addr_in[i]+1;
	mango_write_synchronization(&write_event,1);
dbg();
	mango_close(15);
}
