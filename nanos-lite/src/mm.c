#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
	/* static int no = 1; */
	void* ret = pf;
	memset(ret, 0, nr_page*4096);
	pf += nr_page*4096;
	/* printf("new_page(%d):%d=[ %p , %p ]\n", nr_page, no++, ret, pf); */
	return ret;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
	void* ret = new_page(n >> 12);
	memset(ret, 0, n);
	/* printf("pg_alloc(%d*4096):[ %p , %p ]\n", n/4096, ret, pf); */
	return ret;
}
#endif

void free_page(void *p) {
	panic("not implement yet");
}

/* The brk() system call handler. */
#include <proc.h>
int mm_brk(uintptr_t brk) {
	/* printf("current=%p\tcurrent->as=%p\n", current, current->as); */
	brk &= ~0xfff;
	if(brk > current->max_brk){
		/* printf("maxbrk increase: %p --> %p\n", current->max_brk, brk); */
		uintptr_t old_brk = brk;
		while(brk > current->max_brk){
			uintptr_t page_begin  = (uintptr_t)new_page(1);                                                                                                      
			/* printf("hit line %d\n", __LINE__); */
			/* map(&current->as , (void*)(brk&~0xfff), (void*)page_begin, 0); */
			/* current->as.ptr = current->cp->pdir; */
			map(&current->as , (void*)(brk), (void*)page_begin, 0);
			/* printf("hit line %d\n", __LINE__); */
			brk -= 0x1000;
			/* printf("hit line %d\n", __LINE__); */
		}
		current->max_brk = old_brk;
	}
	return 0;
}

void init_mm() {
	pf = (void *)ROUNDUP(heap.start, PGSIZE);
	Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
	vme_init(pg_alloc, free_page);
#endif
}
