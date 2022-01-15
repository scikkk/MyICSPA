#include <isa.h>

enum {
	EVENT_NULL = 0,
	EVENT_YIELD, EVENT_SYSCALL, EVENT_PAGEFAULT, EVENT_ERROR,
	EVENT_IRQ_TIMER, EVENT_IRQ_IODEV,
};

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * Then return the address of the interrupt/exception vector.
	 */
#ifdef CONFIG_ETRACE
	char type[20];
	switch (NO) {
		case EVENT_NULL: strcpy(type,"EVENT_NULL");break;
		case EVENT_YIELD: strcpy(type, "EVENT_YIELD"); break;
		case EVENT_SYSCALL:strcpy(type, "EVENT_SYSCALL"); break;
		case EVENT_PAGEFAULT:strcpy(type,"EVENT_PAGEFAULT");break;
		case EVENT_ERROR:strcpy(type, "EVENT_ERROR");break;
		case EVENT_IRQ_TIMER :strcpy(type, "EVENT_IRQ_TIMER");break;
		case EVENT_IRQ_IODEV:strcpy(type, "EVENT_IRQ_IODEVi");break;
		default: assert(0);
	}
	printf("[ETRACE] 0x%08x:ERROR: NO(cause):%d(%s); epc:0x%08x; mtvec:0x%08x; mstatus:0x%08x\n", cpu.pc, NO,type, epc, cpu.mtvec, cpu.mstatus);
#endif
	/* isa_reg_display(); */
	cpu.mepc = epc;
	cpu.mcause = NO;
	if((cpu.mstatus & 0x8) == 0){
		cpu.mstatus &= ~0x80;
	}
	else{
		cpu.mstatus |= 0x80;
	}
	cpu.mstatus &= ~0x8;
	return cpu.mtvec;
}


#define IRQ_TIMER 0x80000007  // for riscv32

word_t isa_query_intr() {
	/* return INTR_EMPTY; */
	if ((cpu.mstatus & 0x8) && cpu.INTR){
		cpu.INTR = false;
		return IRQ_TIMER;
	}
	return INTR_EMPTY;
}
