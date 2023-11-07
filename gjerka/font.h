#pragma once
#include <stdint.h>
#include "render.h"

struct _kerning
{
	uint64_t nextGlyph;
	int64_t adjustment;
};
typedef struct _kerning kerning;

struct _glyph
{
	int64_t width;
	int64_t height;
	int64_t x;
	int64_t y;
	int64_t xoff;
	int64_t yoff;
	int64_t xadv;
	int64_t page;
	kerning* kernings;
};
typedef struct _glyph glyph;

typedef glyph* (*_resolve_glyph)(uint64_t glyph);

typedef struct _font
{
	_resolve_glyph resolver;
	uint64_t id;
	uint64_t baseScale;
	uint64_t texCout;
	texture texArr[8];
} font;

void fontManagerInit();
void registerFont(uint64_t id, uint64_t basicPixelDimension, uint64_t pageSize, uint64_t pageCount, uint64_t base, const char** pages, void* resolver);
font* getDefaultFont();
font* getFontById(uint64_t id);