#include "process.h"
#include "debug.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t gn_execvp(char *filename, char **args){
        int i;
	pid_t pid;
	pid = fork();
	if (!pid){ /* child, executor */
/*		int i;
		dprint("Running %s\n", filename);
		for(i=0; args[i]!=0; i++)
			fprintf(stderr, "\tArg %d = %s\n", i, args[i]);*/
		execvp(filename,args);
	}
	for (i=0; args[i]!=NULL; i++)
		free(args[i]);
	free(args);
	return pid;
}

uint32_t gn_wait(pid_t pid){
	int status;
	waitpid(pid, &status, 0);
	//dprint("Pid %d exit: %d\n", pid, WIFEXITED(status));
	//dprint("Kernel Exit Status: %d\n", WEXITSTATUS(status));
	return WEXITSTATUS(status);
}
