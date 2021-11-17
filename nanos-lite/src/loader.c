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
	/* size_t size = get_ramdisk_size(); */
	int fd = fs_open(filename, 0, 0);
	Elf_Ehdr header;
	//	ramdisk_read(&header,0,52);
	fs_read(fd, &header, 52);	
	/* printf("moshu:%x\n", *(uint32_t *)header.e_ident); */
	assert(*(uint32_t *)header.e_ident == 0x464c457f);
	/* printf("type:%d\n", header.e_machine); */
	assert(header.e_machine == EXPECT_TYPE);
	Elf_Phdr ph;
	unsigned off = header.e_phoff;
	/* uint32_t vaddrs[20]; */
	/* uint8_t vaddr_idx = 0; */
	for(int k = 0; k < header.e_phnum; k++){
		//		ramdisk_read(&ph, off, header.e_phentsize);
		fs_lseek(fd, off, SEEK_SET);
		fs_read(fd, &ph, header.e_phentsize);
		off += header.e_phentsize;
		if(ph.p_type == PT_LOAD){
			/* char seg[65536]; */
			//			ramdisk_read((void*)ph.p_vaddr, ph.p_offset, ph.p_filesz);
			fs_lseek(fd, ph.p_offset, SEEK_SET);
			fs_read(fd, (void*)ph.p_vaddr, ph.p_filesz);
			/* vaddrs[vaddr_idx++] = ph.p_vaddr; */
			/* printf("%p\n",ph.p_vaddr); */


			/* printf("\n"); */
			/* for(int k = 0; k < 1000; k++){printf("%c", seg[k]);} */
			/* printf("\n"); */
			/* printf("\n"); */


			/* memcpy((void*)ph.p_vaddr, seg, ph.p_filesz); */
			/* Log("file:%x\tmem:%x\toff:%x\n", ph.p_filesz,ph.p_memsz,ph.p_offset); */
			if(ph.p_memsz > ph.p_filesz){

				memset((void*)(ph.p_vaddr+ph.p_filesz), 0, ph.p_memsz-ph.p_filesz);
			}
		} 
	}
	/* return vaddrs[0]; */
	return header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
	uintptr_t entry = loader(pcb, filename);
	Log("Jump to entry = %p", entry);
	((void(*)())entry) ();
}

