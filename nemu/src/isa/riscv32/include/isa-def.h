#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

typedef struct {
	struct {
		rtlreg_t _32;
	} gpr[32];

	vaddr_t pc;

	// wk 3.1
	rtlreg_t mtvec;
	rtlreg_t mepc, mstatus, mcause; // CSR regs
	rtlreg_t satp;
	bool INTR;


	// wk 3.1

} riscv32_CPU_state;

// decode
typedef struct {
	union {
		struct {
			uint32_t opcode1_0 : 2;
			uint32_t opcode6_2 : 5;
			uint32_t rd        : 5;
			uint32_t funct3    : 3;
			uint32_t rs1       : 5;
			int32_t  simm11_0  :12;
		} i;
		struct {
			uint32_t opcode1_0 : 2;
			uint32_t opcode6_2 : 5;
			uint32_t imm4_0    : 5;
			uint32_t funct3    : 3;
			uint32_t rs1       : 5;
			uint32_t rs2       : 5;
			int32_t  simm11_5  : 7;
		} s;
		struct {
			uint32_t opcode1_0 : 2;
			uint32_t opcode6_2 : 5;
			uint32_t rd        : 5;
			uint32_t imm31_12  :20;
		} u;
		// wk 2.1
		struct {
			uint32_t opcode1_0 : 2;
			uint32_t opcode6_2 : 5;
			uint32_t rd        : 5;
			uint32_t imm19_12  : 8;
			uint32_t imm11     : 1;
			uint32_t imm10_1   : 10;
			int32_t simm20    : 1;
		} j;
		// wk 2.1

		// wk 2.2

		struct {
			uint32_t opcode1_0 : 2;
			uint32_t opcode6_2 : 5;
			uint32_t rd    : 5;
			uint32_t funct3    : 3;
			uint32_t rs1       : 5;
			uint32_t rs2       : 5;
			uint32_t  funct7  : 7;
		} r;
		struct {
			uint32_t opcode1_0 : 2;
			uint32_t opcode6_2 : 5;
			uint32_t imm11    : 1;
			uint32_t imm4_1   : 4;
			uint32_t funct3    : 3;
			uint32_t rs1       : 5;
			uint32_t rs2       : 5;
			uint32_t  imm10_5  : 6;
			int32_t   simm12   : 1;
		} b;


		// wk 2.2
		uint32_t val;
	} instr;
} riscv32_ISADecodeInfo;


/* extern riscv32_CPU_state cpu; */
/* // enum { MMU_DIRECT, MMU_TRANSLATE, MMU_DYNAMIC }; */
/* int isa_mmu_check(vaddr_t vaddr, int len, int type) { */
/* 	if((cpu.satp >> 31) == 1){ */
/* 		return 1; // MMU_TRANSLATE; */
/* 	} */
/* 	else{ */
/* 		return 0; // MMU_DIRECT; */
/* 	} */
/* } */

#define isa_mmu_check(vaddr, len, type) (cpu.satp >> 31)
#endif
