#pragma once
#include <stdint.h>

#define IMAGE_FORMAT_RGBA8 1

struct image
{
	uint64_t width;
	uint64_t height;
	void* data;
	uint64_t format;
};
typedef struct image image;

#pragma pack(push, 1)
struct _header
{
	uint8_t		magic1;
	uint8_t		magic2;
	uint32_t	size;
	uint32_t	reserved;
	uint32_t	offBits;
};

struct _info
{
	uint32_t	size;
	uint32_t	width;
	uint32_t	height;
	uint16_t	planeCount;
	uint16_t	bitCount;
	uint32_t	compression;
	uint32_t	sizeImage;
	uint32_t	xpelsPerMeter;
	uint32_t	ypelsPerMeter;
	uint32_t	clrUsed;
	uint32_t	clrImportant;
};
#pragma pack(pop)

image*	loadBitmap(const char* path);
void	freeImage(image* img);