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

struct _serializedWorldTilesetTable
{
	uint64_t		id;
	uint8_t			assetPath[256];
};

struct _serializedWorldHdr
{
	uint64_t magic;
	uint64_t version;

	uint64_t tilesetCount;
	// asset table
	uint64_t tileWidth;
	uint64_t tileHeight;


};
typedef struct _serializedWorldHdr serializedWorldHeader;

struct _world_editor_state
{
	uint64_t selectedX;
	uint64_t selectedY;
};
typedef struct _world_editor_state editor_state;

wworld* createWorld(uint64_t tileResolution, uint64_t width, uint64_t height, uint64_t layers);
void renderWorldEditor(wworld* world, editor_state* state);
void renderWorld(wworld* world, float cameraX, float cameraY, float cameraWidth, float cameraHeight);