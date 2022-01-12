#include <isa.h>
#include <memory/paddr.h>


word_t vaddr_read(vaddr_t addr, int len) {
	switch(isa_mmu_check(addr, len, 0)){
		case MMU_TRANSLATE:
			/* printf("satp = 0x%x\n", cpu.satp); */
			addr = isa_mmu_translate(addr, len, 0);
		case MMU_DIRECT:
			break;
		default : assert(0);
	}
	return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
	switch(isa_mmu_check(addr, len, 0)){
		case MMU_TRANSLATE:
			/* printf("satp = 0x%x\n", cpu.satp); */
			addr = isa_mmu_translate(addr, len, 0);
		case MMU_DIRECT:
			paddr_write(addr, len, data);
			break;
		default : assert(0);
	}
}

word_t vaddr_ifetch(vaddr_t addr, int len) {
	return vaddr_read(addr, len);
	/* return paddr_read(addr, len); */
}
