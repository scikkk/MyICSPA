#include <NDL.h>
#include <sdl-video.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(dst && src);
	assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
	assert(dst->format->BytesPerPixel == src->format->BytesPerPixel);
	assert(dst->format->BitsPerPixel / dst->format->BytesPerPixel == 8);
	int bytes_num = src->format->BytesPerPixel;
	assert(bytes_num == 1 || bytes_num == 4);
	/* printf("Bytes num:%d\n", bytes_num); */
	int dx,dy;
	int sx,sy,w,h;
	if(srcrect == NULL){
		sx = 0;
		sy = 0;
		w = src->w;
		h = src->h;
	}
	else{
		sx = srcrect->x;
		sy = srcrect->y;
		w = srcrect->w;
		h = srcrect->h;
	}
	if (dstrect == NULL){
		dx = 0;
		dy = 0;
	}
	else{
		dx = dstrect->x;
		dy = dstrect->y;
		dstrect->w = w;
		dstrect->h = h;
	}
	assert(dx >= 0 && dy >= 0);
	assert(src->w && dst->w);
	uint64_t 	srcoffset, dstoffset;
	if (bytes_num == 4){
		/* printf("w=%d\th=%d\tsw=%d\tsh=%d\tdw=%d\tdh=%d\n", w, h, src->w, src->h, dst->w, dst->h); */
		uint32_t *srcpixels, *dstpixels;
		for(int cur_h = 0; cur_h < h; cur_h++){
			/* printf("sx=%d\tsy=%d\n", sx, sy+cur_h); */
			/* printf("dx=%d\tdy=%d\n", dx, dy+cur_h); */
			srcoffset = (src->w)*(sy + cur_h) + sx;
			srcpixels = (uint32_t*)src->pixels + srcoffset;
			dstoffset = (dst->w)*(dy + cur_h) + dx;
			dstpixels = (uint32_t*)dst->pixels + dstoffset;
			/* printf("dx=%d,dy=%d\n", dx, dy+cur_h); */
			/* printf("sx=%d,sy=%d\n", sx, sy+cur_h); */
			/* printf("soff:%d\tdoff:%d\n", srcoffset, dstoffset); */
			for(int k = 0; k < w; k++){
				dstpixels[k] = srcpixels[k];
			}
		}
	}
	else{
		/* printf("w=%d\th=%d\tsw=%d\tsh=%d\tdw=%d\tdh=%d\n", w, h, src->w, src->h, dst->w, dst->h); */
		uint8_t *srcpixels, *dstpixels;
		for(int cur_h = 0; cur_h < h; cur_h++){
			/* printf("sx=%d\tsy=%d\n", sx, sy+cur_h); */
			/* printf("dx=%d\tdy=%d\n", dx, dy+cur_h); */
			srcoffset = (src->w)*(sy + cur_h) + sx;
			srcpixels = (uint8_t*)src->pixels + srcoffset;
			dstoffset = (dst->w)*(dy + cur_h) + dx;
			dstpixels = (uint8_t*)dst->pixels + dstoffset;
			/* printf("dx=%d,dy=%d\n", dx, dy+cur_h); */
			/* printf("sx=%d,sy=%d\n", sx, sy+cur_h); */
			/* printf("soff:%d\tdoff:%d\n", srcoffset, dstoffset); */
			for(int k = 0; k < w; k++){
				dstpixels[k] = srcpixels[k];
			}
		}
	}
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
	int bytes_num = dst->format->BytesPerPixel; 
	assert(bytes_num == 4 || bytes_num == 1);
	int x,y,w,h,offset=0;
	if(dstrect == NULL){
		x = 0;
		y = 0;
		w = dst->w;
		h = dst->h;
	}
	else{
		x = dstrect->x;
		y = dstrect->y;
		w = dstrect->w;
		h = dstrect->h;
	}
	/* printf("x=%d; y=%d\n", x, y); */
	for(int cur_y = y; cur_y < y + h; cur_y++){
		int offset = (dst->w*cur_y + x)*bytes_num;
		uint8_t *pixels_8 = dst->pixels+offset;
		for(int k = 0; k < w; k++){
			if(bytes_num == 4){
				*(uint32_t*)pixels_8 = color;
				pixels_8 += 4;
			}
			else{
				*pixels_8++ = color&0xff;
			}
		}
	}
	/* printf("\n\nvideo.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

void SDL_UpdateRect(SDL_Surface *s, int x, int y, int w, int h) {
	/* printf("\n\nvideo.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
	int bytes_num = s->format->BytesPerPixel; 
	assert(bytes_num == 1 || bytes_num == 4);
	if (x == 0 && y == 0 && w == 0 && h == 0){
		w = s->w; 
		h = s->h;
	}
	uint32_t *pixels = (uint32_t*)malloc(w*h*4);
	uint32_t *begin = pixels;
	for(int cur_y = y; cur_y < y + h; cur_y++){
		int offset = (s->w*cur_y + x)*bytes_num;
		uint8_t *pixels_8 = s->pixels+offset;
		for(int k = 0; k < w; k++){
			if(bytes_num == 4){
				*pixels++ = *(uint32_t*)pixels_8;
				pixels_8 += 4;
			}
			else{
				SDL_Color *curcolor = &s->format->palette->colors[*pixels_8++];
				uint32_t a=curcolor->a,r=curcolor->r,g=curcolor->g,b=curcolor->b;
				*pixels =  a << 24 | r << 16 | g << 8 | b;	
				pixels += 1;
			}
		}
	}
	assert(begin == pixels-w*h);
	NDL_DrawRect(begin, x, y, w, h);
	free(begin);
	/* printf("\n\nvideo.c: %d: TODO!!!!!!!!!!!!!!!!!!!\n\n", __LINE__); */
}

