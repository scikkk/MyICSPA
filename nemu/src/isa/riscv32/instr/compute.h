def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}


// wk 2.1
def_EHelper(auipc) {
	rtl_addi(s, ddest, &cpu.pc, id_src1->imm);
	//  printf("%-10x\n%-10x",cpu.pc,*ddest);
}

def_EHelper(jal) {
	rtl_addi(s,ddest,&cpu.pc,0x4);
	rtl_sext(s,&id_src1->imm,&id_src1->imm,11);
	rtl_addi(s, &(s->dnpc),&cpu.pc, id_src1->imm);
	// printf("0x%-10x\n",id_src1->imm);
}

def_EHelper(jalr) {
	rtl_addi(s, ddest,&cpu.pc, 0x4);
	rtl_sext(s,&id_src2->imm,&id_src2->imm,20);
	rtl_addi(s,&(s->dnpc),dsrc1,id_src2->imm);
}
def_EHelper(addi) {
	rtl_addi(s,ddest,dsrc1,id_src2->imm);
}
// wk 2.1



// wk 2.2
def_EHelper(add) {
	*ddest = *dsrc1 + *dsrc2;
}
def_EHelper(bge) {
     if (*dsrc1 >= *dsrc2)	{
	rtl_sext(s,&id_src1->imm,&id_src1->imm,11);
	rtl_addi(s, &(s->dnpc),&cpu.pc, id_src1->imm);
	// printf("0x%-10x\n",id_src1->imm);

}
}
// wk 2.2
