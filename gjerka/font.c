#include "font.h"
#include "image.h"

static font FONT_MEMORY[8];

extern int64_t font_register0f64();

void fontManagerInit()
{
	font_register0f64();
}

void registerFont(uint64_t id, uint64_t basicPixelDimension, uint64_t pageSize, uint64_t pageCount, uint64_t base, const char** pages, void* resolver)
{
	font* f = &FONT_MEMORY[id];
	uint64_t i;
	f->baseScale = basicPixelDimension;
	f->resolver = resolver;
	f->texCout = pageCount;
	
	for (i = 0; i < pageCount; i++)
	{
		image* img = loadBitmap(pages[i]);
		loadTexture(&f->texArr[i], img);
		freeImage(img);
	}
}

font* getDefaultFont()
{
	font* ff = &FONT_MEMORY[0];
	return ff;
}

font* getFontById(uint64_t id)
{
	return &FONT_MEMORY[id];
}