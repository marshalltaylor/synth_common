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
    uint16_t indexLastNLines(int16_t lines, uint16_t width);
    char getChar(uint16_t i);
private:
    uint16_t start = 0;
    uint16_t nextToWrite = 0;
    uint16_t length;
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
    void setScroll(int16_t value);
    void draw(uint8_t * dst);
private:
    CRTVideo * driver;
    TextBuffer * data;
    int32_t xPos;
    int32_t yPos;
    int32_t wPx;
    int32_t hPx;
    uint8_t wC;
    uint8_t hC;
    int16_t scroll;
    font_t * font;
};

#endif
