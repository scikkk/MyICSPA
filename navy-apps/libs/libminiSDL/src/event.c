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
	  printf("%s\n", type);
	  printf("%s\n", name);

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