// APIs below are already implemented.

static inline int maskToShift(uint32_t mask) {
	switch (mask) {
		case 0x000000ff: return 0;
		case 0x0000ff00: return 8;
		case 0x00ff0000: return 16;
		case 0xff000000: return 24;
		case 0x00000000: return 24; // hack
		default: assert(0);
	}
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
		uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
	assert(depth == 8 || depth == 32);
	SDL_Surface *s = malloc(sizeof(SDL_Surface));
	assert(s);
	s->flags = flags;
	s->format = malloc(sizeof(SDL_PixelFormat));
	assert(s->format);
	if (depth == 8) {
		s->format->palette = malloc(sizeof(SDL_Palette));
		assert(s->format->palette);
		s->format->palette->colors = malloc(sizeof(SDL_Color) * 256);
		assert(s->format->palette->colors);
		memset(s->format->palette->colors, 0, sizeof(SDL_Color) * 256);
		s->format->palette->ncolors = 256;
	} else {
		s->format->palette = NULL;
		s->format->Rmask = Rmask; s->format->Rshift = maskToShift(Rmask); s->format->Rloss = 0;
		s->format->Gmask = Gmask; s->format->Gshift = maskToShift(Gmask); s->format->Gloss = 0;
		s->format->Bmask = Bmask; s->format->Bshift = maskToShift(Bmask); s->format->Bloss = 0;
		s->format->Amask = Amask; s->format->Ashift = maskToShift(Amask); s->format->Aloss = 0;
	}

	s->format->BitsPerPixel = depth;
	s->format->BytesPerPixel = depth / 8;

	s->w = width;
	s->h = height;
	s->pitch = width * depth / 8;
	assert(s->pitch == width * s->format->BytesPerPixel);

	if (!(flags & SDL_PREALLOC)) {
		s->pixels = malloc(s->pitch * height);
		assert(s->pixels);
	}

	return s;
}

SDL_Surface* SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth,
		int pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
	SDL_Surface *s = SDL_CreateRGBSurface(SDL_PREALLOC, width, height, depth,
			Rmask, Gmask, Bmask, Amask);
	assert(pitch == s->pitch);
	s->pixels = pixels;
	return s;
}

