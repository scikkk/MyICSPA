#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_X86__)
# define EXPECT_TYPE EM_X86_64  
#elif defined(__ISA_MIPS32__)
# define EXPECT_TYPE EM_MIPS 
#elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
# define EXPECT_TYPE EM_RISCV 
#else
# error Unsupported ISA
#endif


/* // wk 3.2 */
/* // 从ramdisk中`offset`偏移处的`len`字节读入到`buf`中 */
/* size_t ramdisk_read(void *buf, size_t offset, size_t len); */

/* // 把`buf`中的`len`字节写入到ramdisk中`offset`偏移处 */
/* size_t ramdisk_write(const void *buf, size_t offset, size_t len); */

/* // 返回ramdisk的大小, 单位为字节 */
/* size_t get_ramdisk_size(); */
/* // wk 3.2 */


// wk 3.3
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
// wk 3.3

static uintptr_t MAX_BRK = 0x40000000;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

static uintptr_t loader(PCB *pcb, const char *filename) {
	/* TODO(); */
	int fd = fs_open(filename, 0, 0);
	Elf_Ehdr header;
	fs_read(fd, &header, 52);	
	/* printf("line:%dloader: fd:%d\n", __LINE__, fd); */
	assert(*(uint32_t *)header.e_ident == 0x464c457f);
	/* printf("%d\n%d\n", header.e_machine, EXPECT_TYPE); */
	assert(header.e_machine == EXPECT_TYPE);
	Elf_Phdr ph;
	unsigned off = header.e_phoff;
	for(int k = 0; k < header.e_phnum; k++){
		/* printf("\nloader %d\n", k); */
		fs_lseek(fd, off, SEEK_SET);
		fs_read(fd, &ph, header.e_phentsize);
		off += header.e_phentsize;
		printf("vaddr=%p\tpaddr=%\tfilesz=%p\tmemsz=%p\n", ph.p_vaddr, ph.p_paddr, ph.p_filesz, ph.p_memsz);
		if(ph.p_type == PT_LOAD){
			uintptr_t load_begin = 0;
			for(int pagenum = 0; ph.p_memsz + ph.p_vaddr >= (ph.p_vaddr & ~0xfff) + (pagenum)*0x1000; pagenum++){
				uintptr_t page_begin  = (uintptr_t)new_page(1);
				printf("loader: pagenum=%d\tva=%p\tpa=%p\n", pagenum, ph.p_vaddr + 0x1000*pagenum, page_begin);
				if(pagenum == 0) load_begin = page_begin;
				map(&pcb->as , (void*)(ph.p_vaddr&~0xfff) + 0x1000*pagenum, (void*)page_begin, 0);
				MAX_BRK = ((ph.p_vaddr) + 0x1000*pagenum) & ~0xfff;
			}
			fs_lseek(fd, ph.p_offset, SEEK_SET);
			fs_read(fd, (void*)((uintptr_t)ph.p_vaddr & 0xfff) + load_begin, ph.p_filesz);
			printf("loder begin%p\n", ((uintptr_t)ph.p_vaddr & 0xfff) + load_begin);
			printf("line%d:loader: vaddr=%p --- %p\n", __LINE__, ph.p_vaddr, ph.p_vaddr + ph.p_memsz);
		} 
	}
	return header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
	uintptr_t entry = loader(pcb, filename);
	Log("Jump to entry = %p", entry);
	((void(*)())entry) ();
}

// wk 4.1
void context_kload(PCB *pcb, void (*entry)(void *), void *arg){
	Area kstack = {pcb, pcb+1};
	/* printf("start=%p\tend=%p\n", kstack.start, kstack.end); */
	pcb->cp = kcontext(kstack, entry, arg);
	/* printf("%p-4=%p\n", entry, entry-4); */
	/* printf("kcontext-ret=%p\n", pcb->cp); */
}
// wk 4.1


