#ifndef BITMAPS_H
#define BITMAPS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct bitmap
{
	uint8_t * data;
	uint32_t width;
	uint32_t height;
} bitmap_t;

typedef struct sprite
{
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	bitmap_t * pBitmapSource;
	uint8_t nFrames;
} sprite_t;

#ifdef __cplusplus
}
#endif
#endif
