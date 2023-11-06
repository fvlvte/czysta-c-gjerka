#include "core.h"
#include "render.h"
#include "image.h"
#include "game.h"
#include "system.h"

#define PLAYER_MOMENTUM_MUL_MAX 2.137

struct _playerBase
{
	float speedPerSecond;
	float momentum;
};
typedef struct _playerBase playerBase;

void spawnPlayer(world* w, texture* tex)
{
	entityBase* e = &w->entityTable[w->entityCount++];
	playerBase* p = knurmalloc(sizeof(*p));

	e->classId = ENTITY_PLAYER;
	e->id = 2137;
	e->vX = 50.0f;
	e->vY = 50.0f;
	e->width = 100.0f;
	e->height = 100.0f;
	e->texture = tex;
	e->textureDimensions.h = 1.0f;
	e->textureDimensions.w = 1.0f;
	e->textureDimensions.x = 0.0f;
	e->textureDimensions.y = 0.0f;
	e->entityClass = p;

	p->speedPerSecond = 21.37f;
	p->momentum = 1.0;

	w->playerEntity = e;
}

void simulatePlayerMovement(world* w)
{
	uint64_t current = highResolutionTimestamp();

	uint64_t diff = current - w->previousUpdateTick;

	double updateDelta = ((double)diff) / 1000.0l;

	w->previousUpdateTick = current;

	entityBase* eb = w->playerEntity;
	playerBase* pb = (playerBase*)w->playerEntity->entityClass;

	if (w->movingVectorX != 0.0f || w->movingVectorY != 0.0f)
	{
		pb->momentum += 1.0f * updateDelta;
		if (pb->momentum > PLAYER_MOMENTUM_MUL_MAX)
		{
			pb->momentum = PLAYER_MOMENTUM_MUL_MAX;
		}
	}
	else
	{
		pb->momentum = 1.0f;
	}

	eb->vX += pb->speedPerSecond * updateDelta * w->movingVectorX * pb->momentum;
	eb->vY += pb->speedPerSecond * updateDelta * w->movingVectorY * pb->momentum;
}

void coreMain(void)
{
	core *c = knurmalloc(sizeof(*c));

	world *w = knurmalloc(sizeof(*w));

	w->previousUpdateTick = highResolutionTimestamp();

	w->entityCount = 0;
	
	c->targetWindow = createWindow("uwu kocham widzuw uwu", 500, 500);

	texture t;

	image* i = loadBitmap("uwu.bmp");
	int x;

	loadTexture(&t, i);

	spawnPlayer(w, &t);

	freeImage(i);

	while (isWindowOpen(c->targetWindow)) // Przerywamy tą pętle gdy okno zostanie zamknięte.
	{
		updateWindow(c->targetWindow); // Funkcja która przetwarza eventy które okno otrzymało od systemu operacyjnego i na nie reaguje.

		for (x = 0; x < c->targetWindow->inputStackSize; x++)
		{
			input* input = &c->targetWindow->inputStack[x];

			if (input->type == EVENT_KEYDOWN)
			{
				if (input->paramA == 'w' || input->paramA == 'W')
				{
					w->movingVectorY = -1.0f;
				}
				else if (input->paramA == 's' || input->paramA == 'S')
				{
					w->movingVectorY = 1.0f;
				}
				else if (input->paramA == 'a' || input->paramA == 'A')
				{
					w->movingVectorX = -1.0f;
				}
				else if (input->paramA == 'd' || input->paramA == 'D')
				{
					w->movingVectorX = 1.0f;
				}
			}
			if (input->type == EVENT_KEYUP)
			{
				if (input->paramA == 'w' || input->paramA == 'W' || input->paramA == 's' || input->paramA == 'S')
				{
					w->movingVectorY = 0.0f;
				}
				else if (input->paramA == 'a' || input->paramA == 'A' || input->paramA == 'd' || input->paramA == 'D')
				{
					w->movingVectorX = 0.0f;
				}
			}
		}

		beginFrame(c->targetWindow);

		simulatePlayerMovement(w);
		
		for (x = 0; x < w->entityCount; x++)
		{
			entityBase* e = &w->entityTable[x];

			rect computedDimensions = { e->vX, e->vY, e->width, e->height };
			drawRectangle(e->texture, &e->textureDimensions, &computedDimensions, NULL, 0);
		}

		endFrame(c->targetWindow);
	}
}