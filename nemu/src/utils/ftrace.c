#include <common.h>
#include <elf.h>


/* extern uint64_t g_nr_guest_instr; */
FILE *elf_fp = NULL;

void init_ftrace(const char *elf_file) {
	elf_fp = stdout;
	Elf32_Ehdr elfheader;

	/* printf("elf_file: %s\n", elf_file); */

	FILE *fp = fopen(elf_file, "r");
	Assert(fp, "Can not open '%s'", elf_file);
    
	elf_fp = fp;

	assert(fread(&elfheader,32,1,elf_fp)==1);
	printf("%hx\n",elfheader.e_shstrndx);
	Log("Symbol table is loaded from %s", elf_file);
}

/* bool ftrace_enable() { */
/*   return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) && */
/*          (g_nr_guest_instr <= CONFIG_TRACE_END), false); */
/* } */

