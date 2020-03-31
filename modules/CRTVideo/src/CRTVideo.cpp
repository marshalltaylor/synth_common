// bsp
#include "bsp.h"

// driver
#include "CRTVideo.h"
#include "GraphicsDefault.h"

// normal_distribution
#include <iostream>
#include <string>
#include <random>
#include <string.h>

//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf consoleDebug.printf
Logging consoleDebug;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

CRTVideo::CRTVideo(void)
{
}

void CRTVideo::init(void)
{
	calculateAsciiLUT();
	localPrintf("\n");
	int offset = ('A'-0x20) * 48;
	for(int i = offset; i < offset + 200; i++)
	{
		localPrintf("0x%02X ", asciiLUT[i]);
		if(((i+1) % 6) == 0) localPrintf("\n");
	}
	bspDACInit();
	bspDACInterlace(false);
}

void CRTVideo::writeChar(char c)
{
	if(c == '\n')
	{
		if(cursorY >= TEXT_MAP_HEIGHT - 1)
		{
			shiftTextUp();
			cursorY = TEXT_MAP_HEIGHT - 1;
		}
		else
		{
			cursorY++;
		}
		return;
	}
	else if(c == '\r')
	{
		cursorX = 0;
		return;
	}
	
	textMap[(TEXT_MAP_WIDTH * cursorY) + cursorX] = c;
	cursorX++;
	if(cursorX >= TEXT_MAP_WIDTH)
	{
		cursorX = 0;
		cursorY++;
		if(cursorY >= TEXT_MAP_HEIGHT)
		{
			shiftTextUp();
			cursorY = TEXT_MAP_HEIGHT - 1;
		}
	}
}

void CRTVideo::drawFrame(void)
{
	bspIOPinWrite(D31, 0);
	
	uint8_t textX = 0;
	uint8_t textY = 0;

	uint8_t * nextFrame = 0;
	if(bspDACGetBufferBlank(&nextFrame)) //address of pointer
	{
		for(textY = 0; textY < TEXT_MAP_HEIGHT; textY++)
		{
			for(textX = 0; textX < TEXT_MAP_WIDTH; textX++)
			{
				writeAscii5x7(nextFrame, textX * 6, textY * 8, textMap[(textY * TEXT_MAP_WIDTH) + textX]);
			}
		}
		bspDACSwapBuffers();
	}
	bspIOPinWrite(D31, 1);
}

void CRTVideo::shiftTextUp(void)
{
	int i;
	for(i = 0; i < TEXT_MAP_HEIGHT - 1; i++)
	{
		memcpy(&textMap[(i*TEXT_MAP_WIDTH)], &textMap[((i+1)*TEXT_MAP_WIDTH)], TEXT_MAP_WIDTH);
	}
	memset(&textMap[((i+1)*TEXT_MAP_WIDTH)], ' ', TEXT_MAP_WIDTH);
}

void CRTVideo::writeAscii5x7(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c)
{
	int line;
	if((c >= 0x20)&&(c < 0x7F))
	{
		//printable
	}
	else
	{
		c = 0x20; //default print space
	}
	
	for(line = 0; line < 8; line++)
	{
		//8 * 6 = 48
		memcpy(&dst[((dstY + line) * 192) + dstX], &asciiLUT[((c-0x20) * 48) + (line * 6)], 6);
	}
}
