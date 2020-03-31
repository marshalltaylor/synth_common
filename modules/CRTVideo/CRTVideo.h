#ifndef CRTVIDEO_H
#define CRTVIDEO_H
#include <stdint.h>

#define TEXT_MAP_HEIGHT 18
#define TEXT_MAP_WIDTH 30

class CRTVideo
{
public:
	CRTVideo(void);
	void init(void);
	void writeChar(char c);
	void drawFrame(void);
private:
	void shiftTextUp(void);
	void writeAscii5x7(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c);

	uint8_t cursorX = 0;
	uint8_t cursorY = 0;
	char textMap[TEXT_MAP_HEIGHT*TEXT_MAP_WIDTH];
};

#endif