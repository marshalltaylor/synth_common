#ifndef CRTVIDEO_H
#define CRTVIDEO_H
#include <stdint.h>

#include "bitmaps.h"
#include "fonts.h"

#define PIXEL_WIDTH 192
#define PIXEL_HEIGHT 144
#define PIXEL_TOT (PIXEL_WIDTH * PIXEL_HEIGHT)
#define NUM_FONTS 2
// Console buffer needs to fit largest resolution font
#define CONSOLE_CHAR_BUFFER_LENGTH (39 * 24)

class CRTVideo
{
public:
	CRTVideo(void);
	//General settings
	void interlace(bool var);

	//Page operations
	bool getBlank(uint8_t ** output, uint8_t fill);
	void swap(void);

	//Graphics drawing
	bool pixel(uint8_t * dst, uint8_t x, uint8_t y, uint8_t value);
	bool line(uint8_t * dst, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value);
	void box(uint8_t * dst, int32_t x1, int32_t y1, int32_t w1, int32_t h1, uint8_t data);
	bool drawTile(uint8_t * dst, int16_t tileNumber, bitmap_file_t * srcFile, int16_t x, int16_t y);
	bool drawBitmap(uint8_t * dst, virtual_bitmap_type_t * src, int16_t x, int16_t y);
	
	//Character drawing
	void drawAscii(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c, font_t * font);
	void drawAscii(uint8_t * dst, uint8_t xOrigin, uint8_t yOrigin, const char * str, uint8_t xChar, uint8_t yChar, font_t * font);
private:
};

#endif