#include "world.h"
#include "system.h"
#include "image.h"

wworld* createWorld(uint64_t tileResolution, uint64_t width, uint64_t height, uint64_t layers)
{
	wworld* w = knurmalloc(sizeof(*w));

	image* img = loadBitmap("asset/aha.bmp");
	texture* tx = knurmalloc(sizeof(*tx));

	loadTexture(tx, img);
	freeImage(img);


	w->layerCount = layers;
	w->tileHeight = height;
	w->tileResolution = tileResolution;
	w->tileWidth = width;
	w->layers = knurmalloc(sizeof(void*) * layers);

	uint64_t i;
	uint64_t x, y;

	rect firstTile = { 32.0f / 512.0f, 32.0f / 512.0f, 32.0f / 512.0f, 32.0f / 512.0f };

	for (i = 0; i < layers; i++)
	{
		w->layers[i] = knurmalloc(sizeof(tile) * width * height);

		for (x = 0; x < width; x++)
		{
			for (y = 0; y < height; y++)
			{
				tile* t = &w->layers[i][x * w->tileWidth + y];
				t->tileset = tx;
				knurcopy(&t->tilesetOffset, &firstTile, sizeof(rect));		
				t->flags = 0;
			}
		}
	}

	return w;
}

void renderWorld(wworld* world, float cameraX, float cameraY, float cameraWidth, float cameraHeight)
{
	uint64_t nearestXTile = (uint64_t)cameraX / world->tileResolution;
	uint64_t nearestXTileOffset = (uint64_t)cameraX % world->tileResolution; // minus

	uint64_t nearestYTile = (uint64_t)cameraY / world->tileResolution;
	uint64_t nearestYTileOffset = (uint64_t)cameraY % world->tileResolution; // minus

	uint64_t tilesToRenderX = (uint64_t)cameraWidth / world->tileResolution;
	uint64_t tilesToRenderY = (uint64_t)cameraHeight / world->tileResolution;

	if ((uint64_t)cameraWidth % world->tileResolution != 0)
		tilesToRenderX++;
	if ((uint64_t)cameraHeight % world->tileResolution != 0)
		tilesToRenderY++;

	uint64_t x, y;
	for (x = 0; x < tilesToRenderX; x++)
	{
		for (y = 0; y < tilesToRenderY; y++)
		{
			uint64_t tileOffset = (nearestXTile + x) * world->tileWidth + nearestYTile + y;

			rect target = { (float)x * (float)world->tileResolution - (float)nearestXTileOffset,
				(float)y * (float)world->tileResolution - (float)nearestYTileOffset, (float)world->tileResolution, (float)world->tileResolution };
			tile* t = &world->layers[0][tileOffset];
			drawRectangle(t->tileset, &t->tilesetOffset, &target, NULL, 0);
		}
	}
}

void renderWorldEditor(wworld* world, editor_state* state)
{
	texture* t = world->layers[0]->tileset;
	rect texC = { 0.0f, 0.0f, 1.0f, 1.0f };
	rect screenC = { 0.0f, 0.0f, (float)t->pixelWidth, (float)t->pixelHeight };
	drawRectangle(t, &texC, &screenC, NULL, 0);

	rect selectionRect = { (float)state->selectedX * 32.0f, (float)state->selectedY * 32.0f, 32.0f, 32.0f};
	color selectionOverlay = { 1.0f, 0.0f, 0.0f, 0.2f };
	drawRectangle(NULL, NULL, &selectionRect, &selectionOverlay, 0);
}