#include <common.h>

// wk
/* #include <unistd.h> */
/* #include <sys/stat.h> */
/* #include <setjmp.h> */
/* #include <sys/time.h> */
/* #include <assert.h> */
/* #include <time.h> */
#include "syscall.h"
// wk

int sys_execve(const char *pathname, char *const argv[], char *const envp[]);
int sys_brk(int32_t addr);
int sys_lseek(int fd, size_t offset, int whence);
int sys_close(int fd) ;
int sys_open(const char *path, int flags, int mode);
int sys_read(int fd, void *buf, int len);
void sys_exit(int code);
int sys_yield();
int sys_write(int fd, const void *buf, int count);

int sys_yield(){
	// wk 4.1
	/* printf("YIELD!!!\n"); */
	/* return (int)schedule(NULL); */
	// wk 4.1
	yield();
	return 0;
}

void sys_exit(int code){
	halt(0);
	sys_execve("/bin/nterm", NULL, NULL);
	halt(code);
}

int fs_write(int fd, const void *buf, int len);
int sys_write(int fd, const void *buf, int count){
	/* count = 200; */
	/* if (fd == 1 || fd == 2){ */
	/* 	for(int k = 0; k < count; k++){ */
	/* 		putch(*(((char*)buf)+k)); */
	/* 	} */ 
	/* 	return count; */
	/* } */
	/* printf("count: %d\n", count); */
	return fs_write(fd, buf, count);
}

int fs_read(int fd, void *buf, int len);
int sys_read(int fd, void *buf, int len){
	int ret =  fs_read(fd, buf, len);
	/* assert(ret); */
	/* printf("ret=%d\n", ret); */
	return ret;
}


int fs_open(const char *pathname, int flags, int mode);
int sys_open(const char *path, int flags, int mode) {
	int ret = fs_open(path, flags, mode);
	/* printf("open:%s\n", path); */
	return ret;
}


int fs_close(int fd);
int sys_close(int fd) {
	int ret = fs_close(fd);
	return ret;
}

int fs_lseek(int fd, int offset, int whence);
int sys_lseek(int fd, size_t offset, int whence) {
	/* fd = 12; */
	assert(fd > 2 );
	return fs_lseek(fd, offset, whence);
}

extern int mm_brk(uintptr_t brk);
int sys_brk(int32_t addr){
	/* printf("brk_addr:%p\n", addr); */
	return mm_brk(addr);
}

#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
extern void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);
extern void switch_boot_pcb();
int sys_execve(const char *pathname, char *const argv[], char *const envp[]){
	/* naive_uload(NULL, pathname); */
	if(fs_open(pathname, 0, 0) == -2){
		return -2;
	}
	/* printf("sys: argv=%p\tenvp=%p\n", argv, envp); */
	/* printf("sys: argv[0]=%p\tenvp[0]=%p\n", argv[0], envp[0]); */
	context_uload(current, pathname, argv, envp);
	/* printf("pathname:%s\n", pathname); */
	switch_boot_pcb();
	yield();
	return 0;
}
struct timeval{
	int32_t tv_sec;
	int32_t tv_usec;
};

int sys_gettimeofday(struct timeval *tv, void *tz) {
	/* return gettimeofday(tv, tz); */
	/* printf("enter sys,%d\n", sizeof(tv->tv_usec)); */
	/* printf("sys_usec: %d\n", (int)tv->tv_usec); */
	/* printf("tv:%p\ttz:%p\n", tv, tz); */
	assert(tz == NULL);
	if(tv == NULL){
		return -1;
	}
	uint64_t us = io_read(AM_TIMER_UPTIME).us;
	tv->tv_usec = (long)us % 1000000;
	/* printf("sys_usec: %d\n", (int)tv->tv_usec); */
	/* tv->tv_usec = 520; */
	/* printf("sys_usec: %d\n", (int)tv->tv_usec); */
	tv->tv_sec = us / 1000000;
	/* printf("sys_sec: %d\n", (int)tv->tv_sec); */
	/* printf("exit sys\n"); */
	/* printf("%d\t%d\n", tv->tv_sec, tv->tv_usec); */
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
	[3] = {"/dev/events",0, 0},
	[4] = {"/proc/dispinfo", 0, 0},
	[5] = {"/dev/fb", 0,  0},
#include "files.h"
};

