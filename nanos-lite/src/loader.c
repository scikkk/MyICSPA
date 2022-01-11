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
		fs_lseek(fd, off, SEEK_SET);
		fs_read(fd, &ph, header.e_phentsize);
		off += header.e_phentsize;
		if(ph.p_type == PT_LOAD){
			fs_lseek(fd, ph.p_offset, SEEK_SET);
			fs_read(fd, (void*)ph.p_vaddr, ph.p_filesz);
			if(ph.p_memsz > ph.p_filesz){
				memset((void*)(ph.p_vaddr+ph.p_filesz), 0, ph.p_memsz-ph.p_filesz);
			}
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
	pcb->cp = kcontext(kstack, entry, arg);
	/* printf("kcontext-ret=%p\n", pcb->cp); */
}
// wk 4.1


// wk 4.1
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]){

	/* printf("context_uload:filename=%s\n", filename); */
	/* printf("argv=%p\tenvp=%p\n", argv, envp); */
	Area kstack = {pcb, pcb+1};
	uintptr_t entry = loader(pcb, filename);
	pcb->cp = ucontext(NULL, kstack, (void*)entry);
	pcb->cp->GPRx = (uintptr_t)new_page(8);
	int argc = 0, envpc = 0;
	/* if(envp)	printf("uload envp[0]=%p\n", envp[0]); */
	if(argv){
		argc = -1;
		while(argv[++argc]){
			printf("context_uload:argv[%d]=%p\n", argc ,argv[argc]);
			printf("context_uload:argv[%d]=%s\n", argc ,argv[argc]);
		}
	}
	/* printf("argc=%d\n", argc); */
	if(0&&envp){
		envpc = -1;
		while(envp[++envpc]){
			printf("context_uload:envp[%d]=%p\n", envpc ,envp[envpc]);
			printf("context_uload:envp[%d]=%s\n", envpc ,envp[envpc]);
		}
	}
	/* printf("envpc=%d\n", envpc); */
	*(int*)(pcb->cp->GPRx) = argc > 0 ? argc : 0;
	uintptr_t argv_start = pcb->cp->GPRx + 4;
	uintptr_t envp_start = argv_start + 4*argc + 4;
	uintptr_t envp_end = argv_start + 4*argc + 4 + 4*envpc + 4;
	uintptr_t string_end = ((envp_end>>2)+1)<<2;
	for(int k = 0; k < argc; k++){
		int len = strlen(argv[k]) + 1;
		*((uintptr_t*)argv_start + k) = string_end;
		memcpy((void*)string_end, argv[k], len);
		/* printf("argv[%d]=%s\n", k, (char*)string_end); */
		/* printf("string=%p\n", (void*)string_end); */
		string_end += len;
	}
	memset((uintptr_t*)argv_start + argc, 0, 4);
	for(int k = 0; k < envpc; k++){
		int len = strlen(envp[k]) + 1;
		*((uintptr_t*)argv_start + k) = string_end;
		memcpy((void*)string_end, envp[k], len);
		string_end += len;
	}
	memset((uintptr_t*)envp_start + envpc, 0, 4);
}
// wk 4.1
