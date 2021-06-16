#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "fonts.h"

class TextBuffer
{
public:
    TextBuffer(char * d, uint16_t sz);
    void clear(void);
    void write(char c);
    void write(const char * str);
    uint16_t bytesUsed(void);
	uint16_t bytesFree(void);
    char getChar(uint16_t i);
	void setCircularMode(bool cEnb);
private:
    uint16_t start = 0;
    uint16_t nextToWrite = 0;
    uint16_t length;
	bool circularMode = false;
    char * data;
};

class TextBox
{
public:
    TextBox(void);
    void box(int32_t x1, int32_t y1, int32_t w1, int32_t h1);
    void setFont(font_t * f);
    void setDriver(CRTVideo * drv);
    void setTextBuffer(TextBuffer * f);
	uint16_t getHeightInLines(void);
	uint16_t countLines(void);
    uint16_t indexCharAtLine(int16_t lineNum);
    void draw(uint8_t * dst);
    void draw(uint8_t * dst, int16_t lineFromStart);

private:
    CRTVideo * driver;
    TextBuffer * data;
    int32_t xPos;
    int32_t yPos;
    int32_t wPx;
    int32_t hPx;
    uint8_t wC;
    uint8_t hC;
    font_t * font;
};

#endif
