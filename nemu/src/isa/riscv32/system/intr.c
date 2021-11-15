#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  printf("ERROR: NO:%d;\tepc:0x%08x\n", NO, epc);
  cpu.mepc = epc;
  cpu.mcause = NO;
  isa_reg_display();
  return cpu.mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
