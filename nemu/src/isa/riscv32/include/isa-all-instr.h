#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) \
   f(auipc) f(jal) f(jalr) f(addi) \
   f(add) f(sub) f(slti) f(sltiu) f(slt) f(sltu) f(sll) f(srl) f(sra)\
   f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) \
    f(xor) f(or) f(and) \
   f(xori) f(ori) f(andi)\
  f(slli) f(srli) f(srai) \
   f(lb)  f(lbu) f(lh) f(lhu) f(sb) f(sh) \
f(mul) f(mulh) f(mulhu) f(div) f(divu)  f(rem) f(remu) \
f(ecall) f(mret) f(csrrw) f(csrrs) 
// wk 2.1

def_all_EXEC_ID();
