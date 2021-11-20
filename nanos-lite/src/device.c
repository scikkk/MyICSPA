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
	for(size_t k = 0; k < len; k++){
		putch(*(((char*)buf)+k));
	} 
	return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	assert(offset == 0);
	/* assert(0); */
	char tmp[50];
	/* while (1) { */
	AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
	if (ev.keycode == AM_KEY_NONE)
		return 0;
	/* break; */

	sprintf(tmp, "%s %s\n\0", ev.keydown ? "kd" : "ku", keyname[ev.keycode]);
	strcat(buf, tmp);
	/* } */
	return strlen(buf);
	/* return 0; */
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	return 0;
}

void init_device() {
	Log("Initializing devices...");
	ioe_init();
}
