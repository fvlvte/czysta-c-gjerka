#pragma once
#include <stdint.h>
#pragma once

#include "render.h"

struct _tile
{
	texture*	tileset;
	rect		tilesetOffset;
	uint64_t	flags;
};
typedef struct _tile tile;

struct __world
{
	uint64_t tileResolution;

	uint64_t layerCount;
	
	uint64_t tileWidth;
	uint64_t tileHeight;

	tile** layers;
};
typedef struct __world wworld;

wworld* createWorld(uint64_t tileResolution, uint64_t width, uint64_t height, uint64_t layers);
void renderWorld(wworld* world, float cameraX, float cameraY, float cameraWidth, float cameraHeight);