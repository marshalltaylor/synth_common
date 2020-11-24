// bsp
#include "bsp.h"

// driver
#include "CRTVideo.h"
#include "GraphicsDefault.h"
#include "fonts.h"
#include "textBox.h"

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

TextBuffer::TextBuffer(char * d, uint16_t sz)
{
    data = d;
    length = sz;
}

void TextBuffer::clear(void)
{
    start = 0;
    nextToWrite = 0;
    data[0] = 0; //cheaper than memset.
}

void TextBuffer::write(char c)
{
    uint16_t advNext = nextToWrite + 1;
    if(advNext >= length) advNext -= length;
    if(advNext == start)
    {
        //overwriting old data
        start++;
        if(start >= length) start -= length;
        data[advNext] = 0;
    }
    //could return here if 'no circular' set
    //Ok, now we have space, write it in.
    data[nextToWrite] = c;
    nextToWrite = advNext;
}

void TextBuffer::write(const char * str)
{
	while(*str != 0)
	{
		write(*str);
		str++;
	}
}

//  in, out,  (sub), delta
//   3,   5,      2,     2
//   299, 2,   -297,     3
//     9, 8,     -1,   299
// and on a (length - 1) we are full.
uint16_t TextBuffer::bytesUsed(void)
{
    int32_t indexDelta = nextToWrite - start;
    if(indexDelta < 0)
    {
        //rolled
        indexDelta += length;
    }
    return indexDelta;
}

//uint16_t TextBuffer::indexLastNLines(uint16_t lines, uint16_t width)
//{
//    if(!data || (nextToWrite == start))
//    {
//        return 0;
//    }
//    int32_t index = bytesUsed() - 1;
//    int32_t lineCtr = 0;
//    int32_t charCtr = 0;
//    //count '\n'
//    for(int i = index; i >= 0; i--)
//    {
//        if(getChar(i) == '\n')
//        {
//            lineCtr += charCtr / width;
//            charCtr = 0;
//            lineCtr++;
//        }
//        else
//        {
//            charCtr++;
//            if(charCtr == width)
//            {
//                charCtr = 0;
//                lineCtr++;
//            }
//        }
//        if(lineCtr == lines)
//        {
//            return i;
//        }
//    }
//    return 0;
//}

char TextBuffer::getChar(uint16_t i)
{
    i += start;
    if(i >= length)
    {
        //rolled
        i -= length;
    }
    return data[i];
}

TextBox::TextBox(void)
{
    data = NULL;
    font = NULL;
    xPos = 0;
    yPos = 0;
    wPx = 0;
    hPx = 0;
    wC = 0;
    hC = 0;
}

void TextBox::box(int32_t x1, int32_t y1, int32_t w1, int32_t h1)
{
    if(!data || !font)
    {
        return;
    }
    //No error checking on geometry!!!
    xPos = x1;
    yPos = y1;
    wPx = w1;
    hPx = h1;
    wC = wPx / font->charWPx;
    hC = hPx / font->charHPx;
}

void TextBox::setFont(font_t * f)
{
    font = f;
    wC = wPx / font->charWPx;
    hC = hPx / font->charHPx;
}

void TextBox::setDriver(CRTVideo * drv)
{
    driver = drv;
}

void TextBox::setTextBuffer(TextBuffer * b)
{
    data = b;
}

uint16_t TextBox::getHeightInLines(void)
{
	return hC;
}

uint16_t TextBox::countLines(void)
{
	//Start at the beginning of the document and scan, keeping track of the nth from last item
	int lastChar = data->bytesUsed() - 1;
    int i = 0;

    int charCtr = 0;

    // 1.  Count all lines in the document
    int lineCtr = 1; // a line with no breaks or ends is still a line
    for(i = 0; i < lastChar; i++)
    {
        if(data->getChar(i) == '\n')
        {
			//bspPrintf(" N@%d", i);
            charCtr = 0;
            lineCtr++;
        }
        else
        {
            charCtr++;
            if(charCtr == wC)
            {
				//bspPrintf(" %d", i + 1);
                charCtr = 0;
                lineCtr++;
            }
        }
	}
	return lineCtr;
}

uint16_t TextBox::indexCharAtLine(int16_t lineNum)
{
	if(lineNum < 0) lineNum = 0;
	//Start at the beginning of the document and scan, keeping track of the nth from last item
	int firstChar = 0;
	int lastChar = data->bytesUsed() - 1;
    int i = 0;

    int charCtr = 0;
	int lineCtr = 1;

	while((i < lastChar)&&(lineCtr < lineNum))
    {
		char c = data->getChar(i);
        if(c == '\n')
        {
            charCtr = 0;
            lineCtr++;
			firstChar = i + 1;
        }
        else
        {
            charCtr++;
            if(charCtr == wC)
            {
                charCtr = 0;
                lineCtr++;
				firstChar = i + 1;
            }
        }
		i++;
	}
	//if((firstChar < 0)||(firstChar >= lastChar))
	//{
	//	while(1);
	//	//bspPrintf("Error: %d, %d\n", firstChar, lastChar);
	//	return 0;
	//}
	//bspPrintf("AT return, %d, %d\n", firstChar, lastChar);
    return firstChar;
}

void TextBox::draw(uint8_t * dst)
{
	//Auto view
	draw(dst, countLines() + 1 - hC);
}

void TextBox::draw(uint8_t * dst, int16_t lineFromStart)
{
    if(!data || !font || (lineFromStart > countLines()))
    {
        return;
    }
    int dataLen = data->bytesUsed();

    int readChar = indexCharAtLine(lineFromStart);
    int xPx = xPos;
    int yPx = yPos;
    int lineCtr = 0;
    //bspPrintf("TEST %d, %d, %d, %d, \n", dataLen, readChar, xPx, yPx);
    for(; (readChar < dataLen)&&(lineCtr < hC); readChar++)
    {
        char c = data->getChar(readChar);
        if(c == '\n')
        {
            yPx += font->charHPx;
            xPx = xPos;
            lineCtr++;
        }
        else
        {
            //draw
            driver->drawAscii(dst, xPx, yPx, c, font);
            //move
            xPx += font->charWPx;
            if(xPx > (wPx - font->charWPx) + xPos)
            {
                yPx += font->charHPx;
                xPx = xPos;
                lineCtr++;
            }
        }
    }
    //bspPrintf("     %d, %d, %d, %d, \n", dataLen, readChar, lineCtr, hC);
}
