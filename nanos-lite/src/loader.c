#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

// wk 3.2
// 从ramdisk中`offset`偏移处的`len`字节读入到`buf`中
size_t ramdisk_read(void *buf, size_t offset, size_t len);

// 把`buf`中的`len`字节写入到ramdisk中`offset`偏移处
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

// 返回ramdisk的大小, 单位为字节
size_t get_ramdisk_size();
// wk 3.2


static uintptr_t loader(PCB *pcb, const char *filename) {
	/* TODO(); */
	/* size_t size = get_ramdisk_size(); */
	Elf_Ehdr header;
	ramdisk_read(&header,0,52);
	Elf_Phdr ph;
	for(int k = 0; k < header.e_phnum; k++){
		ramdisk_read(&ph, header.e_phoff, header.e_phentsize);
		if(ph.p_type == PT_LOAD){
		char seg[65536];
		ramdisk_read(seg, ph.p_offset, ph.p_filesz);
		memcpy((void*)ph.p_vaddr, seg, ph.p_filesz);
		printf("%x\n\n", ph.p_vaddr);
		memset((void*)(ph.p_vaddr+ph.p_filesz), 0, ph.p_memsz-ph.p_filesz);
		}
	}

	return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
	uintptr_t entry = loader(pcb, filename);
	Log("Jump to entry = %p", entry);
	((void(*)())entry) ();
}

