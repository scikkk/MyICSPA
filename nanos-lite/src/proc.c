#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
	printf("switch_boot_pcb current=%p\n", current);
	current = &pcb_boot;
	printf("switch_boot_pcb current=%p\n", current);
}

void hello_fun(void *arg) {
	int j = 1;
	while (1) {
		/* if(j % 1024 == 0) */
		Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
		j ++;
		yield();
	}
}

extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void (*entry)(void *), void *arg);
extern void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);
void init_proc() {
	// wk 4.1
	context_kload(&pcb[0], hello_fun, (void*)0x99999999);

	char *argv[] = {"--skip", NULL};
	context_uload(&pcb[1], "/bin/pal", argv, NULL);
	/* context_uload(&pcb[1], "/bin/pal", NULL, NULL); */

	/* char *argv[] = {"/bin/exec-test", NULL}; */
	/* context_uload(&pcb[1], "/bin/exec-test", argv, NULL); */

	/* context_uload(&pcb[1], "/bin/menu", NULL, NULL); */

	/* char *argv[] = {"WK", "SCS", "HaHaHa", NULL}; */
	/* char *envp[] = {"USER=WK", "ID=LOVE", NULL}; */
	/* context_uload(&pcb[1], "/bin/nterm", argv, envp); */

	/* context_uload(&pcb[1], "/bin/dummy", NULL, NULL); */

	/* char *argv[] = {"I","WK", "SCS", "HaHaHa", "TIME", NULL}; */
	/* char *envp[] = {"NAME=WK", "NAME=SCS", "ID=201300008", NULL}; */
	/* context_uload(&pcb[1], "/bin/hello", argv, envp); */
	/* context_uload(&pcb[1], "/bin/hello", NULL, NULL); */

	/* context_uload(&pcb[1], "/bin/nslider", NULL, NULL); */

	switch_boot_pcb();
	// wk 4.1

	Log("Initializing processes...");
	/* naive_uload(NULL, "/bin/dummy"); */
}

Context* schedule(Context *prev) {
	if(current == NULL){
		return prev;
	}
	/* printf("enter shedule\n"); */
	// wk 4.1
	/* printf("prev=%p\n", prev); */
	// save the context pointer
	/* printf("current=%p\n", current); */
	/* printf("&current->cp=%p\n", &current->cp); */
	current->cp = prev;
	/* printf("current=%p\tcurrent->as=%p\tas.ptr=%p\tpdir=%p\tmaxbrk=%p\n", current, current->as, current->as.ptr, current->cp->pdir, current->max_brk); */

	static short no = 0;
	no = (no + 1) % 40;
	if(no == 0){
		current = &pcb[0];
	}
	else{
		current = &pcb[1];
	}
	/* printf("current=%p\tcurrent->as=%p\tas.ptr=%p\tpdir=%p\tmaxbrk=%p\n", current, current->as, current->as.ptr, current->cp->pdir, current->max_brk); */

	/* printf("pcb[0].as=%p\tpcb[1].as=%p\n", &pcb[0].as, &pcb[1].as); */
	// then return the new context
	/* printf("cur-cp=%p\n", current->cp); */

	/* printf("pcb0->cp=%p\n", pcb[0].cp); */
	/* printf("pcb1->cp=%p\n", pcb[1].cp); */

	/* printf("mepc=%p\n", (current->cp)->mepc); */
	/* printf("gpr1=%p\n", (current->cp)->GPR1); */
	/* printf("exit shedule\n"); */

	if(current == &pcb[0]){
		/* printf("enter pcb[0]: pdir=%p\n", pcb[0].cp->pdir); */
		pcb[0].cp->pdir = NULL;
	}
	else{
		/* printf("enter pcb[1]: pdir=%p\n", pcb[1].cp->pdir); */
	}
	return current->cp;
	// wk 4.1
	/* return NULL; */
}
