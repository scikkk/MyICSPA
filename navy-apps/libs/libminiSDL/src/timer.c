#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
	printf("\n\ntimer.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
	printf("\n\ntimer.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return 1;
}

uint32_t SDL_GetTicks() {
	/* printf("\n\ntimer.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	return NDL_GetTicks();
}

void SDL_Delay(uint32_t ms) {
	/* printf("\n\ntimer.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	uint32_t begin = SDL_GetTicks();
	while(SDL_GetTicks() - begin < ms);
}
