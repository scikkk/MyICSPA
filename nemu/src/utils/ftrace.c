#include <common.h>
#include <elf.h>


/* extern uint32_t g_nr_guest_instr; */
FILE *elf_fp = NULL;

struct func{
	paddr_t addr;
	uint32_t size;
	char name[20];
} func_table[99];
static short func_idx=0;

void tableheader(const char *pbuff)
{
	//从节区里面定位到偏移
	Elf32_Ehdr* pfilehead = (Elf32_Ehdr*)pbuff;
	Elf32_Half eshstrndx = pfilehead->e_shstrndx;
	Elf32_Shdr* psecheader = (Elf32_Shdr*)(pbuff + pfilehead->e_shoff);
	Elf32_Shdr* pshstr = (Elf32_Shdr*)(psecheader + eshstrndx);
	char* pshstrbuff = (char *)(pbuff + pshstr->sh_offset);
	for(int i = 0;i<pfilehead->e_shnum;++i)
	{
		if(!strcmp(psecheader[i].sh_name + pshstrbuff, ".dynsym") || !strcmp(psecheader[i].sh_name + pshstrbuff, ".symtab"))
		{
			Elf32_Sym* psym = (Elf32_Sym*)(pbuff + psecheader[i].sh_offset);
			int ncount = psecheader[i].sh_size / psecheader[i].sh_entsize;
			char* pbuffstr = (char*)((psecheader + psecheader[i].sh_link)->sh_offset + pbuff);
			/* printf("Symbol table '%s' contains %d entries:\r\n", psecheader[i].sh_name + pshstrbuff, ncount); */
			for(int idx = 0; idx <ncount; idx++)
			{
				if(ELF32_ST_TYPE(psym->st_info)==STT_FUNC){
					/* printf("%-8x\t %u\t %s\n",psym->st_value, psym->st_size,(psym->st_name+pbuffstr)); */
					func_table[func_idx].addr = psym->st_value;
					func_table[func_idx].size = psym->st_size;
					strcat(func_table[func_idx].name, psym->st_name+pbuffstr);
					func_idx++;
				}	
				psym++;
			}
			continue;
		}
	}
}

void func_display(){
	for(int k = 0; k < func_idx; k++){
		printf("0x%-10x%-6d%s\n",func_table[k].addr,func_table[k].size,func_table[k].name);
	}
}

void init_ftrace(const char *elf_file) {
	elf_fp = stdout;

	/* printf("elf_file: %s\n", elf_file); */

	FILE *fp = fopen(elf_file, "r");
	Assert(fp, "Can not open '%s'", elf_file);

	elf_fp = fp;
	fseek(elf_fp, 0, SEEK_END);
	int file_size = ftell(fp);
	/* printf("size: %i\n", file_size); */
	char *elf_str = (char*)malloc(sizeof(char)*file_size);
	fseek(elf_fp, 0, SEEK_SET);

	unsigned long ret =	fread(elf_str, sizeof(char), file_size, elf_fp);
	assert(ret == file_size);

	fclose(elf_fp);
	tableheader(elf_str);
	free(elf_str);
	Log("Symbol table is loaded from %s", elf_file);
}

/* bool ftrace_enable() { */
/*   return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) && */
/*          (g_nr_guest_instr <= CONFIG_TRACE_END), false); */
/* } */