void strace(Context *c, char* ret) {
	char  oneline[1280];
	char whence[3][20] = {"SEEK_SET", "SEEK_CUR", "SEEK_END"};
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
			sprintf(oneline, "sys_open(%s, %d, %d)", (char*)a[1], (int)a[2], (int)a[3]);
			break;
		case SYS_read:
			sprintf(oneline, "sys_read(%d(%s), %p, %d)", (int)a[1], file_table[a[1]].name, (void*)a[2], (int)a[3]);
			break;
		case SYS_write:
			strncpy(tmp, (char*)a[2], 9);
			tmp[strlen((char*)a[2])] = '\0';
			tmp[9] = '\0';
			if(strlen((char*)a[2]) > 9){
				strcat(tmp, "...");
			}
			sprintf(oneline, "sys_write(%d(%s), \"%s\", %d)", (int)a[1], file_table[a[1]].name, tmp, (int)a[3]);
			break;
		case SYS_kill:
			strcpy(oneline, "sys_kill");
			break;
		case SYS_getpid:
			strcpy(oneline, "sys_getpid");
			break;
		case SYS_close:
			sprintf(oneline, "sys_close(%d(%s))", (int)a[1], file_table[a[1]].name);
			break;
		case SYS_lseek:
			sprintf(oneline, "sys_lseek(%d(%s), %d, %s)", (int)a[1], file_table[a[1]].name, (int)a[2], whence[(int)a[3]]);
			break; 
		case SYS_brk:
			sprintf(oneline, "sys_brk(%p)", (int)a[1]);
			break;
		case SYS_fstat:
			strcpy(oneline, "sys_fstat");
			break;
		case SYS_time:
			strcpy(oneline, "time");
			break;
		case SYS_signal:
			strcpy(oneline, "sys_signal");
			break;
		case SYS_execve:
			strcpy(oneline, "sys_execve");
			break;
		case SYS_gettimeofday:
			sprintf(oneline, "sys_gettimeofday(%p, %p)", (void*)a[1], (void*)a[2]);
			break;
		default: assert(0);
	}
	static int no = 1;
	printf("[STRACE] %d %s = %s\n\n", no++, oneline, ret);
}



void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

	int strace_ret = 0;
	/* char sret[20]="ret"; */
	switch (a[0]) {
		case SYS_exit:
			/* strace(c, "void"); */
			/* printf("%d\t%d\t%d\t%d\n\n", a[0],a[1],a[2],a[3]); */
			sys_exit(a[1]);
			break;
		case SYS_yield:
			strace_ret = sys_yield();
			/* printf("yield=%p\n", strace_ret); */
			break;
		case SYS_open:
			strace_ret = sys_open((char*)a[1], a[2], a[3]);
			break;
		case SYS_read:
			strace_ret = sys_read(a[1], (void*)a[2], a[3]);
			/* printf("strace_ret=%d\n", strace_ret); */
			break;
		case SYS_write:
			/* printf("write_a[3]:%d\n", a[3]); */
			strace_ret = sys_write(a[1], (void*)a[2], a[3]);
			break;
		case SYS_kill:
			assert(0);
			break;
		case  SYS_getpid:
			assert(0);
			break;
		case SYS_close:
			strace_ret = sys_close(a[1]);
			break;
		case SYS_lseek:
			strace_ret = sys_lseek(a[1], a[2], a[3]);
			break;
		case SYS_brk:
			strace_ret = sys_brk(a[1]);
			break;
		case SYS_fstat:
			assert(0);
			break;
		case SYS_time:
			assert(0);
			break;
		case SYS_signal:
			assert(0);
			break;
		case SYS_execve:
			strace_ret = sys_execve((char*)a[1], (char*const*)a[2], (char*const*)a[3]);
			break;
		case  SYS_fork:
			assert(0);
			break;
		case  SYS_link:
			assert(0);
			break;
		case  SYS_unlink:
			assert(0);
			break;
		case  SYS_wait:
			assert(0);
			break;
		case  SYS_times:
			assert(0);
			break;
		case SYS_gettimeofday:
			strace_ret = sys_gettimeofday((struct timeval*)a[1], (void*)a[2]);
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
	/* printf("strace_ret=%d\n", strace_ret); */
	/* sprintf(sret, "%d", strace_ret); */
	/* strace(c, sret); */
	c->GPRx = strace_ret;
	/* printf("GPRx:%d\n", c->GPRx); */
}
