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
	char **argv = (char**)(args+4);
	char **envp = (char**)(args + 4 + argc*4 + 4);
	environ =  envp;
	// wk
	printf("call_main %s\n", argv[0]);
	assert(0);
	// wk
	exit(main(argc, argv,  envp));
	assert(0);
}
