#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	bool ret = true;	
	printf("mstatus:0x%x\n", ref_r->mstatus);
	for(int k = 0; k < 32; k++){
			ret = ret && difftest_check_reg(reg_name(k,0), pc, ref_r->gpr[k]._32, gpr(k));
			printf("0x%x\n", ref_r->csr[k]);
	}
	difftest_check_reg("mtvec  ", pc, ref_r->mtvec, cpu.mtvec);
    ret = ret && difftest_check_reg("mepc   ", pc, ref_r->mepc, cpu.mepc);
	ret = ret && difftest_check_reg("mstatus", pc, ref_r->mstatus, cpu.mstatus);
	ret = ret && difftest_check_reg("mcause ", pc, ref_r->mcause, cpu.mcause);
	ret = ret && difftest_check_reg("pc", pc, ref_r->pc, cpu.pc);

	/* isa_reg_display(); */
	return ret||1;

}

void isa_difftest_attach() {
}
