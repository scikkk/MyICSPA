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
	rtl_addi(s, &(s->dnpc),&cpu.pc, id_src1->simm);

}

def_EHelper(jalr) {
	rtl_addi(s, ddest,&cpu.pc, 0x4);
	rtl_addi(s,&(s->dnpc),dsrc1,id_src2->simm);
}
def_EHelper(addi) {
	rtl_addi(s,ddest,dsrc1,id_src2->simm);
}
// wk 2.1



// wk 2.2
def_EHelper(add) {
	*ddest = *dsrc1 + *dsrc2;
}
def_EHelper(beq) {
	if (*dsrc1 == *dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, *ddest);
	}
}

def_EHelper(bne) {
	if (*dsrc1 != *dsrc2)	{
		/* printf("%-10x\n%d\n",cpu.pc,*ddest); */

		rtl_addi(s, &(s->dnpc),&cpu.pc, *ddest);
	}
}

def_EHelper(blt) {
	if (*dsrc1 < *dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, *ddest);
	}
}

def_EHelper(bge) {
	if (*dsrc1 >= *dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, *ddest);
	}}
def_EHelper(bltu) {
	if ((unsigned)*dsrc1 < (unsigned)*dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, *ddest);
	}
}

def_EHelper(bgeu) {
	if ((unsigned)*dsrc1 >= (unsigned)*dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, *ddest);
	}
}


def_EHelper(slti) {
	if (*dsrc1 < id_src2->simm){
		*ddest = 1;
	}
	else
	{
		*ddest = 0;
	}
}
def_EHelper(sltiu) {
	if (*dsrc1 < id_src2->imm){
		*ddest = 1;
	}
	else
	{
		*ddest = 0;
	}
}
// wk 2.2
