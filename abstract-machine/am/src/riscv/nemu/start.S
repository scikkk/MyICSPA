.section entry, "ax"
.globl _start
.type _start, @function

_start:
  ; mv s0, zero
  ; la sp, _stack_pointer
  # wk 4.1
  mv sp, a0
  # wk 4.1
  jal _trm_init
