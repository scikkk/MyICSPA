
#include <common.h>
#include <elf.h>


/* extern uint32_t g_nr_guest_instr; */
FILE *elf_fp = NULL;
FILE *ftrace_fp = NULL;
static struct func{
	paddr_t begin_addr;
	paddr_t end_addr;
	uint32_t size;
	char name[100];
} func_table[1024];
static short func_idx=0;



static struct FtraceOneline{
	bool is_call;
	paddr_t pc;
	unsigned name_idx;
	paddr_t dst;
} ftrace_res[65535];
static unsigned ftrace_idx = 0;

static void tableheader(const char *pbuff)
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
					func_table[func_idx].begin_addr = psym->st_value;
					func_table[func_idx].end_addr = psym->st_value+psym->st_size-4;
					func_table[func_idx].size = psym->st_size;
					strcpy(func_table[func_idx].name, psym->st_name+pbuffstr);
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
		printf("begin:0x%-10xend:%-10xsize:%-6dname:%s\n",func_table[k].begin_addr,func_table[k].end_addr,func_table[k].size,func_table[k].name);
	}
}

void init_ftrace(const char *elf_file) {
	elf_fp = stdout;

	/* printf("elf_file: %s\n", elf_file); */

	// open elf
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
	/* func_display(); */

	// wk 3.2 ---------------------------------------------------------

	/* char ramdisk_file[] =  "/home/wk/ics2021/navy-apps/fsimg/bin/pal"; */
	/* fp = fopen(ramdisk_file, "r"); */
	/* Assert(fp, "Can not open '%s'", ramdisk_file); */

	/* FILE *ramdisk_fp = fp; */
	/* fseek(ramdisk_fp, 0, SEEK_END); */
	/* file_size = ftell(fp); */
	/* /1* printf("size: %i\n", file_size); *1/ */
	/* elf_str = (char*)malloc(sizeof(char)*file_size); */

	/* fseek(ramdisk_fp, 0, SEEK_SET); */

	/* ret =	fread(elf_str, sizeof(char), file_size, ramdisk_fp); */
	/* assert(ret == file_size); */

	/* fclose(ramdisk_fp); */
	/* tableheader(elf_str); */
	/* free(elf_str); */
	/* Log("Symbol table is loaded from %s", ramdisk_file); */
	// wk 3.2 ---------------------------------------------------------

	/* func_display(); */
#ifdef CONFIG_FTRACE_FILE_COND
	if(FTRACE_FILE_COND){
		char tmp[100];
		strcpy(tmp, elf_file);
		char* path_a = strstr(tmp, "build/");
		strcpy(path_a, "build/ftrace-res.txt");
		/* printf("%s\n\n\n", tmp); */
		FILE *fp = fopen(tmp, "w");
		Assert(fp, "Can not open '%s'", tmp);
		ftrace_fp = fp;
		Log("Log is written to %s", tmp);
	}
#endif
}

/* bool ftrace_enable() { */
/*   return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) && */
/*          (g_nr_guest_instr <= CONFIG_TRACE_END), false); */
/* } */

void write_to_file(struct FtraceOneline *cur){
#ifdef CONFIG_FTRACE_FILE_COND
	if(FTRACE_FILE_COND){
		static int depth = -1;
		fprintf(ftrace_fp, "0x%08x: ",cur->pc);
		if(cur->is_call){
			depth++;
			for(int _ = 0; _ < depth; _++) fprintf(ftrace_fp, "  |");
			fprintf(ftrace_fp, "call [%s@0x%x]\n", func_table[cur->name_idx].name, cur->dst);
		}
		else{
			for(int _ = 0; _ < depth; _++) fprintf(ftrace_fp, "  |");
			depth--;
			fprintf(ftrace_fp,"ret [%s]\n", func_table[cur->name_idx].name);
		}
		fflush(ftrace_fp);

	}
#endif
}

static bool in_func(int idx, paddr_t addr){
	return (addr >= func_table[idx].begin_addr) && (addr <= func_table[idx].end_addr);    
}


bool in_ban_funcs(char *src_name){
	char ban_list[][20] = {"putch", "_putc_r", "_fputc_r", "\0"};
	for(int k = 0; strlen(ban_list[k]) > 0; k++){
		if (strcmp(ban_list[k], src_name) == 0){
			return true;
		}
	}
	return false;

}

void ftrace_write(paddr_t src, paddr_t dst, bool is_call){
	// ret 
	if (!is_call){
		struct FtraceOneline *cur = &ftrace_res[ftrace_idx++];
#ifdef CONFIG_FTRACE_FILE_COND
		if(FTRACE_FILE_COND){
			ftrace_idx = 6;
		}
#endif 
		for (int k = 0; k < func_idx; k++){
			if (in_func(k, src)){
				if (in_ban_funcs(func_table[k].name)) {
					ftrace_idx--;
					return;
				}
				cur->is_call = 0;
				cur->pc = src;
				cur->name_idx = k;
				write_to_file(cur);
			}
		}
	}
	else{

		for (int k = 0; k < func_idx; k++){
			if (dst == func_table[k].begin_addr){
				struct FtraceOneline *cur = &ftrace_res[ftrace_idx++];
#ifdef CONFIG_FTRACE_FILE_COND
				if(FTRACE_FILE_COND){
					ftrace_idx = 6;
				}
#endif 
				/* if (strcmp(func_table[k].name, "putch") == 0){ */
				if (in_ban_funcs(func_table[k].name)) {
					ftrace_idx--;
					return;
				}
				cur->is_call = 1;
				cur->pc = src;
				cur->name_idx = k;
				cur->dst = dst;
				write_to_file(cur);
			}
		}
	}

	/* printf("%s\n", func_table[cur->name_idx].name); */
	/* assert(0); */
}

static void tab_in(int dep){ 
	for(int k = 0; k < dep; k++){ 
		printf("   "); 
	} 
}

void ftrace_display(){
	struct FtraceOneline *cur;
	int depth = 0;
	for(unsigned k = 0; k < ftrace_idx; k++){
		cur = &ftrace_res[k];
		printf("0x%08x: ",cur->pc);
		if(cur->is_call){
			tab_in(++depth);
			printf("call [%s@0x%x]\n", func_table[cur->name_idx].name, cur->dst);
		}
		else{
			tab_in(depth--);
			printf("ret [%s]\n", func_table[cur->name_idx].name);
		}
	}
}


