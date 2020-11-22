#error File not finished, do not includes

#ifndef CONSOLE_H
#define CONSOLE_H

#ifndef CRTVIDEO_H
#define CRTVIDEO_H
#include <stdint.h>

#include "CRTVideo.h"

// Console buffer needs to fit largest resolution font
#define CONSOLE_CHAR_BUFFER_LENGTH (39 * 24)

class Console
{
public:
	Console(void);
	void init(void);
	void writeChar(char c);
	void drawFrame(void);
	void drawData(uint8_t * dst);
	
	void setFont(uint8_t fontNum);
	void clearConsole(void);
	void writeAscii(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c);
	void writeStringAlign(uint8_t * dst, uint8_t xOrigin, uint8_t yOrigin, const char * str, uint8_t xChar, uint8_t yChar);
private:
	void shiftTextUp(void);

	uint8_t fSel = 0;
	uint8_t cursorX = 1;
	uint8_t cursorY = 1;
	char textMap[CONSOLE_CHAR_BUFFER_LENGTH];
};

#endif
