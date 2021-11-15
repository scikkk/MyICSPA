// compute imm
def_EHelper(lui) {
	rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
	/* printf("%0x\n", cpu.pc+id_src1->imm); */
	rtl_addi(s, ddest, &cpu.pc, id_src1->imm);
	/* *ddest = cpu.pc+id_src1->imm; */
}

def_EHelper(addi) {
	rtl_addi(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(slti) {
	if (interpret_relop(RELOP_LT,*dsrc1 , id_src2->simm)){
		*ddest = 0x1;
	}
	else{
		*ddest = 0x0;
	}
}

def_EHelper(sltiu) {
	if (interpret_relop(RELOP_LTU,*dsrc1 , id_src2->imm)){
		*ddest = 0x1;
	}
	else{
		*ddest = 0x0;
	}
}

def_EHelper(xori) {
	rtl_xori(s,ddest ,dsrc1,id_src2->simm);
}

def_EHelper(ori) {

	rtl_ori(s,ddest ,dsrc1,id_src2->simm);
}

def_EHelper(andi) {
	rtl_andi(s,ddest ,dsrc1,id_src2->simm);
}

def_EHelper(slli) {
	rtl_slli(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(srli) {
	rtl_srli(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(srai) {
	rtl_srai(s,ddest,dsrc1,id_src2->simm);
}


// compute reg
def_EHelper(add) {
	rtl_add(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(sub) {
	rtl_sub(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(sll) {
	rtl_sll(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(slt) {
	if (interpret_relop(RELOP_LT, *dsrc1, *dsrc2)){
		*ddest = 0x1;
	}
	else{
		/* printf("*ddest=%u; *dsrc1=%d; *dsrc2=%d\n",*ddest,*dsrc1,*dsrc2); */
		*ddest = 0x0;
		/* printf("*ddest=%u; *dsrc1=%d; *dsrc2=%d\n",*ddest,*dsrc1,*dsrc2); */
	}
}
def_EHelper(sltu) {
	if (interpret_relop(RELOP_LTU, *dsrc1, *dsrc2)){
		*ddest = 0x1;
	}
	else{
		*ddest = 0x0;
	}
}

def_EHelper(xor) {
	rtl_xor(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(srl) {
	rtl_srl(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(sra) {
	rtl_sra(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(or) {

	rtl_or(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(and) {
	rtl_and(s,ddest ,dsrc1,dsrc2);
}


// muldiv
def_EHelper(mul) {
	rtl_mulu_lo(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(mulh) {
	rtl_muls_hi(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(mulhu) {
	rtl_mulu_hi(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(div) {
	rtl_divs_q(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(divu) {
	rtl_divu_q(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(rem) {
	rtl_divs_r(s,ddest ,dsrc1,dsrc2);
}

def_EHelper(remu) {
	rtl_divu_r(s,ddest ,dsrc1,dsrc2);
}


