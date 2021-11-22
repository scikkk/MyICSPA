#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

// wk 3.3
/* #define SYS_gettimeofday 19 */
#include <sys/time.h>
extern int gettimeofday(struct timeval *ptimeval, void *ptimezone);
// wk 3.3
uint32_t NDL_GetTicks() {
	struct timeval tv;
	/* _syscall_(SYS_gettimeofday, (unsigned long)&tv, 0, 0); */
	gettimeofday(&tv, NULL);
	printf("tv:%p\n", &tv);
	printf("hit there\n");
	uint32_t ret = tv.tv_sec*1000+tv.tv_usec/1000;

	printf("NDL_sec: %ld\n", tv.tv_sec);
	printf("NDL_usec: %ld\n", tv.tv_usec);
	printf("NDL_ms: %d\n", ret);
	return ret;

	/* return 0; */
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
	if (*w == 0 && *h == 0){
		*w = screen_w;
		*h = screen_h;
	}
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	int offset = 0;
	// wk center
	x = (screen_w - w)/2;
	y = (screen_h - h)/2;
	// wk center
	FILE *fp = fopen("/dev/fb", "w");
	for(int cur_y = y; cur_y < y + h; cur_y++){
		offset = (screen_w*cur_y + x)*4;
		fseek(fp, offset, SEEK_SET);
		/* printf("x=%d\ty=%d\toffset=%d\n", x, cur_y, offset); */
		fwrite(pixels, 4, w, fp);
		pixels += w;
	}
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
	printf("%p\n", fp);
	
	fscanf(fp, "WIDTH : %d\nHEIGHT : %d", &screen_w, &screen_h);
	/* printf("W:%d\tH:%d\n", screen_w, screen_h); */
	fclose(fp);

	return 0;
}

void NDL_Quit() {
}
