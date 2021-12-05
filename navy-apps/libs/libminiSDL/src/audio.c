#include <NDL.h>
#include <SDL.h>

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	return 0;
}

void SDL_CloseAudio() {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

void SDL_PauseAudio(int pause_on) {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume) {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

SDL_AudioSpec *SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len) {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf) {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

void SDL_LockAudio() {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

void SDL_UnlockAudio() {
	/* printf("\n\naudio.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}
