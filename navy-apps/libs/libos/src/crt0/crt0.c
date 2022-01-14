#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* // wk */
#include <stdio.h>
/* // wk */

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
	printf("\n\ncall_main\n");
	int argc = *((int*)args);
	char **argv = (char**)((uintptr_t)args+sizeof(int));
	char **envp = (char**)(argv + argc + 1);
	printf("ctr0:callamin:argv=%p\n", argv);
	printf("ctr0:callamin:envp=%p\n", envp);
	if(argv){
		for(int k = 0; argv[k]; k++){
			printf("%s\n", argv[k]);
		}
	}
	if(envp){
		for(int k = 0; envp[k]; k++){
			printf("%s\n", envp[k]);
		}
	}
	printf("callmain\n");
	/* if(*argv==NULL) argv = NULL; */
	/* if(*envp==NULL) envp = NULL; */
	environ = envp;
	exit(main(argc, argv,  envp));
	assert(0);
}
