#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
	NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
	uintptr_t mode = 1ul << (__riscv_xlen - 1);
	asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
	/* printf("set_satp=%p\n", mode | ((uintptr_t)pdir >> 12)); */
}

static inline uintptr_t get_satp() {
	uintptr_t satp;
	asm volatile("csrr %0, satp" : "=r"(satp));
	/* printf("get_satp=%p\n", satp); */
	return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
	pgalloc_usr = pgalloc_f;
	pgfree_usr = pgfree_f;

	kas.ptr = pgalloc_f(PGSIZE);

	int i;
	for (i = 0; i < LENGTH(segments); i ++) {
		void *va = segments[i].start;
		for (; va < segments[i].end; va += PGSIZE) {
			map(&kas, va, va, 0);
		}
	}

	/* printf("before set satp!\n"); */
	set_satp(kas.ptr);
	vme_enable = 1;
	/* printf("vme_init finished!\n"); */
	return true;
}

void protect(AddrSpace *as) {
	PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
	as->ptr = updir;
	as->area = USER_SPACE;
	as->pgsize = PGSIZE;
	// map kernel space
	memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
	c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
	if (vme_enable && c->pdir != NULL) {
		set_satp(c->pdir);
	}
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
	assert(prot == 0);
	uintptr_t pt1_base = (uintptr_t)as->ptr;
	if((uintptr_t)va < 0x50000000)
	printf("map:pt1base=%p va=%p --> pa=%p\n", pt1_base, va, pa);
	/* if((uintptr_t)va < 0x50000000) */
	/* printf("map as->ptr=%p\n", as->ptr); */
	assert((pt1_base & 0xfff) == 0);
	uintptr_t vpn1 = ((uintptr_t)va >> 22);
	uintptr_t vpn0 = ((uintptr_t)va >> 12) & 0x3ff;
	/* pa = (pg_table[va >> 12] & ~0xfff) | (va & 0xfff); */
	uintptr_t satp_ppn = pt1_base >> 12;
	uintptr_t pte1_addr = (satp_ppn << 12) | (vpn1 << 2);
	/* if((uintptr_t)va < 0x50000000) */
	/* printf("map hit line: %d\n", __LINE__); */
	/* printf("pte1_addr=%p\n", pte1_addr); */
	uintptr_t pte1 = *(uintptr_t*)pte1_addr;
	/* if((uintptr_t)va < 0x50000000) */
	/* printf("map hit line: %d\n", __LINE__); */
	if((pte1 & 0x1) == 0){
		pte1 = (uintptr_t)pgalloc_usr(PGSIZE);
		assert((pte1 & 0xfff) == 0);
		pte1 |= 0x1;
		*(uintptr_t*)pte1_addr = pte1; 
	}
	uintptr_t pte2_addr = (pte1 & ~0xfff) | (vpn0 << 2);

	/* printf("pte2_addr=%p\n", pte2_addr); */
	*(uintptr_t*)pte2_addr = ((uintptr_t)pa & ~0xfff) | 0x1; 
	/* if((uintptr_t)va == 0x00000000) */
	/* printf("map: pte1addr=%p\tpte2addr=%p\n", pte1_addr, pte2_addr); */
	/* if((uintptr_t)va == 0x00000000) */
	/* printf("map: pte1=%p\tpte2=%p\n", pte1, *(uintptr_t*)pte2_addr); */
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
	// wk 4.1
	Context *ctx =  (Context*)kstack.end - 1;;
	*ctx = (Context) { 0 };
	ctx->mepc = (uintptr_t)entry;
	/* printf("hit there line: %d\n", __LINE__); */
	/* printf("ucontext pdir=%p\n", as->ptr); */
	ctx->pdir = as->ptr;
	/* printf("hit there line: %d\n", __LINE__); */
	return ctx;
	// wk 4.1
	/* return NULL; */
}
