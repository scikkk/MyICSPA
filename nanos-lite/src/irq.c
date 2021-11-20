#include <common.h>
void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
	// wk pc+4
	uint32_t tmp = 0;
	asm volatile("csrr  %0, mepc;"
			"addi %0, %0, 4;"
			"csrw mepc, %0"	   

			: 
			: "r"(tmp));	
	// wk pc+4
	switch (e.event) {
		case EVENT_NULL:
			Log("Unhandled EVENT_NULL!\n");
			break;
		case EVENT_YIELD: 
			Log("Unhandled EVENT_YIELD!\n");
			break;
		case EVENT_SYSCALL: 
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
			Log("Unhandled EVENT_IRQ_TIMER\n");
			break;
		case EVENT_IRQ_IODEV:
			Log("Unhandled EVENT_IRQ_IODEV!\n");
			break;
		default: panic("Unhandled event ID = %d", e.event);
	}

	return c;
}

void init_irq(void) {
	Log("Initializing interrupt/exception handler...");
	cte_init(do_event);
}
