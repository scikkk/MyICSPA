#include <NDL.h>

int SDL_Init(uint32_t flags) {
	/* printf("\n\ngeneral.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	return NDL_Init(flags);
}

void SDL_Quit() {
	printf("\n\ngeneral.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	NDL_Quit();
}

char *SDL_GetError() {
	printf("\n\ngeneral.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char* fmt, ...) {
	printf("\n\ngeneral.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return -1;
}

int SDL_ShowCursor(int toggle) {
	/* printf("\n\ngeneral.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon) {
	printf("SDL_WM_SetCaption: title=%-16sicon=%s\n", title, icon);
	/* printf("\n\ngeneral.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}
