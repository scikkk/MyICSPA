#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
    bool ret = true;	
	for(int k = 0; k < 32; k++){
		if (!difftest_check_reg(reg_name(k,1), cpu.pc, ref_r->gpr[k]._32, gpr(k))) 
			ret = false;
	}
	difftest_check_reg("pc", cpu.pc, pc, cpu.pc); 
	return ret;
}

void isa_difftest_attach() {
}
