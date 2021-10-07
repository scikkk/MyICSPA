#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) \
   f(auipc) f(jal) f(jalr) f(addi) \
   f(add)  f(slti) f(sltiu) f(slt) f(sltu)\
   f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) 
// wk 2.1

def_all_EXEC_ID();
