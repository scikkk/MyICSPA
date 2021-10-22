#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	/* kbd->keydown = inw(KBD_ADDR)&KEYDOWN_MASK; */
	/* kbd->keycode = inw(KBD_ADDR+2); */
	
	/* kbd->keydown = 0; */ 
	/* kbd->keycode = AM_KEY_NONE; */

	uint32_t key = inl(KBD_ADDR);
	kbd->keydown = (key)&KEYDOWN_MASK; 
	kbd->keycode = (key)&(~KEYDOWN_MASK);
}
