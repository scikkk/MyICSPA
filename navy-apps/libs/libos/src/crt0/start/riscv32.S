.globl  _start
_start:
  move s0, zero
# wk 4.1
  mv sp, a1
# wk 4.1
  jal call_main
