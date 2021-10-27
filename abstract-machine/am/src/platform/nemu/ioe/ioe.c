#include <am.h>
#include <klib-macros.h>

void __am_timer_init();
void __am_gpu_init();
void __am_audio_init();
void __am_input_keybrd(AM_INPUT_KEYBRD_T *);
void __am_timer_rtc(AM_TIMER_RTC_T *);
void __am_timer_uptime(AM_TIMER_UPTIME_T *);
void __am_gpu_config(AM_GPU_CONFIG_T *);
void __am_gpu_status(AM_GPU_STATUS_T *);
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *);
void __am_audio_config(AM_AUDIO_CONFIG_T *);
void __am_audio_ctrl(AM_AUDIO_CTRL_T *);
void __am_audio_status(AM_AUDIO_STATUS_T *);
void __am_audio_play(AM_AUDIO_PLAY_T *);
void __am_disk_config(AM_DISK_CONFIG_T *cfg);
void __am_disk_status(AM_DISK_STATUS_T *stat);
void __am_disk_blkio(AM_DISK_BLKIO_T *io);

static void __am_timer_config(AM_TIMER_CONFIG_T *cfg) { cfg->present = true; cfg->has_rtc = true; }
static void __am_input_config(AM_INPUT_CONFIG_T *cfg) { cfg->present = true;  }
static void __am_uart_config(AM_UART_CONFIG_T *cfg)   { cfg->present = false; }
static void __am_net_config (AM_NET_CONFIG_T *cfg)    { cfg->present = false; }

typedef void (*handler_t)(void *buf);
static void *lut[128] = {
	[AM_TIMER_CONFIG] = __am_timer_config,
	[AM_TIMER_RTC   ] = __am_timer_rtc,
	[AM_TIMER_UPTIME] = __am_timer_uptime,
	[AM_INPUT_CONFIG] = __am_input_config,
	[AM_INPUT_KEYBRD] = __am_input_keybrd,
	[AM_GPU_CONFIG  ] = __am_gpu_config,
	[AM_GPU_FBDRAW  ] = __am_gpu_fbdraw,
	[AM_GPU_STATUS  ] = __am_gpu_status,
	[AM_UART_CONFIG ] = __am_uart_config,
	[AM_AUDIO_CONFIG] = __am_audio_config,
	[AM_AUDIO_CTRL  ] = __am_audio_ctrl,
	[AM_AUDIO_STATUS] = __am_audio_status,
	[AM_AUDIO_PLAY  ] = __am_audio_play,
	[AM_DISK_CONFIG ] = __am_disk_config,
	[AM_DISK_STATUS ] = __am_disk_status,
	[AM_DISK_BLKIO  ] = __am_disk_blkio,
	[AM_NET_CONFIG  ] = __am_net_config,
};

static void fail(void *buf) { panic("access nonexist register"); }

bool ioe_init() {
	for (int i = 0; i < LENGTH(lut); i++)
		if (!lut[i]) lut[i] = fail;
	__am_gpu_init();
	__am_timer_init();
	__am_audio_init();
	return true;
}
// wk 2.3 dtrace
void mystrcpy(char *dst, char *src){

	while(*src){
		*dst++=*src++;
	}
	*dst = '\0';
}
void regidx2name(int reg, char* name){
	switch (reg){
		case 0:mystrcpy(name, "AM_TIMER_CONFIG"); break;
		case 1:mystrcpy(name, "AM_TIMER_RTC   "); break;
		case 2:mystrcpy(name, "AM_TIMER_UPTIME"); break;
		case 3:mystrcpy(name, "AM_INPUT_CONFIG"); break;
		case 4:mystrcpy(name, "AM_INPUT_KEYBRD"); break;
		case 5:mystrcpy(name, "AM_GPU_CONFIG  "); break;
		case 6:mystrcpy(name, "AM_GPU_FBDRAW  "); break;
		case 7:mystrcpy(name, "AM_GPU_STATUS  "); break;
		case 8:mystrcpy(name, "AM_UART_CONFIG "); break;
		case 9:mystrcpy(name, "AM_AUDIO_CONFIG"); break;
		case 10:mystrcpy(name,"AM_AUDIO_CTRL  "); break;
		case 11:mystrcpy(name,"AM_AUDIO_STATUS"); break;
		case 12:mystrcpy(name,"AM_AUDIO_PLAY  "); break;
		case 13:mystrcpy(name,"AM_DISK_CONFIG "); break;
		case 14:mystrcpy(name,"AM_DISK_STATUS "); break;
		case 15:mystrcpy(name,"AM_DISK_BLKIO  "); break;
		case 16:mystrcpy(name,"AM_NET_CONFIG  "); break;
		default:mystrcpy(name,"UnKnown Device ");
	}
}
// wk 2.3 dtrace



void ioe_read (int reg, void *buf) {
#ifdef CONFIG_DTRACE
	static int count = 1;
	char name[20];
	regidx2name(reg, name);	
	printf("[read |ioe] count:%-6d name:%s \n", count++, name, ret, ret);
#endif
	((handler_t)lut[reg])(buf); 
}
void ioe_write(int reg, void *buf) { 
#ifdef CONFIG_DTRACE
	static int count = 1;
	char name[20];
	regidx2name(reg, name);	
	printf("[write|ioe] count:%-6d name:%s \n", count++, name, data, data);
#endif
	((handler_t)lut[reg])(buf); 
}
