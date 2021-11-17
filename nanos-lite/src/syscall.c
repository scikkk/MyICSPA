#include <common.h>

enum {
	SYS_exit,
	SYS_yield,
	SYS_open,
	SYS_read,
	SYS_write,
	SYS_kill,
	SYS_getpid,
	SYS_close,
	SYS_lseek,
	SYS_brk,
	SYS_fstat,
	SYS_time,
	SYS_signal,
	SYS_execve,
	SYS_fork,
	SYS_link,
	SYS_unlink,
	SYS_wait,
	SYS_times,
	SYS_gettimeofday
};

int sys_yield(){
	yield();
	return 0;
}

void sys_exit(int code){
	halt(code);
}

int sys_write(int fd, const void *buf, int count){
	/* count = 200; */
	if (fd == 1 || fd == 2){
		for(int k = 0; k < count; k++){
			putch(*(((char*)buf)+k));
		} 
		return count;
	}
	return -1;
}

int sys_brk(int32_t addr){
	/* printf("brk_addr:%p\n", addr); */
	return 0;
}





void strace(Context *c) {
	char oneline[20];
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

	switch (a[0]) {
		case SYS_exit:
			sprintf(oneline, "sys_exit(%d)", (int)a[1]);
			break;
		case SYS_yield:
			strcpy(oneline, "sys_yield()");
			break;
		case SYS_open:
			strcpy(oneline, "sys_open");
			break;
		case SYS_read:
			strcpy(oneline, "sys_read");
			break;
		case SYS_write:
			sprintf(oneline, "sys_write(%d, %s, %d)", (int)a[1], (char*)a[2], (int)a[3]);
			break;
		case SYS_kill:
			strcpy(oneline, "sys_kill");
			break;
		case SYS_getpid:
			strcpy(oneline, "sys_getpid");
			break;
		case SYS_close:
			strcpy(oneline, "sys_close");
			break;
		case SYS_lseek:
			strcpy(oneline, "sys_lseek");
			break;
		case SYS_brk:
			sprintf(oneline, "sys_brk(%p)", a[1]);
			break;
		case SYS_fstat:
			strcpy(oneline, "sys_fstat");
			break;
		case SYS_time:
			strcpy(oneline, "sys_time");
			break;
		case SYS_signal:
			strcpy(oneline, "sys_signal");
			break;
		case SYS_execve:
			strcpy(oneline, "sys_execve");
	}
	static int no = 1;
	printf("[STRACE] %d %s\n", no++, oneline);
}


void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

	strace(c);

	switch (a[0]) {
		case SYS_yield:
			sys_yield();
			break;
		case SYS_exit:
			/* printf("%d\t%d\t%d\t%d\n\n", a[0],a[1],a[2],a[3]); */
			sys_exit(a[1]);
			break;
		case SYS_write:
			sys_write(a[1], (void*)a[2], a[3]);
			break;
		case SYS_brk:
			sys_brk(a[1]);
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
}
