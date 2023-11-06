#pragma once

#include "window.h"
#include "image.h"
#include <stdint.h>

#define TEXTURE_FLAG_ALPHA_BLEND 0b1

#define RENDER_TEX_OFFSET_PX 0b1

#define ENTITY_PLAYER 1

struct _abstractTexture
{
	uint64_t	pixelWidth;
	uint64_t	pixelHeight;
	uint64_t	flags;
	void*		systemDescriptor;

};
typedef struct _abstractTexture texture;

struct _color
{
	float r;
	float g;
	float b;
	float a;
};
typedef struct _color color;

struct _rect
{
	float x;
	float y;
	float w;
	float h;
};
typedef struct _rect rect;

void beginFrame(window* w);
void endFrame(window* w);

#define TEXTURE_PIXEL_FORMAT_RGBA 0b1

void loadTexture(texture* out, image* i);
void freeTexture(texture* in);

void drawRectangle(texture* tex, rect* textureCoords, rect* coords, color* c, uint64_t flags);
