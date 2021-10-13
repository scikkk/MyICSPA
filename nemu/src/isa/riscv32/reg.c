#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
	"$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
	"s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
	"a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
	"s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
	// added by wk
	printf("\n    NAME\t%%x\t%%d\n");
	printf("    %-6s\t0x%-8x\t%d\n","pc", cpu.pc,cpu.pc);
	for (int k = 0; k <32; k++){
		printf("    %-6s\t0x%-8x\t%d\n", regs[k], gpr(k), gpr(k));
	}		
	// added by wk
}

word_t isa_reg_str2val(const char *s, bool *success) {
	char ss[5];
	strcpy(ss,s);
	if(strcmp(ss,"pc")==0) return cpu.pc;
	for (int k = 0; ss[k] != '\0'; k++){
		if(s[k] >= 'A' && s[k] <= 'Z') ss[k]+=32;
	}
	for (int k = 0; k < 32; k++) {
		if (strcmp(ss,regs[k]) == 0) return gpr(k);
	}
	*success = false;
	return -1;
}
