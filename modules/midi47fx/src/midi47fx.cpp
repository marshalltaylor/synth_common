#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp.h"

#include "midi47fx.h"

//functions
HardwareSerial::HardwareSerial(void)
{
}

void HardwareSerial::initPort(comPort_t port)
{
	if(port >= NUM_COM_PORTS)
	{
		while(1);
	}

	bspGetSerialFunctions(port, &bspSerialMidiObj);

	if(( bspSerialMidiObj.bytesAvailable == NULL )
				|| ( bspSerialMidiObj.peek == NULL )
				|| ( bspSerialMidiObj.read == NULL )
				|| ( bspSerialMidiObj.write == NULL ))
	{
		while(1);
	}
}

void HardwareSerial::begin(unsigned long baud)
{
}

int HardwareSerial::available(void)
{
	return bspSerialMidiObj.bytesAvailable();
}

int HardwareSerial::peek(void)
{
	return bspSerialMidiObj.peek();
}

int HardwareSerial::read(void)
{
	return bspSerialMidiObj.read();
}

size_t HardwareSerial::write(uint8_t c)
{
	bspSerialMidiObj.write(c);
	return 1;
}