// wk 4.1
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]){
	protect(&pcb->as);
	
	/* printf("context_uload:filename=%s\n", filename); */
	/* printf("argv=%p\tenvp=%p\n", argv, envp); */
	Area kstack = {pcb, pcb+1};
	printf("as.area.start=%p\tend=%p\n", pcb->as.area.start, pcb->as.area.end);
	uintptr_t gprx = (uintptr_t)new_page(8) + 0x8000;
	uintptr_t p_stack_end = gprx;
	map(&pcb->as , (void*)pcb->as.area.end - 0x8000, (void*)gprx - 0x8000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x7000, (void*)gprx - 0x7000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x6000, (void*)gprx - 0x6000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x5000, (void*)gprx - 0x5000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x4000, (void*)gprx - 0x4000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x3000, (void*)gprx - 0x3000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x2000, (void*)gprx - 0x2000, 0);
	map(&pcb->as , (void*)pcb->as.area.end - 0x1000, (void*)gprx - 0x1000, 0);
	
	int argc = 0, envpc = 0, str_tot = 0;
	if(argv){
		argc = -1;
		while(argv[++argc]){
			str_tot += strlen(argv[argc]) + 1;
			/* printf("context_uload:argv[%d]=%p\n", argc ,argv[argc]); */
			/* printf("context_uload:argv[%d]=%s\n", argc ,argv[argc]); */
		}
	}
	/* printf("argc=%d\n", argc); */
	if(envp){
		envpc = -1;
		while(envp[++envpc]){
			str_tot += strlen(envp[envpc]) + 1;
			/* printf("context_uload:envp[%d]=%p\n", envpc, envp[envpc]); */
			/* printf("context_uload:envp[%d]=%s\n", envpc, envp[envpc]); */
		}
	}

	str_tot = ((str_tot >> 2) + 1) << 2;
	uintptr_t string_start = gprx - str_tot;

	/* if(envp)	printf("uload:%d: envp[0]=%p\n",__LINE__,  envp[0]); */
	/* printf("envpc=%d\n", envpc); */
	/* printf("gcpx = %p\n", gprx); */
	gprx = string_start - 4*argc - 4*envpc - 12;
	*(int*)(gprx) = argc > 0 ? argc : 0;
	uintptr_t argv_start = gprx + 4;
	uintptr_t envp_start = argv_start + 4*argc + 4;
	for(int k = 0; k < argc; k++){
		int len = strlen(argv[k]) + 1;
		*((uintptr_t*)argv_start + k) = (uintptr_t)pcb->as.area.end - (p_stack_end - string_start);
		memcpy((void*)string_start, argv[k], len);
		printf("argv[%d]=%s\n", k, (char*)string_start);
		printf("string=%p\n", (void*)string_start);
		printf("string=%p\n", (uintptr_t)pcb->as.area.end - (p_stack_end - string_start));
		string_start += len;
	}
	memset((uintptr_t*)argv_start + argc, 0, 4);
	for(int k = 0; k < envpc; k++){
		int len = strlen(envp[k]) + 1; 
		*((uintptr_t*)envp_start + k) = (uintptr_t)pcb->as.area.end - (p_stack_end - string_start);
		memcpy((void*)string_start, envp[k], len);
		/* printf("envp[%d]=%s\n", k, (char*)string_start); */
		/* printf("string=%p\n", (void*)string_start); */
		string_start += len;
	}
	memset((uintptr_t*)envp_start + envpc, 0, 4);

	uintptr_t entry = loader(pcb, filename);
    pcb->cp = ucontext(&pcb->as, kstack, (void*)entry);
	/* printf("%p-4=%p\n", (void*)entry, (void*)entry-4); */
	pcb->cp->GPRx = (uintptr_t)pcb->as.area.end - (p_stack_end - gprx);
	printf("GPRx=%p\n", pcb->cp->GPRx);
	pcb->max_brk = MAX_BRK;
	printf("argv_start=%p\tenvp_start=%p\n", (uintptr_t)pcb->as.area.end - (p_stack_end - argv_start), (uintptr_t)pcb->as.area.end - (p_stack_end - envp_start));
	printf("uload pcb=%p\tbrk=%p\n", pcb, pcb->max_brk);
	/* printf("pcb=%p\tas=%p\tas->ptr=%p\n", pcb, pcb->as, pcb->as.ptr); */
	Log("Uload file=%s, entry=%p", filename, entry);
	/* if(envp)	printf("uload:%d: envp[0]=%p\n",__LINE__,  envp[0]); */
	/* char **wargv = (char**)((uintptr_t)gprx+sizeof(int)); */
	/* char **wenvp = (char**)(wargv + argc + 1); */
	/* printf(":wargv=%p\n", wargv); */
	/* printf(":wenvp=%p\n", wenvp); */
	/* if(wenvp){ */
	/* 	printf(":wenvp[0]=%p\n", wenvp[0]); */
	/* 	if(wenvp[0]) printf(":wenvp[0]=%s\n", wenvp[0]); */
	/* } */
}
// wk 4.1
