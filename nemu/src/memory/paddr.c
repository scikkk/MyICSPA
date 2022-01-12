#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

#if   defined(CONFIG_TARGET_AM)
static uint8_t *pmem = NULL;
#else
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
#endif

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static word_t pmem_read(paddr_t addr, int len) {
	word_t ret = host_read(guest_to_host(addr), len);
	return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
	host_write(guest_to_host(addr), len, data);
}

void init_mem() {
#if   defined(CONFIG_TARGET_AM)
	pmem = malloc(CONFIG_MSIZE);
	assert(pmem);
#endif
#ifdef CONFIG_MEM_RANDOM
	uint32_t *p = (uint32_t *)pmem;
	int i;
	for (i = 0; i < (int) (CONFIG_MSIZE / sizeof(p[0])); i ++) {
		p[i] = rand();
	}
#endif
	Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]",
			(paddr_t)CONFIG_MBASE, (paddr_t)CONFIG_MBASE + CONFIG_MSIZE);
}

// wk 2.2 mtrace
bool mtrace_enable(paddr_t addr){
#ifdef CONFIG_MTRACE_COND 
  return MTRACE_COND;
#endif
  return true;
}
// wk 2.2 mtrace

word_t paddr_read(paddr_t addr, int len) {
	if (likely(in_pmem(addr))){

		word_t ret = pmem_read(addr, len);
/* #ifdef CONFIG_MTRACE */
/* 		static int count = 1; */
/* 		if (mtrace_enable(addr)) */
/* 		printf("[read ] count:%-6d begin:0x%-10x end:0x%-10x data:0x%-9x %-16d\n", count++, addr, addr+len, ret, ret); */
/* #endif */
		return ret;
	}
	MUXDEF(CONFIG_DEVICE, return mmio_read(addr, len),
			panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR ") at pc = " FMT_WORD,
				addr, CONFIG_MBASE, CONFIG_MBASE + CONFIG_MSIZE, cpu.pc));
}

void paddr_write(paddr_t addr, int len, word_t data) {
	if (likely(in_pmem(addr))) {
#ifdef CONFIG_MTRACE
		static int count = 1;
		word_t old = pmem_read(addr, len);
		if (mtrace_enable(addr)){
		printf("0x%-10x:[write] count:%-6d begin:0x%-10x end:0x%-10x old :0x%-9x %-16d new :0x%-9x %d\n",cpu.pc, count++, addr, addr+len,old,old, data, data);
		isa_reg_display();}
#endif 
		pmem_write(addr, len, data); 
		return;
	}
	MUXDEF(CONFIG_DEVICE, mmio_write(addr, len, data),
			panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR ") at pc = " FMT_WORD,
				addr, CONFIG_MBASE, CONFIG_MBASE + CONFIG_MSIZE, cpu.pc));
}
