#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
	"NONE",
	_KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	while (1) {
		char buf[64];
		if (NDL_PollEvent(buf, sizeof(buf))) {
			printf("receive event: %s\n", buf);
			char type[3], name[20];
			sscanf(buf, "%s %s\n", type, name);
			if(strcmp(type, "ku") == 0){
				event->type = SDL_KEYUP;
			}
			else if(strcmp(type, "kd")){
				event->type = SDL_KEYDOWN;
			}
			else{
				return 0;
			}
			if(!strcmp(name, "0")){

				event->key.keysym.sym = SDLK_0;
			}
			else if(!stramp(name, "1")){
				event->key.keysym.sym = SDLK_1; 
			}
			else if(!stramp(name, "2")){
				event->key.keysym.sym = SDLK_2; 
			}
			else if(!stramp(name, "3")){
				event->key.keysym.sym = SDLK_3; 
			}
			else if(!stramp(name, "4")){
				event->key.keysym.sym = SDLK_4; 
			}
			else if(!stramp(name, "5")){
				event->key.keysym.sym = SDLK_5; 
			}
			else if(!stramp(name, "6")){
				event->key.keysym.sym = SDLK_6;
			}
			else if(!stramp(name, "7")){
				event->key.keysym.sym = SDLK_7; 
			}
			else if(!stramp(name, "8")){
				event->key.keysym.sym = SDLK_8;
			}
			else if(!stramp(name, "9")){
				event->key.keysym.sym = SDLK_9; 
			}
			else if(!stramp(name, "J")){
				event->key.keysym.sym = SDLK_J; 
			}
			else if(!stramp(name, "DOWN")){
				event->key.keysym.sym = SDLK_DOWN; 
			}
			else if(!stramp(name, "K")){
				event->key.keysym.sym = SDLK_K; 
			}
			else if(!stramp(name, "UP")){
				event->key.keysym.sym = SDLK_UP; 
			}
			else if(!stramp(name, "G")){
				event->key.keysym.sym = SDLK_G; 
			}
			else{
				return 0;
			}
			/* printf("%s\n", type); */
			/* printf("%s\n", name); */

			return 1;
		}
	}
	return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return NULL;
}
