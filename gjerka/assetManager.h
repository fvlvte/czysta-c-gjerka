#pragma once

#include <stdint.h>
#include "render.h"

#define ASSET_TYPE_TEXTURE_2D	0x00
#define ASSET_TYPE_SPRITE_2D	0x01

enum AssetTable
{
	UI_TEXTURE = 0x00,

	ASSET_TABLE_END
};

#define ASSET_MANAGER_MAX 2137

#define ASSET_LOADED_MASK 0x21372137

#define ASSET_UI_COMMON 0x21

struct _abstractAssetEntry
{
	uint64_t id;
	uint64_t type;
	uint64_t mask;
	void* descriptor;
};
typedef struct _abstractAssetEntry abstractAssetEntry;

struct _assetManager
{
	struct _abstractAssetEntry table[ASSET_MANAGER_MAX];
};
typedef struct _assetManager assetManager;

void assetManagerInit(assetManager* in);
void assetManagerLoadTexture(assetManager* in, uint64_t assetId, const char* path);
abstractAssetEntry* assetManagerFetch(assetManager* in, uint64_t assetId);