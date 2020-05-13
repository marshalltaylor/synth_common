#ifndef BITMAPS_H
#define BITMAPS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/***** File *****/
typedef struct bitmap_file
{
	uint16_t width;
	uint16_t height;
	//divWidth and divHorCount should be mathematically consistent 
	uint16_t divWidth;
	uint16_t divHeight;
	uint8_t * data;
} bitmap_file_t;

/***** Base Classes *****/
// Sprite

class Sprite
{
public:
	Sprite(void){};
	uint16_t xPos;
	uint16_t yPos;
	uint16_t width;
	uint16_t height;
	bitmap_file_t * srcFile;
	Sprite * prevSprite = 0;
	Sprite * nextSprite = 0;
};

// Layer
typedef struct layer
{
	int16_t xOffset;
	int16_t yOffset;
	uint16_t width;
	uint16_t height;
	Sprite * spriteLL;// = NULL;
} layer_t;

#ifdef __cplusplus
}
#endif
#endif
