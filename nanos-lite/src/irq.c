#include <common.h>
void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
	switch (e.event) {
		case EVENT_NULL:
			Log("\nUnhandled EVENT_YILED!\n");
			break;
		case EVENT_YIELD: 
			Log("\nUnhandled EVENT_YIELD!\n");
			break;
		case EVENT_SYSCALL: 
			/* Log("\nUnhandled EVENT_SYSCALL!\n"); */
			do_syscall(c);
			break;
		case EVENT_PAGEFAULT: 
			Log("\nUnhandled EVENT_PAGEFAULT!\n");
			break;
		case EVENT_ERROR:
			Log("\nUnhandled EVENT_ERROR!\n");
			break;
		case EVENT_IRQ_TIMER:
			Log("\nUnhandled EVENT_IRQ_TIMER\n");
			break;
		case EVENT_IRQ_IODEV:
			Log("\nUnhandled EVENT_IRQ_IODEV!\n");
			break;
		default: panic("Unhandled event ID = %d", e.event);
	}

	return c;
}

void init_irq(void) {
	Log("Initializing interrupt/exception handler...");
	cte_init(do_event);
}