void SDL_FreeSurface(SDL_Surface *s) {
	if (s != NULL) {
		if (s->format != NULL) {
			if (s->format->palette != NULL) {
				if (s->format->palette->colors != NULL) free(s->format->palette->colors);
				free(s->format->palette);
			}
			free(s->format);
		}
		if (s->pixels != NULL && !(s->flags & SDL_PREALLOC)) free(s->pixels);
		free(s);
	}
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
	if (flags & SDL_HWSURFACE) NDL_OpenCanvas(&width, &height);
	return SDL_CreateRGBSurface(flags, width, height, bpp,
			DEFAULT_RMASK, DEFAULT_GMASK, DEFAULT_BMASK, DEFAULT_AMASK);
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(src && dst);
	assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
	assert(dst->format->BitsPerPixel == 8);

	int x = (srcrect == NULL ? 0 : srcrect->x);
	int y = (srcrect == NULL ? 0 : srcrect->y);
	int w = (srcrect == NULL ? src->w : srcrect->w);
	int h = (srcrect == NULL ? src->h : srcrect->h);

	assert(dstrect);
	if(w == dstrect->w && h == dstrect->h) {
		/* The source rectangle and the destination rectangle
		 * are of the same size. If that is the case, there
		 * is no need to stretch, just copy. */
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_BlitSurface(src, &rect, dst, dstrect);
	}
	else {
		assert(0);
	}
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, int firstcolor, int ncolors) {
	assert(s);
	assert(s->format);
	assert(s->format->palette);
	assert(firstcolor == 0);

	s->format->palette->ncolors = ncolors;
	memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

	if(s->flags & SDL_HWSURFACE) {
		assert(ncolors == 256);
		for (int i = 0; i < ncolors; i ++) {
			uint8_t r = colors[i].r;
			uint8_t g = colors[i].g;
			uint8_t b = colors[i].b;
		}
		SDL_UpdateRect(s, 0, 0, 0, 0);
	}
}

static void ConvertPixelsARGB_ABGR(void *dst, void *src, int len) {
	int i;
	uint8_t (*pdst)[4] = dst;
	uint8_t (*psrc)[4] = src;
	union {
		uint8_t val8[4];
		uint32_t val32;
	} tmp;
	int first = len & ~0xf;
	for (i = 0; i < first; i += 16) {
#define macro(i) \
		tmp.val32 = *((uint32_t *)psrc[i]); \
		*((uint32_t *)pdst[i]) = tmp.val32; \
		pdst[i][0] = tmp.val8[2]; \
		pdst[i][2] = tmp.val8[0];

		macro(i + 0); macro(i + 1); macro(i + 2); macro(i + 3);
		macro(i + 4); macro(i + 5); macro(i + 6); macro(i + 7);
		macro(i + 8); macro(i + 9); macro(i +10); macro(i +11);
		macro(i +12); macro(i +13); macro(i +14); macro(i +15);
	}

	for (; i < len; i ++) {
		macro(i);
	}
}

SDL_Surface *SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, uint32_t flags) {
	assert(src->format->BitsPerPixel == 32);
	assert(src->w * src->format->BytesPerPixel == src->pitch);
	assert(src->format->BitsPerPixel == fmt->BitsPerPixel);

	SDL_Surface* ret = SDL_CreateRGBSurface(flags, src->w, src->h, fmt->BitsPerPixel,
			fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

	assert(fmt->Gmask == src->format->Gmask);
	assert(fmt->Amask == 0 || src->format->Amask == 0 || (fmt->Amask == src->format->Amask));
	ConvertPixelsARGB_ABGR(ret->pixels, src->pixels, src->w * src->h);

	return ret;
}

uint32_t SDL_MapRGBA(SDL_PixelFormat *fmt, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	assert(fmt->BytesPerPixel == 4);
	uint32_t p = (r << fmt->Rshift) | (g << fmt->Gshift) | (b << fmt->Bshift);
	if (fmt->Amask) p |= (a << fmt->Ashift);
	return p;
}

int SDL_LockSurface(SDL_Surface *s) {
	return 0;
}

void SDL_UnlockSurface(SDL_Surface *s) {
}
