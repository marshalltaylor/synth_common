#include <stdint.h>
#include "font5x7.h"
#include "GraphicsDefault.h"

uint8_t asciiLUT[6*8*100];

void calculateAsciiLUT(void)
{
	//Start at ascii 0x20, space
	int asciiChar;
	int byteNum;
	int bitNum;
	const unsigned char * dataHead;
	char dataByte;
	for(asciiChar = 0x20; asciiChar < 0x7F; asciiChar++)
	{
		dataHead = &font5x7[6 + (5 * asciiChar)];
		for(byteNum = 0; byteNum < 6; byteNum++)
		{
			dataByte = (char)(dataHead[byteNum]);
			if(byteNum == 5) // overwrite blank bar
			{
				dataByte = 0x00;
			}
			for(bitNum = 0; bitNum < 8; bitNum++)
			{
				if(((dataByte >> bitNum) & 0x01) == 0x01)
				{
					asciiLUT[((asciiChar - 0x20) * 48) + (bitNum * 6) + byteNum] = 0xFF;
				}
				else
				{
					asciiLUT[((asciiChar - 0x20) * 48) + (bitNum * 6) + byteNum] = ASCII_BLACK_LEVEL;
				}
			}
		}
	}
}