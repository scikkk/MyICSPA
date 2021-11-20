#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

// wk 3.3
#define SYS_gettimeofday 19
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);
// wk 3.3
uint32_t NDL_GetTicks() {
	struct timeval tv;
	_syscall_(SYS_gettimeofday, (unsigned long)&tv, 0, 0);
	return tv.tv_sec*1000+tv.tv_usec/1000;

	return 0;
}

int NDL_PollEvent(char *buf, int len) {
	/* int fp = open("/dev/events", 0); */
	/* assert(fp); */
	/* return read(fp, buf, len); */
	FILE *fp = fopen("/dev/events", "r");
	return fread(buf, 1, len, fp);
	/* return 0; */
}

void NDL_OpenCanvas(int *w, int *h) {
	if (getenv("NWM_APP")) {
		int fbctl = 4;
		fbdev = 5;
		screen_w = *w; screen_h = *h;
		char buf[64];
		int len = sprintf(buf, "%d %d", screen_w, screen_h);
		// let NWM resize the window and create the frame buffer
		write(fbctl, buf, len);
		while (1) {
			// 3 = evtdev
			int nread = read(3, buf, sizeof(buf) - 1);
			if (nread <= 0) continue;
			buf[nread] = '\0';
			if (strcmp(buf, "mmap ok") == 0) break;
		}
		close(fbctl);
	}
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
	return 0;
}

int NDL_QueryAudio() {
	return 0;
}

int NDL_Init(uint32_t flags) {
	if (getenv("NWM_APP")) {
		evtdev = 3;
	}

	FILE *fp = fopen("/proc/dispinfo", "r");
	char junk[20];
	fscanf(fp, "%[A-Za-z ]%d\n%[A-Za-z ]%d", junk, &screen_w, junk, &screen_h);
	printf("W:%d\tH:%d\n", screen_w, screen_h);
	fclose(fp);

	return 0;
}

void NDL_Quit() {
}
