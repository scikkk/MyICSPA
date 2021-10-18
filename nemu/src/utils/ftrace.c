#include <common.h>

/* extern uint64_t g_nr_guest_instr; */
FILE *elf_fp = NULL;

void init_ftrace(const char *elf_file) {
  elf_fp = stdout;
  printf("elf_file: %s\n", elf_file);
  if (elf_file != NULL) {
    FILE *fp = fopen(elf_file, "r");
    Assert(fp, "Can not open '%s'", elf_file);
    elf_fp = fp;
  }
  Log("Symbol table is loaded from %s", elf_file ? elf_file : "stdout");
}

/* bool ftrace_enable() { */
/*   return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) && */
/*          (g_nr_guest_instr <= CONFIG_TRACE_END), false); */
/* } */

