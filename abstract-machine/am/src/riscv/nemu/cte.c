#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

extern void __am_get_cur_as(Context *c);
extern void __am_switch(Context *c);
Context* __am_irq_handle(Context *c) {
	printf("enter am irq handle %d:cause=%p\n", __LINE__, c->mcause);
	/* printf("c->mepc=%p\n", c->mepc); */
	__am_get_cur_as(c);
	if (user_handler) {
		Event ev = {0};
		/* ev.event = c->mcause; */
		/* switch (c->mcause) { */
		/* 	case EVENT_NULL: ev.event = EVENT_NULL;break; */
		/* 	case EVENT_YIELD: ev.event = EVENT_YIELD; break; */
		/* 	case EVENT_SYSCALL:ev.event = EVENT_SYSCALL; break; */
		/* 	case EVENT_PAGEFAULT:ev.event = EVENT_PAGEFAULT;break; */
		/* 	case EVENT_ERROR:ev.event=EVENT_ERROR;break; */
		/* 	case EVENT_IRQ_TIMER :ev.event=EVENT_IRQ_TIMER;break; */
		/* 	case EVENT_IRQ_IODEV:ev.event=EVENT_IRQ_IODEV;break; */
		/* 	default: printf("Unkonwn error num:%d\n", c->mcause); assert(0); */
		/* } */
		/* printf("x0:%d\n",c->gpr[0]); */
		/* printf("mstatus:%d\nmepc:%d\nmcause:%d\n", c->mstatus,c->mepc,c->mcause); */

		/* printf("__am_irq_handle=%p\n", c); */
		/* if(c->mcause != 11) */ 
		switch(c->GPR1){
			case 0:case 1:case 2:case 3: case 4:case 5: case 6:case 7:
			case 8:case 9:case 10:case 11:case 12: case 13:case 14: case 15: case 16: case 17:
			case 18: case 19: ev.event = EVENT_SYSCALL;break;
			default: ev.event = EVENT_YIELD;
		}
		if(c->mcause == 0x80000007) 
			ev.event = EVENT_IRQ_TIMER;
		c = user_handler(ev, c);
		assert(c != NULL);
		/* assert(0); */
	}
	__am_switch(c);
	/* printf("c->mepc=%p\n", c->mepc); */
	/* printf("exit am_irqhandle irqhandle ret c = %p\n", c); */
	return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
	/* printf("user_handler=%p\thandler=%p\n", user_handler, handler); */
	// initialize exception entry
	asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

	// register event handler
	/* printf("user_handler=%p\thandler=%p\n", user_handler, handler); */
	assert(user_handler == NULL);
	user_handler = handler;

	return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
	// wk 4.1
	Context *ctx =  (Context*)kstack.end - 1;;
	/* printf("kcontex ctx=%p\n", ctx); */
	*ctx = (Context) { 0 };
	ctx->mepc = (uintptr_t)entry;
	ctx->mstatus |= 0x8;
	/* printf("kcontex!!! arg=%p\n", arg); */
	ctx->GPR2 = (uintptr_t)arg;
	ctx->pdir = NULL;
	/* ctx->GPR2 = (uintptr_t)entry; */
	return ctx;
	// wk 4.1
	/* return NULL; */
}

void yield() {
	asm volatile("li a7, -1; ecall");
}

bool ienabled() {
	return false;
}

void iset(bool enable) {
}
