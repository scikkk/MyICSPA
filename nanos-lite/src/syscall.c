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

int fs_open(const char *pathname, int flags, int mode);
int sys_open(const char *path, int flags, int mode) {
	int ret = fs_open(path, flags, mode);
	return ret;
}


int fs_lseek(int fd, int offset, int whence);
int sys_lseek(int fd, size_t offset, int whence) {
	int ret = fs_lseek(fd, offset, whence);
	return ret;
}

int sys_brk(int32_t addr){
	/* printf("brk_addr:%p\n", addr); */
	return 0;
}



typedef struct {
	char *name;
	size_t size;
	size_t disk_offset;
} Finfo;

static Finfo file_table[] __attribute__((used)) = {
	[0]  = {"stdin", 0, 0},
	[1] = {"stdout", 0, 0},
	[2] = {"stderr", 0, 0},
#include "files.h"
};

void strace(Context *c, char* ret) {
	char  oneline[1280];
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

			char tmp[20];
	switch (a[0]) {
		case SYS_exit:
			sprintf(oneline, "sys_exit(%d)", (int)a[1]);
			break;
		case SYS_yield:
			strcpy(oneline, "sys_yield()");
			break;
		case SYS_open:
			/* sprintf(oneline, "sys_open(%d(%s), %d, %d)", (int)a[1], file_table[a[1]].name, (int)a[2], (int)a[3]); */
			break;
		case SYS_read:
			strcpy(oneline, "sys_read");
			break;
		case SYS_write:
		 	strncpy(tmp, (char*)a[2], 9);
			tmp[9] = '\0';
			
			sprintf(oneline, "sys_write(%d(%s), %s..., %d)", (int)a[1], file_table[a[1]].name, tmp, (int)a[3]);
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
			sprintf(oneline, "sys_lseek(%d(%s), %p, %d)", (int)a[1], file_table[a[1]].name, a[2], (int)a[3]);
			break;
		case SYS_brk:
			sprintf(oneline, "sys_brk(%p)", (int)a[1]);
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
	printf("[STRACE] %d %s = %s\n", no++, oneline, ret);
}


void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

int ret = 0;
char sret[20]="ret";
	strace(c, sret);
	switch (a[0]) {
		case SYS_exit:
			/* printf("%d\t%d\t%d\t%d\n\n", a[0],a[1],a[2],a[3]); */
	strace(c, "void");
			 sys_exit(a[1]);
			break;
		case SYS_yield:
			 ret = sys_yield();
			break;
		case SYS_open:
			 ret = sys_open((char*)a[1], a[2], a[3]);
			break;
		case SYS_write:
			 ret = sys_write(a[1], (void*)a[2], a[3]);
			break;
		case SYS_lseek:
			 ret = sys_lseek(a[1], a[2], a[3]);
			break;
		case SYS_brk:
			 ret = sys_brk(a[1]);
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
	/* printf("%d\n", ret); */
	sprintf(sret, "%d", ret);
}
