#include "../local-include/reg.h"
:a
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

def_all_THelper();

static uint32_t get_instr(Decode *s) {
	return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
	void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

static def_DopHelper(i) {
	op->imm = val;
}

static def_DopHelper(r) {
	bool is_write = flag;
	static word_t zero_null = 0;
	op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
}

static def_DHelper(I) {
	decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
	decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, false);
	decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static def_DHelper(U) {
	decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
	decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static def_DHelper(S) {
	decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
	sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
	decode_op_i(s, id_src2, simm, false);
	decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}

// wk 2.2
static def_DHelper(R) {
	decode_op_r(s, id_src1, s->isa.instr.r.rs1, false);
	decode_op_r(s, id_src2, s->isa.instr.r.rs2, false);
	decode_op_r(s, id_dest, s->isa.instr.r.rd, true);
}
// wk 2.2

// wk 2.1
static def_DHelper(J) {
	sword_t simm = ((s->isa.instr.j.simm20<<20)|(s->isa.instr.j.imm19_12<<12)|s->isa.instr.j.imm11 << 11)|s->isa.instr.j.imm10_1<<1;
	// printf("0x%x\n",simm);
	decode_op_i(s, id_src1, simm, false);
	decode_op_r(s, id_dest, s->isa.instr.j.rd, true);
}
static def_DHelper(B) {
	decode_op_r(s, id_src1, s->isa.instr.b.rs1, false);
	decode_op_r(s, id_src2, s->isa.instr.b.rs2, false);
	sword_t simm = (s->isa.instr.b.simm12 << 12)|s->isa.instr.b.imm11 << 11|s->isa.instr.b.imm10_5 << 5|s->isa.instr.b.imm4_1 << 1;
	decode_op_i(s, id_dest, simm, false);
}
// wk 2.1



def_THelper(load) {
	def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", lb);
	def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", lh);
	def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
	def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", lbu);
	def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", lhu);

	return EXEC_ID_inv;
}

def_THelper(store) { 
	def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", sb);
	def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sh);
	def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);

	return EXEC_ID_inv;
}

// wk 2.1
def_THelper(compute) {
	def_INSTR_TAB("0000000 ????? ????? 000 ????? ????? ??", add);
	def_INSTR_TAB("0100000 ????? ????? 000 ????? ????? ??", sub);
	def_INSTR_TAB("0000000 ????? ????? 001 ????? ????? ??", sll);
	def_INSTR_TAB("0000000 ????? ????? 010 ????? ????? ??", slt);
	def_INSTR_TAB("0000000 ????? ????? 011 ????? ????? ??", sltu);
	def_INSTR_TAB("0000000 ????? ????? 100 ????? ????? ??", xor);
	def_INSTR_TAB("0000000 ????? ????? 101 ????? ????? ??", srl);
	def_INSTR_TAB("0100000 ????? ????? 101 ????? ????? ??", sra);
	def_INSTR_TAB("0000000 ????? ????? 110 ????? ????? ??", or);
	def_INSTR_TAB("0000000 ????? ????? 111 ????? ????? ??", and);
	// muldiv
	def_INSTR_TAB("0000001 ????? ????? 000 ????? ????? ??", mul);
	def_INSTR_TAB("0000001 ????? ????? 001 ????? ????? ??", mulh);
	def_INSTR_TAB("0000001 ????? ????? 011 ????? ????? ??", mulhu);
	def_INSTR_TAB("0000001 ????? ????? 100 ????? ????? ??", div);
	def_INSTR_TAB("0000001 ????? ????? 101 ????? ????? ??", divu);
	def_INSTR_TAB("0000001 ????? ????? 110 ????? ????? ??", rem);
	def_INSTR_TAB("0000001 ????? ????? 111 ????? ????? ??", remu);

	return EXEC_ID_inv;
}
// wk 2.1

// wk 2.2
def_THelper(computeimm) {
	def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", addi);
	def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", slti);
	def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", sltiu);
	def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", xori);
	def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", ori);
	def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", andi);
	// shamt
	def_INSTR_TAB("0000000 ????? ????? 001 ????? ????? ??", slli);
	def_INSTR_TAB("0000000 ????? ????? 101 ????? ????? ??", srli);
	def_INSTR_TAB("0100000 ????? ????? 101 ????? ????? ??", srai);

	return EXEC_ID_inv;
}
def_THelper(branch) {
	def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", beq);
	def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", bne);
	def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", blt);
	def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", bge);
	def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", bltu);
	def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", bgeu);
	return EXEC_ID_inv;
}

// wk 2.2

// wk 3.1

def_THelper(system) {
	def_INSTR_TAB("0000000 00000 00000 000 00000 ????? ??", ecall);
	def_INSTR_TAB("0011000 00010 00000 000 00000 ????? ??", mret);
	def_INSTR_TAB("0001000 00010 00000 000 00000 ????? ??", sret);
	def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", csrrw);
	def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", csrrs);
	return EXEC_ID_inv;
}
// wk 3.1

def_THelper(main) {
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load); // main
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I     , computeimm); // main
	// wk 3.1
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11100 11", I     , system); // main
	// wk 3.1
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U     , auipc);
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store); // main
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01100 11", R     , compute); // main
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U     , lui);
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11000 11", B     , branch); // main
	def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J     , jal);
	def_INSTR_IDTAB("??????? ????? ????? 000 ????? 11001 11", I     , jalr);
	def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
	return table_inv(s);
};

int isa_fetch_decode(Decode *s) {
	s->isa.instr.val = instr_fetch(&s->snpc, 4);
	int idx = table_main(s);
	return idx;
}
