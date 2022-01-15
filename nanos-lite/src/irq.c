#include <common.h>
void do_syscall(Context *c);
Context* schedule(Context *prev) ;

static Context* do_event(Event e, Context* c) {
	/* printf("do_event:e.event=%p\n", e.event); */
	switch (e.event) {
		case EVENT_NULL:
			Log("Unhandled EVENT_NULL!\n");
			break;
		case EVENT_YIELD: 
			c->mepc += 4;
			/* Log("enter EVENT_YIELD!!!"); */
			c = schedule(c);
			/* Log("exit EVENT_YIELD!!!"); */
			break;
		case EVENT_SYSCALL: 
			c->mepc += 4;
			/* Log("\nUnhandled EVENT_SYSCALL!\n"); */
			do_syscall(c);
			break;
		case EVENT_PAGEFAULT: 
			Log("Unhandled EVENT_PAGEFAULT!\n");
			break;
		case EVENT_ERROR:
			Log("Unhandled EVENT_ERROR!\n");
			break;
		case EVENT_IRQ_TIMER:
			/* Log("enter EVENT_IRQ_TIMER!!!"); */
			c = schedule(c);
			/* Log("exit EVENT_IRQ_TIMER!!!"); */
			break;
		case EVENT_IRQ_IODEV:
			Log("Unhandled EVENT_IRQ_IODEV!\n");
			break;
		default: panic("Unhandled event ID = %d", e.event);
	}
	/* printf("do_event=%p\n", c); */
	return c;
}

void init_irq(void) {
	Log("Initializing interrupt/exception handler...");
	cte_init(do_event);
}
