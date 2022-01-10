#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
	current = &pcb_boot;
}

void hello_fun(void *arg) {
	int j = 1;
	while (1) {
		Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
		j ++;
		yield();
	}
}
// wk 4.1
Context* context_kload(PCB *pcb, void (*entry)(void *), void *arg){
	Area kstack = {&pcb[0], (void*)pcb+sizeof(PCB)};
	pcb->cp = kcontext(kstack, entry, arg);
	printf("kcontext-ret=%p\n", pcb->cp);
	return pcb->cp;
}
// wk 4.1


extern void naive_uload(PCB *pcb, const char *filename);
void init_proc() {
	// wk 4.1
	context_kload(&pcb[0], hello_fun, NULL);
	// wk 4.1


	switch_boot_pcb();

	Log("Initializing processes...");

	// load program here

	// TEST
	/* naive_uload(NULL, "/bin/exec-test"); */
	/* naive_uload(NULL, "/bin/cpp-test"); */

	// PASSED!
	/* naive_uload(NULL, "/bin/bmp-test"); */
	naive_uload(NULL, "/bin/dummy");
	/* naive_uload(NULL, "/bin/event-test"); */
	/* naive_uload(NULL, "/bin/file-test"); */
	/* naive_uload(NULL, "/bin/hello"); */
	/* naive_uload(NULL, "/bin/timer-test"); */

	// APP
	/* naive_uload(NULL, "/bin/nslider"); */
	/* naive_uload(NULL, "/bin/menu"); */
	/* naive_uload(NULL, "/bin/nterm"); */
	/* naive_uload(NULL, "/bin/bird"); */
	/* naive_uload(NULL, "/bin/pal"); */

}

Context* schedule(Context *prev) {
	// wk 4.1
	// save the context pointer
	printf("prev=%p\n", prev);
	current->cp = prev;
	// always select pcb[0] as the new process
	current = &pcb[0];
	// then return the new context
	printf("cur-cp=%p\n", current->cp);
	return current->cp;
	// wk 4.1
	/* return NULL; */
}
