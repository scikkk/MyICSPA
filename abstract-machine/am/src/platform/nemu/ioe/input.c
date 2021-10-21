#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	kbd->keydown = inw(KBD_ADDR)&KEYDOWN_MASK;
	kbd->keycode = inw(KBD_ADDR+2);
}
