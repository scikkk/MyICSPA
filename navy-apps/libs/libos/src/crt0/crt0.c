#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// wk
#include <stdio.h>
// wk

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
	printf("args=%p\n", args);
	int argc = *((int*)args);
	printf("call_main:argc=%d\n", argc);
	char **argv = (char**)((uintptr_t)args+sizeof(int));
	char **envp = (char**)(argv + argc + 1);
	printf("argv=%p\tenvp=%p\n", argv, envp);
	environ =  envp;
	// wk
	printf("call_main=%s\n", argv[0]);
	assert(0);
	// wk
	exit(main(argc, argv,  envp));
	assert(0);
}
