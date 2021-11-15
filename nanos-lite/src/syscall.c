#include <common.h>
#include "syscall.h"
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
		  printf("%d\t%d\t%d\t%d\n\n", a[0],a[1],a[2],a[3]);
		  sys_exit(a[1]);
		  
		  break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
