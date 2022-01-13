#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
	/* if((vaddr > 0x80000000)) return vaddr; */
	paddr_t paddr;
	assert(type == 0);
	assert((cpu.satp >> 31) == 1);

	vaddr_t vpn1 = (vaddr >> 22);
	vaddr_t vpn0 = (vaddr >> 12) & 0x3ff;

	/* pa = (pg_table[va >> 12] & ~0xfff) | (va & 0xfff); */

	vaddr_t satp_ppn = cpu.satp & 0x3fffff;
	paddr_t pte1_addr = (satp_ppn << 12) | (vpn1 << 2);
	/* printf("pte1_addr=0x%u\n", pte1_addr); */
	paddr_t pte1 = paddr_read(pte1_addr, 4);
	if((pte1 & 0x1) == 0){
		isa_reg_display();
		printf("mmu vaddr=0x%x\n", vaddr);
		printf("mmu satp=0x%x\n", cpu.satp);
		printf("mmu pte1_addr=0x%x\n", pte1_addr);
		printf("mmu pte1=0x%x\n", pte1);
	}
	assert((pte1 & 0x1) == 1);

	paddr_t pte2_addr = (pte1 & ~0xfff) | (vpn0 << 2);
	paddr_t pte2 = paddr_read(pte2_addr, 4);
	if((pte2 & 0x1) == 0){
		isa_reg_display();
		printf("mmu vaddr=0x%x\n", vaddr);
		printf("mmu satp=0x%x\n", cpu.satp);
		printf("mmu pte2_addr=0x%x\n", pte2_addr);
		printf("mmu pte2=0x%x\n", pte2);
	}
	assert((pte2 & 0x1) == 1);
	paddr = (pte2 & ~0xfff) | (vaddr & 0xfff); 
	/* if(paddr != vaddr){ */
		/* printf("mmu: vaddr=0x%x ---> paddr=0x%x\n", vaddr, paddr); */
	/* } */
	/* assert(paddr == vaddr); */
	return paddr;
	/* return MEM_RET_FAIL; */
}
