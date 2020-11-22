// bsp
#include "bsp.h"

// driver
#include "CRTVideo.h"
#include "GraphicsDefault.h"
#include "font5x6.h"

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

void CRTVideo::interlace(bool var)
{
	bspDACInterlace(var);
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

void CRTVideo::drawAscii(uint8_t * dst, uint8_t dstX, uint8_t dstY, char c, font_t * font)
{
	int line;
	if((c >= font->startC)&&(c < (0x20 + font->lenC)))
	{
		//printable
	}
	else
	{
		c = font->startC; //default print char
	}
	
	int pxPerC = font->charWPx * font->charHPx;
	for(line = 0; line < font->charHPx; line++)
	{
        int writeIndex = ((dstY + line) * PIXEL_WIDTH) + dstX;
        if(writeIndex < PIXEL_TOT - font->charWPx)
        {
            //weak check, at least it's on the array data
            memcpy(&dst[writeIndex], &(font->data[((c-font->startC) * pxPerC) + (line * font->charWPx)]), font->charWPx);
        }
	}
}

void CRTVideo::drawAscii(uint8_t * dst, uint8_t xOrigin, uint8_t yOrigin, const char * str, uint8_t xChar, uint8_t yChar, font_t * font)
{
	int i = 0;
	yOrigin = yOrigin + (yChar * font->charHPx);
	while(*str != 0)
	{
		drawAscii(dst, xOrigin + ((i + xChar) * font->charWPx), yOrigin, *str, font);
		i++;
		str++;
	}
}
