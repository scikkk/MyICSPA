.globl  _start
_start:
  move s0, zero
# wk 4.1
  mv sp, zero
  li sp, 0
# wk 4.1
  jal call_main
