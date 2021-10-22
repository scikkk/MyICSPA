#include <am.h>
#include <nemu.h>

#ifdef MODE_800x600
# define W    800
# define H    600
#else
# define W    400
# define H    300
#endif 


#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
	int i;
	
	int w = 400;  // TODO: get the correct width
	int h = 300;  // TODO: get the correct height
    /* w = inl(VGACTL_ADDR+8); */
	/* h = inl(VGACTL_ADDR+12); */

	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
	for (i = 0; i < w * h; i ++) fb[i] = i;
	outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
	*cfg = (AM_GPU_CONFIG_T) {
		.present = true, .has_accel = false,
			.width = 400, .height = 300,
			.vmemsz = 0
	};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	if (ctl->sync) {
		outl(SYNC_ADDR, 1);
	}
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
	status->ready = true;
}
