#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	
	for(int k = 0; k < 32; k++){
		if (ref_r->gpr[k]._32 != gpr(k)) return false;
		/* printf("%d %d\n", ref_r->gpr[k]._32,gpr(k)); */
	}
	/* return cpu.pc == pc; */
	/* printf("%x; %x ", pc,  cpu.pc); */
	return pc == cpu.pc;
}

void isa_difftest_attach() {
}
