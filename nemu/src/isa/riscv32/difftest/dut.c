#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	bool ret = true;	
	for(int k = 0; k < 32; k++){
			ret = ret && difftest_check_reg(reg_name(k,0), pc, ref_r->gpr[k]._32, gpr(k));
	}
	ret = ret && difftest_check_reg("pc", pc, ref_r->pc, cpu.pc);
	/* isa_reg_display(); */
	/* return 1; */
	return ret;
}

void isa_difftest_attach() {
}
