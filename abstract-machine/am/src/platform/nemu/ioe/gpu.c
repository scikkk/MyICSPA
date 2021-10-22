#include <am.h>
#include <nemu.h>

#ifdef MODE_800x600
# define W    800
# define H    600
#else
# define W    400
# define H    300
#endif 


#define SYNC_ADDR (VGACTL_ADDR + 20)

void __am_gpu_init() {
	int i;
	int w = 400;  // TODO: get the correct width
	int h = 300;  // TODO: get the correct height
	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
	for (i = 0; i < w * h; i ++) fb[i] = i;
	outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
	*cfg = (AM_GPU_CONFIG_T) {
		.present = true, .has_accel = false,
			.width = W, .height = H,
			.vmemsz = 0
	};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	uint32_t *pixels = ctl->pixels;
	for(int r = y; r < y + h; r++, pixels+=w){
		for(int c = x; c < x + w; c++){
			uint32_t p = pixels[c-x];
			outl(FB_ADDR+c+r*W, p);
		}
	}
	if (ctl->sync) {
		outb(SYNC_ADDR, 1);
	}
	outl(VGACTL_ADDR, x);
	outl(VGACTL_ADDR+4, y);
	outl(VGACTL_ADDR+12, w);
	outl(VGACTL_ADDR+16, h);
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
	status->ready = true;
}

/* void __am_gpu_memcpy(AM_GPU_MEMCPY_T *cpy){ */

/* } */

/* void __am_gpu_render(AM_GPU_RENDER_T *render){ */

/* } */
