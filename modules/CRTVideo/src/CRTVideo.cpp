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
	interlace(false);
}

void CRTVideo::interlace(bool var)
{
	bspDACInterlace(var);
}

void CRTVideo::writeChar(char c)
{
	if(c == '\n')
	{
		cursorY++;
		if(cursorY >= TEXT_MAP_HEIGHT - 1)
		{
			shiftTextUp();
			cursorY = TEXT_MAP_HEIGHT - 2;
		}
		//Disabled for rx on line feed only
//		return;
//	}
//	else if(c == '\r')
//	{
		cursorX = 1;
		return;
	}
	else if(c == 0x08)//backspace
	{
		cursorX--;
		if(cursorX <= 0)
		{
			//Special case, no up sroll
			if(cursorY <= 1)
			{
				cursorX = 1;
				return;
			}
			else if(cursorY > 1)
			{
				cursorY--;
				cursorX = TEXT_MAP_WIDTH - 2;
			}
		}
		textMap[(TEXT_MAP_WIDTH * cursorY) + cursorX] = ' ';
		return;
	}
	textMap[(TEXT_MAP_WIDTH * cursorY) + cursorX] = c;
	cursorX++;
	if(cursorX >= TEXT_MAP_WIDTH - 1)
	{
		cursorX = 1;
		cursorY++;
		if(cursorY >= TEXT_MAP_HEIGHT - 1)
		{
			shiftTextUp();
			cursorY = TEXT_MAP_HEIGHT - 2;
		}
	}
}

