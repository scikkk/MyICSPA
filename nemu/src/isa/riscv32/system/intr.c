#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * Then return the address of the interrupt/exception vector.
	 */
#ifdef CONFIG_ETRACE
	printf("0x%08x:ERROR: NO(cause):%d\tepc:0x%08x\tmtvec:0x%08x\tmstatus:0x%08x\n", cpu.pc, NO, epc, cpu.mtvec, cpu.mstatus);
	/* isa_reg_display(); */
#endif
	cpu.mepc = epc;
	cpu.mcause = NO;
	return cpu.mtvec;
}

word_t isa_query_intr() {
	return INTR_EMPTY;
}
