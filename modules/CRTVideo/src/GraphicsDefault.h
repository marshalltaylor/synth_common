#ifndef GRAPHICS_DEFAULT_H
#define GRAPHICS_DEFAULT_H
#ifdef __cplusplus
extern "C" {
#endif

#define ASCII_BLACK_LEVEL 98

extern uint8_t asciiLUT[];

void calculateAsciiLUT(void);

#ifdef __cplusplus
}
#endif
#endif