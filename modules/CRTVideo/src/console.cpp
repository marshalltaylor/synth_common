#error File not finished, do not includes

#include "console.h"

void Console::writeChar(char c)
{
	if(c == '\n')
	{
		cursorY++;
		if(cursorY >= 10 - 1)
		{
			shiftTextUp();
			cursorY = 10 - 2;
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
				cursorX = 20 - 2;
			}
		}
		textMap[(20 * cursorY) + cursorX] = ' ';
		return;
	}
	textMap[(20 * cursorY) + cursorX] = c;
	cursorX++;
	if(cursorX >= 20 - 1)
	{
		cursorX = 1;
		cursorY++;
		if(cursorY >= 10 - 1)
		{
			shiftTextUp();
			cursorY = 10 - 2;
		}
	}
}

//Optional/example call to write text to screen.
//Alternatly, take your own frame and use console()
void Console::drawFrame(void)
{
	bspIOPinWrite(D31, 0);
	
	uint8_t textX = 1;
	uint8_t textY = 1;

	uint8_t * nextFrame = 0;
	if(bspDACGetBufferBlank(&nextFrame, 0x00)) //address of pointer
	{
		for(textY = 0; textY < 10; textY++)
		{
			for(textX = 0; textX < 20; textX++)
			{
				writeAscii(nextFrame, textX * fonts[fSel].charWPx, textY * fonts[fSel].charHPx, textMap[(textY * 20) + textX]);
			}
		}
		bspDACSwapBuffers();
	}
	bspIOPinWrite(D31, 1);
}


void Console::drawData(uint8_t * dst)
{
	uint8_t textX = 1;
	uint8_t textY = 1;

	for(textY = 0; textY < 10; textY++)
	{
		for(textX = 0; textX < 20; textX++)
		{
			writeAscii(dst, textX * fonts[fSel].charWPx, textY * fonts[fSel].charHPx, textMap[(textY * 20) + textX]);
		}
	}

}

void Console::shiftTextUp(void)
{
	int i;
	for(i = 1; i < 10 - 1; i++)
	{
		memcpy(&textMap[(i*20)], &textMap[((i+1)*20)], 20);
	}
	memset(&textMap[((i-1)*20)], ' ', 20);
}

void Console::clearConsole(void)
{
	memset(textMap, ' ', CONSOLE_CHAR_BUFFER_LENGTH);
	cursorX = 1;
	cursorY = 0;
}

