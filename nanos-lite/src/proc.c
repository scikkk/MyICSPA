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

extern void naive_uload(PCB *pcb, const char *filename);
extern Context* context_kload(PCB *pcb, void (*entry)(void *), void *arg);
extern Context* context_uload(PCB *pcb, const char *filename);
void init_proc() {
	// wk 4.1
	context_kload(&pcb[0], hello_fun, (void*)0x99);
	/* context_kload(&pcb[1], hello_fun, (void*)0x66); */
	context_uload(&pcb[0], "/bin/hello");
	context_uload(&pcb[1], "/bin/pal");
	// wk 4.1
	switch_boot_pcb();

	Log("Initializing processes...");

	// load program here
	// TEST
	/* naive_uload(NULL, "/bin/exec-test"); */
	/* naive_uload(NULL, "/bin/cpp-test"); */
	// PASSED!
	/* naive_uload(NULL, "/bin/bmp-test"); */
	/* naive_uload(NULL, "/bin/dummy"); */
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
	/* printf("prev=%p\n", prev); */
	current->cp = prev;
	// always select pcb[0] as the new process
	current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
	// then return the new context
	/* printf("cur-cp=%p\n", current->cp); */

	/* printf("pcb0->cp=%p\n", pcb[0].cp); */
	/* printf("pcb1->cp=%p\n", pcb[1].cp); */

	/* printf("mepc=%p\n", (current->cp)->mepc); */
	/* printf("gpr1=%p\n", (current->cp)->GPR1); */
	return current->cp;
	// wk 4.1
	/* return NULL; */
}
