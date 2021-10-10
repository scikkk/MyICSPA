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
if (interpret_relop(RELOP_EQ,*dsrc1,*dsrc2))	{
		rtl_j(s, cpu.pc + id_dest->simm);
	}
}

def_EHelper(bne) {
	if (*dsrc1 != *dsrc2)	{
		/* printf("%-10x\n%d\n",cpu.pc,id_dest->simm); */
		rtl_j(s, cpu.pc + id_dest->simm);
		/* rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm); */
	}
}

def_EHelper(blt) {
	if (*dsrc1 < *dsrc2)	{
		rtl_j(s, cpu.pc + id_dest->simm);
	}
}

def_EHelper(bge) {
	if (*dsrc1 >= *dsrc2)	{
		rtl_j(s, cpu.pc + id_dest->simm);
	}
}

def_EHelper(bltu) {
	if ((unsigned)*dsrc1 < (unsigned)*dsrc2)	{
		rtl_j(s, cpu.pc + id_dest->simm);
	}
}

def_EHelper(bgeu) {
	if ((unsigned)*dsrc1 >= (unsigned)*dsrc2)	{
		rtl_j(s, cpu.pc + id_dest->simm);
	}
}


