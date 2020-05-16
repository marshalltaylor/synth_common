#ifndef CRTVIDEO_H
#define CRTVIDEO_H
#include <stdint.h>

#include "bitmaps.h"

#define TEXT_MAP_HEIGHT 18
#define TEXT_MAP_WIDTH 30

#define PIXEL_WIDTH 192
#define PIXEL_HEIGHT 144

class CRTVideo
{
public:
	CRTVideo(void);
	void init(void);
	void writeChar(char c);
	void drawFrame(void);
	bool getBlank(uint8_t ** output, uint8_t fill);
	void swap(void);
	void console(uint8_t * dst);
	bool pixel(uint8_t * dst, uint8_t x, uint8_t y, uint8_t value);
	bool line(uint8_t * dst, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value);
	//void setTextRegion(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2); 
	bool drawBitmap(uint8_t * dst, virtual_bitmap_type_t * src, int16_t x, int16_t y);
	
private:
	void shiftTextUp(void);
	void writeAscii5x7(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c);

	uint8_t cursorX = 1;
	uint8_t cursorY = 1;
	char textMap[TEXT_MAP_HEIGHT*TEXT_MAP_WIDTH];
};

#endif