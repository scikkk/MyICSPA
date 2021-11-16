#include <common.h>
#include "syscall.h"

void strace(Context *c) {
	char type[20];
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

	switch (a[0]) {
		case SYS_exit:
			strcpy(type, "SYS_exit");
			break;
		case SYS_yield:
			strcpy(type, "SYS_yield");
			break;
		case SYS_open:
			strcpy(type, "SYS_open");
			break;
		case SYS_read:
			strcpy(type, "SYS_read");
			break;
		case SYS_write:
			strcpy(type, "SYS_write");
			break;
		case SYS_kill:
			strcpy(type, "SYS_kill");
			break;
		case SYS_getpid:
			strcpy(type, "SYS_getpid");
			break;
		case SYS_close:
			strcpy(type, "SYS_close");
			break;
		case SYS_lseek:
			strcpy(type, "SYS_lseek");
			break;
		case SYS_brk:
			strcpy(type, "SYS_brk");
			break;
		case SYS_fstat:
			strcpy(type, "SYS_fstat");
			break;
		case SYS_time:
			strcpy(type, "SYS_time");
			break;
		case SYS_signal:
			strcpy(type, "SYS_signal");
			break;
		case SYS_execve:
			strcpy(type, "SYS_execve");
	}
	printf("[STRACE] TYPE: %s(%p)\ta0:%p\ta1:%p\ta2:%p\n", type, a[0],a[1],a[2],a[3]);
}
void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;


	switch (a[0]) {
		case SYS_yield:
			sys_yield();
			break;
		case SYS_exit:
			/* printf("%d\t%d\t%d\t%d\n\n", a[0],a[1],a[2],a[3]); */
			sys_exit(a[1]);
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
	strace(c);
}
