#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
	void* ret = pf;
	pf += nr_page*4096;
	return ret;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
	void* ret = new_page(n >> 12);
	memset(ret, 0, n);
	return ret;
}
#endif

void free_page(void *p) {
	panic("not implement yet");
}

/* The brk() system call handler. */
#include <proc.h>
int mm_brk(uintptr_t brk) {
	if(brk > current->max_brk){
		printf("brk increase: %p --> %p\n", current->max_brk, brk);
		while(brk > current->max_brk){
			uintptr_t page_begin  = (uintptr_t)new_page(1);                                                                                                      
			/* map(&current->as , (void*)(brk&~0xfff), (void*)page_begin, 0); */
			map(&current->as , (void*)(brk), (void*)page_begin, 0);
		}
		current->max_brk = brk;
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
