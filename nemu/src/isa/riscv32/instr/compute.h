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
def_EHelper(srai) {
	rtl_srai(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(srli) {
	rtl_srli(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(slli) {
	rtl_slli(s,ddest,dsrc1,id_src2->simm);
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


// wk 2.1



// wk 2.2
def_EHelper(add) {
	rtl_add(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(sub) {
	rtl_sub(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(xor) {
	rtl_xor(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(or) {

	rtl_or(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(and) {
	rtl_and(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(sll) {
	rtl_sll(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(srl) {
	rtl_srl(s,ddest ,dsrc1,dsrc2);
}
def_EHelper(sra) {
	rtl_sra(s,ddest ,dsrc1,dsrc2);
}



def_EHelper(beq) {
	if (*dsrc1 == *dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm);
	}
}

def_EHelper(bne) {
	if (*dsrc1 != *dsrc2)	{
		/* printf("%-10x\n%d\n",cpu.pc,id_dest->simm); */

		s->dnpc = cpu.pc + id_dest->simm;
		/* rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm); */
	}
}

def_EHelper(blt) {
	if (*dsrc1 < *dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm);
	}
}

def_EHelper(bge) {
	if (*dsrc1 >= *dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm);
	}}
def_EHelper(bltu) {
	if ((unsigned)*dsrc1 < (unsigned)*dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm);
	}
}

def_EHelper(bgeu) {
	if ((unsigned)*dsrc1 >= (unsigned)*dsrc2)	{
		rtl_addi(s, &(s->dnpc),&cpu.pc, id_dest->simm);
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
def_EHelper(slt) {
	if (*dsrc1 < *dsrc2){
		*ddest = 1;
	}
	else
	{
		*ddest = 0;
	}
}
def_EHelper(sltu) {
	if ((unsigned)*dsrc1 < (unsigned)*dsrc2){
		*ddest = 1;
	}
	else
	{
		*ddest = 0;
	}
}

def_EHelper(rem) {
	printf("ddest: %d; dsrc1: %d; dsrc2: %d", *ddest, *dsrc1, *dsrc2);
	*ddest = (*dsrc1) % (*dsrc2);
}
def_EHelper(remu) {
	printf("ddest: %d; dsrc1: %d; dsrc2: %d", *ddest, *dsrc1, *dsrc2);
	*ddest = (unsigned)(*dsrc1) % (unsigned)(*dsrc2);
}
// wk 2.2