//Optional/example call to write text to screen.
//Alternatly, take your own frame and use console()
void CRTVideo::drawFrame(void)
{
	bspIOPinWrite(D31, 0);
	
	uint8_t textX = 1;
	uint8_t textY = 1;

	uint8_t * nextFrame = 0;
	if(bspDACGetBufferBlank(&nextFrame, 0x00)) //address of pointer
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

bool CRTVideo::getBlank(uint8_t ** output, uint8_t fill)
{
	if(bspDACGetBufferBlank(output, fill)) //address of pointer
	{
		return true;
	}
	return false;
}

void CRTVideo::swap(void)
{
	bspDACSwapBuffers();
}

void CRTVideo::console(uint8_t * dst)
{
	uint8_t textX = 1;
	uint8_t textY = 1;

	for(textY = 0; textY < TEXT_MAP_HEIGHT; textY++)
	{
		for(textX = 0; textX < TEXT_MAP_WIDTH; textX++)
		{
			writeAscii5x7(dst, textX * 6, textY * 8, textMap[(textY * TEXT_MAP_WIDTH) + textX]);
		}
	}
}

bool CRTVideo::pixel(uint8_t * dst, uint8_t x, uint8_t y, uint8_t value)
{
	if((x >= PIXEL_WIDTH)||(y >= PIXEL_HEIGHT))
	{
		return false;
	}
	dst[(y * PIXEL_WIDTH) + x] = value;
		
	return true;
}

bool CRTVideo::line(uint8_t * dst, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value)
{
	//if((x1 > x2)||(y1 > y2))
	//{
	//	return false;
	//}
	//draw vertical segments on each x
	//Swap x1, x2 if needed
	
	if(x1 < x2)
	{
		for(int i = x1; i < x2; i++)
		{
			if(y1 < y2)
			{
				float yStart = y1 + ((float)(i - x1) / (float)(x2 - x1)) * (float)(y2 - y1);
				float yEnd = y1 + ((float)(i - x1 + 1) / (float)(x2 - x1)) * (float)(y2 - y1);
				for(int j = yStart; j <= yEnd; j++)
				{
					pixel(dst, i, j, value);
				}
			}
			else
			{
				float yStart = y1 - ((float)(i - x1) / (float)(x2 - x1)) * (float)(y1 - y2);
				float yEnd = y1 - ((float)(i - x1 + 1) / (float)(x2 - x1)) * (float)(y1 - y2);
				for(int j = yStart; j >= yEnd; j--)
				{
					pixel(dst, i, j, value);
				}
			}
		}
	}
	else
	{
		for(int i = x2; i < x1; i++)
		{
			if(y1 < y2)
			{
				float yStart = (float)y2 - ((float)(i - x2) / (float)(x1 - x2)) * (float)(y2 - y1);
				float yEnd = (float)y2 - ((float)(i - x2 + 1) / (float)(x1 - x2)) * (float)(y2 - y1);
				for(int j = yStart; j >= yEnd; j--)
				{
					pixel(dst, i, j, value);
				}
			}
			else
			{
				float yStart = (float)y2 + ((float)(i - x2) / (float)(x1 - x2)) * (float)(y1 - y2);
				float yEnd = (float)y2 + ((float)(i - x2 + 1) / (float)(x1 - x2)) * (float)(y1 - y2);
				for(int j = yStart; j <= yEnd; j++)
				{
					pixel(dst, i, j, value);
				}
			}
		}
	}
	
	
	// eh?
	
	//if(x1 > x2)
	//{
	//	int tmp = x1;
	//	x1 = x2;
	//	x2 = tmp;
	//	tmp = y1;
	//	y1 = y2;
	//	y2 = tmp;
	//}
	//for( int i = 0; i <= (x2 - x1); i++ )
	//{
	//	int16_t start;
	//	int16_t end;
	//	if(y2 > y1)
	//	{
	//		start = y1 + (((float)i/(float)(x2 - x1)) * (float)(y2 - y1));
	//		end = y1 + (((float)(i + 1)/(float)(x2 - x1)) * (float)(y2 - y1));
	//		for( int j = start; j <= end; j++ )
	//		{
	//			pixel(dst, x1 + i, j, 0xFF);
	//		}
	//	}
	//	else
	//	{
	//		start = y2 + (((float)i/(float)(x2 - x1)) * (float)(y1 - y2));
	//		end = y2 + (((float)(i + 1)/(float)(x2 - x1)) * (float)(y1 - y2));
	//		for( int j = start; j <= end; j++ )
	//		{
	//			pixel(dst, x1 + i, j, 0xFF);
	//		}
	//	}
	//}
	return true;
}

bool CRTVideo::drawTile(uint8_t * dst, int16_t tileNumber, bitmap_file_t * srcFile, int16_t x, int16_t y)
{
	//Find x and y of indexed area
	int xSrc = tileNumber * srcFile->divWidth;
	while( xSrc >= PIXEL_WIDTH )
		xSrc -= PIXEL_WIDTH;
	int ySrc = srcFile->divHeight * (tileNumber/(PIXEL_WIDTH/srcFile->divWidth));
	
	//Scan all sprite pixels and draw into destination
	for(int iY = 0; iY < srcFile->divHeight; iY++)
	{
		for(int iX = 0; iX < srcFile->divWidth; iX++)
		{
			int16_t sourcePixel = ((iY + ySrc) * srcFile->width) + iX + xSrc;
			int16_t destX = iX + x;
			int16_t destY = iY + y;
			
			if((srcFile->data[sourcePixel] != 0xFF) &&
				(destX >= 0) &&
				(destX < PIXEL_WIDTH) &&
				(destY >= 0) &&
				(destY < 144))
			{
				dst[(destY * PIXEL_WIDTH) + destX] =
					((srcFile->data[sourcePixel]) >> 2)
					+ ASCII_BLACK_LEVEL;
			}
		}
	}
	return true;
}

bool CRTVideo::drawBitmap(uint8_t * dst, virtual_bitmap_type_t * pBitmap, int16_t x, int16_t y)
{
	if((x >= PIXEL_WIDTH)||(y >= PIXEL_HEIGHT))
	{
		return false;
	}
	switch(pBitmap->type)
	{
		case BITMAP_1X1:
		{
			drawTile(dst, pBitmap->data[0], pBitmap->srcFile, x, y);
		}
		break;
		case BITMAP_1X2:
		{
			drawTile(dst, pBitmap->data[0], pBitmap->srcFile, x, y);
			drawTile(dst, pBitmap->data[1], pBitmap->srcFile, x, y + 32);
		}
		break;
		case BITMAP_2X3:
		{
			drawTile(dst, pBitmap->data[0], pBitmap->srcFile, x, y);
			drawTile(dst, pBitmap->data[1], pBitmap->srcFile, x + 32, y);
			drawTile(dst, pBitmap->data[2], pBitmap->srcFile, x, y + 32);
			drawTile(dst, pBitmap->data[3], pBitmap->srcFile, x + 32, y + 32);
			drawTile(dst, pBitmap->data[4], pBitmap->srcFile, x, y + 64);
			drawTile(dst, pBitmap->data[5], pBitmap->srcFile, x + 32, y + 64);
		}
		break;
		default:
		{
			return false;
		}
		break;
	}

	return true;
}

void CRTVideo::shiftTextUp(void)
{
	int i;
	for(i = 1; i < TEXT_MAP_HEIGHT - 1; i++)
	{
		memcpy(&textMap[(i*TEXT_MAP_WIDTH)], &textMap[((i+1)*TEXT_MAP_WIDTH)], TEXT_MAP_WIDTH);
	}
	memset(&textMap[((i-1)*TEXT_MAP_WIDTH)], ' ', TEXT_MAP_WIDTH);
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
