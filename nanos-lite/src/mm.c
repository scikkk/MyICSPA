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
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
