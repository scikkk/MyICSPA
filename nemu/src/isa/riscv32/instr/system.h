word_t* csr(int32_t number){
	switch(number){
  		case 0x300:
			/* printf("mstatus\n"); */
			return &cpu.mstatus;
		case 0x305:
			/* printf("mtvec\n"); */
			return &cpu.mtvec;
		case 0x341:
			/* printf("mepc\n"); */
			return &cpu.mepc;
		case 0x342:
			/* printf("mcause\n"); */
			return &cpu.mcause;
		case 0x180:
 //			printf("0x%x:!!!nemu satp=0x%x!!!\n", cpu.pc, cpu.satp);
 			return &cpu.satp;
		default: assert(0);

	}
}
def_EHelper(ecall) {
	/* if (gpr(17) == -1){ */
	
  /* isa_raise_intr(1 , cpu.pc); */
	/* }else{ */
	isa_raise_intr(11, cpu.pc);
	/* } */
  rtl_j(s, cpu.mtvec);
}

def_EHelper(mret) {
  rtl_j(s, cpu.mepc);
	if((cpu.mstatus & 0x80) == 0){
		cpu.mstatus &= ~0x8;
	}
	else{
		cpu.mstatus |= 0x8;
	}
	cpu.mstatus |= 0x80;
  /* rtl_j(s, cpu.mepc); */
}
def_EHelper(csrrw) {
 *ddest = *(csr(id_src2->simm));
 *(csr(id_src2->simm)) = *dsrc1;
}

def_EHelper(csrrs) {
 *ddest = *(csr(id_src2->simm));
 rtl_or(s, csr(id_src2->simm), csr(id_src2->simm), dsrc1);
/* *(csr(id_src2->simm&0xfff)) |= *dsrc1; */
}
