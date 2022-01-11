#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* // wk */
#include <stdio.h>
/* // wk */

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
	int argc = *((int*)args);
	char **argv = (char**)((uintptr_t)args+sizeof(int));
	char **envp = (char**)(argv + argc + 1);
	printf("ctr0:callamin:argv=%p\n", argv);
	printf("ctr0:callamin:envp=%p\n", envp);
	if(envp){
		printf("ctr0=%p\n", envp[0]);
		if(envp[0]) printf("ctro0=%s\n", envp[0]);
	}
	/* if(*argv==NULL) argv = NULL; */
	/* if(*envp==NULL) envp = NULL; */
	environ = envp;
	exit(main(argc, argv,  envp));
	assert(0);
}
