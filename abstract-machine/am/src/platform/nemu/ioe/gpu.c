#include <am.h>
#include <nemu.h>


#define SYNC_ADDR (VGACTL_ADDR + 4)
#define W   inw(VGACTL_ADDR+2)
#define H   inw(VGACTL_ADDR) 

void __am_gpu_init() {
	int i;
	int w = W;  // TODO: get the correct width
	int h = H;  // TODO: get the correct height
	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
	for (i = 0; i < w * h; i ++) 
	{
		fb[i] = 0x0000ffff;
	}
	outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
	*cfg = (AM_GPU_CONFIG_T) {
		.present = true, .has_accel = false,
			.width = W, .height = H,
			.vmemsz = W*H*32
	};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	uint32_t *pixels = ctl->pixels;
	for(int r = y; r < y + h; r++, pixels+=w){
		for(int c = x; c < x + w; c++){
			uint32_t p = pixels[c-x];
			outl(FB_ADDR+(c+r*W)*4, p);
		}
	}
	/* if (ctl->sync) { */
	/* 	outl(SYNC_ADDR, 1); */
	/* } */
	outl(SYNC_ADDR, ctl->sync);
	/* outb(VGACTL_ADDR, x); */
	/* outb(VGACTL_ADDR+1, y); */
	/* outb(VGACTL_ADDR+2, w); */
	/* outb(VGACTL_ADDR+3, h); */
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
	status->ready = true;
}

/* void __am_gpu_memcpy(AM_GPU_MEMCPY_T *cpy){ */

/* } */

/* void __am_gpu_render(AM_GPU_RENDER_T *render){ */

/* } */
