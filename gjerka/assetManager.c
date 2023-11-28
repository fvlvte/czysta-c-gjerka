#include "assetManager.h"
#include "render.h"
#include "image.h"
#include "system.h"

void assetManagerInit(assetManager* in)
{
	
}

void assetManagerLoadTexture(assetManager* in, uint64_t assetId, const char* path)
{
	image* i = loadBitmap(path);
	
	texture* t = knurmalloc(sizeof(*t));
	loadTexture(t, i);

	freeImage(i);

	in->table[assetId].id = assetId;
	in->table[assetId].mask = ASSET_LOADED_MASK;
	in->table[assetId].type = ASSET_TYPE_TEXTURE_2D;
	in->table[assetId].descriptor = t;
}


abstractAssetEntry* assetManagerFetch(assetManager* in, uint64_t assetId)
{
	return &in->table[assetId];
}