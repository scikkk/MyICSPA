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
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
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

  set_satp(kas.ptr);
  vme_enable = 1;

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
	assert((pt1_base & 0xfff) == 0);
	uintptr_t vpn1 = ((uintptr_t)va >> 22);
	uintptr_t vpn0 = ((uintptr_t)va >> 12) & 0x3ff;
	/* pa = (pg_table[va >> 12] & ~0xfff) | (va & 0xfff); */
	uintptr_t satp_ppn = pt1_base >> 12;
	uintptr_t pte1_addr = (satp_ppn << 12) | (vpn1 << 2);
	uintptr_t pte1 = (uintptr_t)pgalloc_usr(PGSIZE);
	assert((pte1 & 0xfff) == 0);
	pte1 |= 0x1;
	*(uintptr_t*)pte1_addr = pte1; 
	uintptr_t pte2_addr = (pte1 & ~0xfff) | (vpn0 << 2);
	printf("pte2_addr=%p\n", pte2_addr);
	*(uintptr_t*)pte2_addr = ((uintptr_t)pa & ~0xfff) | 0x1; 
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
	// wk 4.1
	Context *ctx =  (Context*)kstack.end - 1;;
	*ctx = (Context) { 0 };
	ctx->mepc = (uintptr_t)entry;
	return ctx;
	// wk 4.1
	/* return NULL; */
}
