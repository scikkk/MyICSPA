#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

extern void __am_get_cur_as(Context *c);
extern void __am_switch(Context *c);
Context* __am_irq_handle(Context *c) {
	/* printf("enter am irq handle %d:cause=%p\ta7=%d\n", __LINE__, c->mcause, c->GPR1); */
	/* printf("c->mepc=%p\n", c->mepc); */
	__am_get_cur_as(c);
	if (user_handler) {
		Event ev = {0};
		/* ev.event = c->mcause; */
		switch (c->mcause) {
			case EVENT_SYSCALL:ev.event = EVENT_SYSCALL;
							  if(c->GPR1 == -1) ev.event = EVENT_YIELD;
							   break;
			case 0x80000007 :ev.event=EVENT_IRQ_TIMER;break;
			default: printf("Unkonwn error num:%d\n", c->mcause); assert(0);
		}
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
	ctx->mstatus |= 0x80;
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
