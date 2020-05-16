#ifndef BITMAPS_H
#define BITMAPS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/***** File *****/
typedef struct bitmap_file
{
	uint16_t width;
	uint16_t height;
	//divWidth and divHorCount should be mathematically consistent 
	uint16_t divWidth;
	uint16_t divHeight;
	uint8_t * data;
} bitmap_file_t;

typedef enum bitmap_type
{
	BITMAP_1X1 = 0,
	BITMAP_1X2 = 1,
	BITMAP_2X3 = 2,
} bitmap_type_t;

typedef struct virtual_bitmap_type
{
	bitmap_type_t type;
	bitmap_file_t * srcFile;
	uint8_t * data;
} virtual_bitmap_type_t;

typedef struct basic_bitmap_type : public virtual_bitmap_type
{
	uint8_t tileTable;
} basic_bitmap_type_t;

typedef struct tall_bitmap_type : public virtual_bitmap_type
{
	uint8_t tileTable[2];
} tall_bitmap_type_t;

typedef struct large_tall_bitmap_type : public virtual_bitmap_type
{
	uint8_t tileTable[6];
} large_tall_bitmap_type_t;

#ifdef __cplusplus
}
#endif
#endif
