word_t* csr(int32_t number){
	switch(number){
		case 0x300:
			return &cpu.mstatus;
		case 0x305:
			return &cpu.mtvec;
		case 0x341:
			return &cpu.mepc;
		case 0x342:
			return &cpu.mcause;
		default: assert(0);

	}
}
def_EHelper(ecall) {
  isa_raise_intr(1 , cpu.pc);
}

def_EHelper(csrrw) {
 *ddest = *(csr(id_src2->simm&0xfff));
 *(csr(id_src2->simm&0xfff)) = *dsrc1;
}
