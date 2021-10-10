def_EHelper(jal) {
	rtl_addi(s,ddest,&cpu.pc,0x4);
	rtl_j(s, cpu.pc + id_src1->simm);

}

def_EHelper(jalr) {
	rtl_addi(s, ddest,&cpu.pc, 0x4);
	rtl_addi(s, s0, dsrc1, id_src2->simm);
	rtl_jr(s, s0);
}


def_EHelper(beq) {
	if (interpret_relop(RELOP_EQ,*dsrc1,*dsrc2)){
		rtl_j(s, cpu.pc + id_dest->simm);
	}
}

def_EHelper(bne) {
	if (interpret_relop(RELOP_NE,*dsrc1,*dsrc2)){
		rtl_addi(s,s0,ddest,cpu.pc);
		rtl_jr(s,s0);
	}
}

def_EHelper(blt) {
	if (interpret_relop(RELOP_LT,*dsrc1,*dsrc2)){
		rtl_addi(s,s0,ddest,cpu.pc);
		rtl_jr(s,s0);
	}
}

def_EHelper(bge) {
	if (interpret_relop(RELOP_GE,*dsrc1,*dsrc2)){
		rtl_addi(s,s0,ddest,cpu.pc);
		rtl_jr(s,s0);	
	}
}

def_EHelper(bltu) {
	if (interpret_relop(RELOP_LTU,*dsrc1,*dsrc2)){
		rtl_addi(s,s0,ddest,cpu.pc);
		rtl_jr(s,s0);		
	}
}

def_EHelper(bgeu) {
	if (interpret_relop(RELOP_GEU,*dsrc1,*dsrc2)){
		rtl_addi(s,s0,ddest,cpu.pc);
		rtl_jr(s,s0);		
	}
}


