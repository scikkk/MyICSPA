#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
	"NONE",
	_KEYS(keyname)
};
static uint8_t keystate[82];

static char key_buf[65535];

int SDL_PushEvent(SDL_Event *ev) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return 0;
}

int SDL_PollEvent(SDL_Event *event) {
	char buf[64];
	if (NDL_PollEvent(buf, sizeof(buf))) {
		/* printf("receive event: %s\n", buf); */
		char type[3], name[20];
		sscanf(buf, "%s %s\n", type, name);
		if(strcmp(type, "ku") == 0){
			event->type = SDL_KEYUP;
		}
		else if(strcmp(type, "kd") == 0){
			event->type = SDL_KEYDOWN;
		}
		else{
			printf("\n\n!!!!!!!!!Error:event.c:%d\n\n", __LINE__);
			return 0;
		}

		if(!strcmp(name, "0")){
			event->key.keysym.sym = SDLK_0;
		}
		else if(!strcmp(name, "1")){
			event->key.keysym.sym = SDLK_1; 
		}
		else if(!strcmp(name, "2")){
			event->key.keysym.sym = SDLK_2; 
		}
		else if(!strcmp(name, "3")){
			event->key.keysym.sym = SDLK_3; 
		}
		else if(!strcmp(name, "4")){
			event->key.keysym.sym = SDLK_4; 
		}
		else if(!strcmp(name, "5")){
			event->key.keysym.sym = SDLK_5; 
		}
		else if(!strcmp(name, "6")){
			event->key.keysym.sym = SDLK_6;
		}
		else if(!strcmp(name, "7")){
			event->key.keysym.sym = SDLK_7; 
		}
		else if(!strcmp(name, "8")){
			event->key.keysym.sym = SDLK_8;
		}
		else if(!strcmp(name, "9")){
			event->key.keysym.sym = SDLK_9; 
		}
		else if(!strcmp(name, "A")){
			event->key.keysym.sym = SDLK_A; 
		}
		else if(!strcmp(name, "B")){
			event->key.keysym.sym = SDLK_B; 
		}
		else if(!strcmp(name, "C")){
			event->key.keysym.sym = SDLK_C; 
		}
		else if(!strcmp(name, "D")){
			event->key.keysym.sym = SDLK_D; 
		}
		else if(!strcmp(name, "E")){
			event->key.keysym.sym = SDLK_E; 
		}
		else if(!strcmp(name, "F")){
			event->key.keysym.sym = SDLK_F; 
		}
		else if(!strcmp(name, "G")){
			event->key.keysym.sym = SDLK_G; 
		}
		else if(!strcmp(name, "H")){
			event->key.keysym.sym = SDLK_H; 
		}
		else if(!strcmp(name, "I")){
			event->key.keysym.sym = SDLK_I; 
		}
		else if(!strcmp(name, "J")){
			event->key.keysym.sym = SDLK_J; 
		}
		else if(!strcmp(name, "K")){
			event->key.keysym.sym = SDLK_K; 
		}
		else if(!strcmp(name, "L")){
			event->key.keysym.sym = SDLK_L; 
		}
		else if(!strcmp(name, "M")){
			event->key.keysym.sym = SDLK_M; 
		}
		else if(!strcmp(name, "N")){
			event->key.keysym.sym = SDLK_N; 
		}
		else if(!strcmp(name, "O")){
			event->key.keysym.sym = SDLK_O; 
		}
		else if(!strcmp(name, "P")){
			event->key.keysym.sym = SDLK_P; 
		}
		else if(!strcmp(name, "Q")){
			event->key.keysym.sym = SDLK_Q; 
		}
		else if(!strcmp(name, "R")){
			event->key.keysym.sym = SDLK_R; 
		}
		else if(!strcmp(name, "S")){
			event->key.keysym.sym = SDLK_S; 
		}
		else if(!strcmp(name, "T")){
			event->key.keysym.sym = SDLK_T; 
		}
		else if(!strcmp(name, "U")){
			event->key.keysym.sym = SDLK_U; 
		}
		else if(!strcmp(name, "V")){
			event->key.keysym.sym = SDLK_V; 
		}
		else if(!strcmp(name, "W")){
			event->key.keysym.sym = SDLK_W; 
		}
		else if(!strcmp(name, "X")){
			event->key.keysym.sym = SDLK_X; 
		}
		else if(!strcmp(name, "Y")){
			event->key.keysym.sym = SDLK_Y; 
		}
		else if(!strcmp(name, "Z")){
			event->key.keysym.sym = SDLK_Z; 
		}
		else if(!strcmp(name, "LEFTBRACKET")){
			event->key.keysym.sym = SDLK_LEFTBRACKET; 
		}
		else if(!strcmp(name, "RIGHTBRACKET")){
			event->key.keysym.sym = SDLK_RIGHTBRACKET; 
		}
		else if(!strcmp(name, "COMMA")){
			event->key.keysym.sym = SDLK_COMMA; 
		}
		else if(!strcmp(name, "PERIOD")){
			event->key.keysym.sym = SDLK_PERIOD; 
		}
		else if(!strcmp(name, "SEMICOLON")){
			event->key.keysym.sym = SDLK_SEMICOLON; 
		}
		else if(!strcmp(name, "APOSTROPHE")){
			event->key.keysym.sym = SDLK_APOSTROPHE; 
		}
		else if(!strcmp(name, "SPACE")){
			event->key.keysym.sym = SDLK_SPACE; 
		}
		else if(!strcmp(name, "SLASH")){
			event->key.keysym.sym = SDLK_SLASH; 
		}
		else if(!strcmp(name, "BACKSLASH")){
			event->key.keysym.sym = SDLK_BACKSLASH; 
		}
		else if(!strcmp(name, "UP")){
			event->key.keysym.sym = SDLK_UP; 
		}
		else if(!strcmp(name, "DOWN")){
			event->key.keysym.sym = SDLK_DOWN; 
		}
		else if(!strcmp(name, "LEFT")){
			event->key.keysym.sym = SDLK_LEFT; 
		}
		else if(!strcmp(name, "RIGHT")){
			event->key.keysym.sym = SDLK_RIGHT; 
		}
		else if(!strcmp(name, "BACKSPACE")){
			event->key.keysym.sym = SDLK_BACKSPACE; 
		}
		else if(!strcmp(name, "DELETE")){
			event->key.keysym.sym = SDLK_DELETE; 
		}
		else if(!strcmp(name, "RETURN")){
			event->key.keysym.sym = SDLK_RETURN; 
		}
		else if(!strcmp(name, "MINUS")){
			event->key.keysym.sym = SDLK_MINUS; 
		}
		else if(!strcmp(name, "EQUALS")){
			event->key.keysym.sym = SDLK_EQUALS; 
		}
		else if(!strcmp(name, "INSERT")){
			event->key.keysym.sym = SDLK_INSERT; 
		}
		else if(!strcmp(name, "HOME")){
			event->key.keysym.sym = SDLK_HOME; 
		}
		else if(!strcmp(name, "END")){
			event->key.keysym.sym = SDLK_END; 
		}
		else if(!strcmp(name, "PAGEUP")){
			event->key.keysym.sym = SDLK_PAGEUP; 
		}
		else if(!strcmp(name, "PAGEDOWN")){
			event->key.keysym.sym = SDLK_PAGEDOWN; 
		}
		else if(!strcmp(name, "TAB")){
			event->key.keysym.sym = SDLK_TAB; 
		}
		else if(!strcmp(name, "F1")){
			event->key.keysym.sym = SDLK_F1; 
		}
		else if(!strcmp(name, "F2")){
			event->key.keysym.sym = SDLK_F2; 
		}
		else if(!strcmp(name, "F3")){
			event->key.keysym.sym = SDLK_F3; 
		}
		else if(!strcmp(name, "F4")){
			event->key.keysym.sym = SDLK_F4; 
		}
		else if(!strcmp(name, "F5")){
			event->key.keysym.sym = SDLK_F5; 
		}
		else if(!strcmp(name, "F6")){
			event->key.keysym.sym = SDLK_F6; 
		}
		else if(!strcmp(name, "F7")){
			event->key.keysym.sym = SDLK_F7; 
		}
		else if(!strcmp(name, "F8")){
			event->key.keysym.sym = SDLK_F8; 
		}
		else if(!strcmp(name, "F9")){
			event->key.keysym.sym = SDLK_F9; 
		}
		else if(!strcmp(name, "F10")){
			event->key.keysym.sym = SDLK_F10; 
		}
		else if(!strcmp(name, "F11")){
			event->key.keysym.sym = SDLK_F11; 
		}
		else if(!strcmp(name, "F12")){
			event->key.keysym.sym = SDLK_F12; 
		}
		else if(!strcmp(name, "ESCAPE")){
			event->key.keysym.sym = SDLK_ESCAPE; 
		}
		else if(!strcmp(name, "GRAVE")){
			event->key.keysym.sym = SDLK_GRAVE; 
		}
		else if(!strcmp(name, "CAPSLOCK")){
			event->key.keysym.sym = SDLK_CAPSLOCK; 
		}
		else if(!strcmp(name, "LSHIFT")){
			event->key.keysym.sym = SDLK_LSHIFT; 
		}
		else if(!strcmp(name, "RSHIFT")){
			event->key.keysym.sym = SDLK_RSHIFT; 
		}
		else if(!strcmp(name, "LCTRL")){
			event->key.keysym.sym = SDLK_LCTRL; 
		}
		else if(!strcmp(name, "RCTRL")){
			event->key.keysym.sym = SDLK_RCTRL; 
		}
		else if(!strcmp(name, "LALT")){
			event->key.keysym.sym = SDLK_LALT; 
		}
		else if(!strcmp(name, "RALT")){
			event->key.keysym.sym = SDLK_RALT; 
		}
		else if(!strcmp(name, "APPLICATION")){
			event->key.keysym.sym = SDLK_APPLICATION; 
		}
		else{
			printf("\n\n!!!!!!!!!Error:event.c:%d\n\n", __LINE__);
			return 0;
		}
		/* printf("%s\n", type); */
		/* printf("%s\n", name); */
		/* printf("%d\n", __LINE__); */
		switch(event->type){
			case SDL_KEYDOWN: keystate[event->key.keysym.sym] = 1; break;
			case SDL_KEYUP: keystate[event->key.keysym.sym] = 0; break;
			default: printf("Shouldn't reach here!\n"); assert(0);
		}
		return 1;
	}
	return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
	while (!SDL_PollEvent(event));
	return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
	printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__);
	return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
	/* printf("\n\nevent.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	if(numkeys != NULL) *numkeys = 82;
	return keystate;
	return NULL;
}
