#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
	[AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
	[AM_KEY_NONE] = "NONE",
	AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
	// wk 4.1
	/* yield(); */
	// wk 4.1
	for(size_t k = 0; k < len; k++){
		putch(*(((char*)buf)+k));
	} 
	return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	// wk 4.1
	/* yield(); */
	// wk 4.1
	assert(offset == 0);
	/* return 0; */
	AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
	if (ev.keycode == AM_KEY_NONE)
		return 0;
	return 1+sprintf(buf, "%s %s\n", ev.keydown ? "kd" : "ku", keyname[ev.keycode]);
	
	/* return 0; */
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	char whole_buf[100];
	sprintf(whole_buf, "WIDTH:%d\nHEIGHT:%d", io_read(AM_GPU_CONFIG).width, io_read(AM_GPU_CONFIG).height);
	strncpy(buf, whole_buf, len);
	/* printf("len:%d\twhole_buf_len:%d\n%s", len, strlen(whole_buf), buf); */
	return strlen(buf);
	/* return 0; */
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	// wk 4.1
	/* yield(); */
	// wk 4.1
	size_t x,y, W;
	W = io_read(AM_GPU_CONFIG).width;
	offset /= 4;
	y = offset / W;
	x = offset - W*y;
	/* printf("SYS:x=%d y=%d offset=%d len=%d\n", x, y, offset*4, len); */
	io_write(AM_GPU_FBDRAW, x, y, (void*)buf, len/4, 1, true);
	return len;
	/* return 0; */
}

void init_device() {
	Log("Initializing devices...");
	ioe_init();
}
