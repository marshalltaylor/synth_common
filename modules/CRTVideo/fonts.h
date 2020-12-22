#ifndef FONTS_H
#define FONTS_H

#include "font8x11.h"
#include "font5x7.h"
#include "font5x6.h"

#define ASCII_BLACK_LEVEL 98

#define NUM_FONTS 3

extern uint8_t asciiLUT[];
//extern const unsigned char font5x6[];

void calculateAsciiLUT(void);

typedef struct font_t
{
	uint8_t charWPx;
	uint8_t charHPx;
	char startC;
	uint8_t lenC;
	char * data;
} font_t;

void drawChar(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c);

#endif
