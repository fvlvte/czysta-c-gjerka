#include "core.h"
#include "render.h"
#include "image.h"
#include "game.h"
#include "system.h"
#include "font.h"
#include "world.h"
#include "ext.h"
#include "assetManager.h"
#include "view.h"
#include "view_mainMenu.h"

static editor_state EDITOR_STATE;

static assetManager ASSET_MANAGER;

void spawnPlayer(game* w, texture* tex)
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

void simulatePlayerMovement(game* w)
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

// Zwraca poprzedni stan.
uint64_t switchGameState(game* g, wworld* w, uint64_t state)
{
	uint64_t previousState = g->state;

	// Jak by to reactoweic ujoł - componentWillUnmount
	switch (previousState)
	{
	default:
		break;
	}


	// Hehe react componentWillMount xD
	switch (state)
	{
	case STATE_EDITOR:
		EDITOR_STATE.selectedX = 0;
		EDITOR_STATE.selectedY = 0;
		break;
	default:
		break;
	}
}

void handleGameState(core* c, game* g, wworld* w)
{
	font* ff = getDefaultFont();
	uint64_t x;

	for (x = 0; x < c->targetWindow->inputStackSize; x++)
	{
		input* input = &c->targetWindow->inputStack[x];

		if (input->type == EVENT_KEYDOWN)
		{
			if (input->paramA == 'W')
			{
				g->movingVectorY = -1.0f;
			}
			else if (input->paramA == 'S')
			{
				g->movingVectorY = 1.0f;
			}
			else if (input->paramA == 'A')
			{
				g->movingVectorX = -1.0f;
			}
			else if (input->paramA == 'D')
			{
				g->movingVectorX = 1.0f;
			}
		}
		if (input->type == EVENT_KEYUP)
		{
			if (input->paramA == 'W' || input->paramA == 'S')
			{
				g->movingVectorY = 0.0f;
			}
			else if (input->paramA == 'A' || input->paramA == 'D')
			{
				g->movingVectorX = 0.0f;
			}
		}
	}

	beginFrame(c->targetWindow);

	renderWorld(w, 0.0, 0.0, (float)c->targetWindow->width, (float)c->targetWindow->height);

	for (x = 0; x < g->entityCount; x++)
	{
		entityBase* e = &g->entityTable[x];

		rect computedDimensions = { e->vX, e->vY, e->width, e->height };
		drawRectangle(e->texture, &e->textureDimensions, &computedDimensions, NULL, 0);
	}

	uint64_t current = highResolutionTimestamp();

	uint64_t diff = current - g->previousUpdateTick;

	rect computed;
	drawUtf8text(c->targetWindow, ff, 2.0f, NULL, "WOJTYLA SURVIWORS", &computed, 0);

	rect textRect = { c->targetWindow->width / 2 - computed.w / 2, c->targetWindow->height / 2 - computed.h / 2, 0, 0 };
	drawUtf8text(c->targetWindow, ff, 2.0f, NULL, "WOJTYLA SURVIWORS", &textRect, 1);

	simulatePlayerMovement(g);

	endFrame(c->targetWindow);
}

void handleGameEditor(core* c, game* g, wworld* w)
{
	uint64_t x;

	for (x = 0; x < c->targetWindow->inputStackSize; x++)
	{
		input* input = &c->targetWindow->inputStack[x];
		
		if (input->type == EVENT_MOUSE_LB)
		{
			uint64_t mX = input->paramA / 32;
			uint64_t mY = input->paramB / 32;

			EDITOR_STATE.selectedX = mX;
			EDITOR_STATE.selectedY = mY;
		}
	}

	beginFrame(c->targetWindow);

	renderWorldEditor(w, &EDITOR_STATE);

	rect textTarget = { 0.0f, c->targetWindow->height - 40, 0.0f, 0.0f };
	color cc = { 1.0f, 1.0f, 1.0f, 1.0f };

	char buffer[2137];
	sprintf_t sprintf = getExtSprintf();
	sprintf(buffer, "Selected X: %d Y: %d", EDITOR_STATE.selectedX, EDITOR_STATE.selectedY);
	
	drawUtf8text(c->targetWindow, getDefaultFont(), 1.0f, &cc, buffer, &textTarget, 1);


	rect textureRect = { 0.0f, 0.0f, 290.0f / 1024.0f, 64.0f / 1024.0f };
	rect screenRect = { (float)c->targetWindow->width - 150.0f, (float)c->targetWindow->height - 40.0f, 290.0f / 2, 64.0f / 2 };
	abstractAssetEntry* ase = assetManagerFetch(&ASSET_MANAGER, ASSET_UI_COMMON);
	drawRectangle(ase->descriptor, &textureRect, &screenRect, &cc, 0);

	endFrame(c->targetWindow);
}

void coreMain(void)
{
	core *c = knurmalloc(sizeof(*c));
	
	c->targetWindow = createWindow("SUPER AUTO KNURS", 800, 600);

	view_manager* mgr = view_manager_init();

	
	view_mainMenu* mainMenu = view_mainMenu_alloc();

	view_manager_setView(mgr, mainMenu);

	view_manager_bindWindow(mgr, c->targetWindow);

	while (isWindowOpen(c->targetWindow)) // Przerywamy tą pętle gdy okno zostanie zamknięte.
	{
		updateWindow(c->targetWindow); // Funkcja która przetwarza eventy które okno otrzymało od systemu operacyjnego i na nie reaguje.
		view_manager_onUpdate(mgr);
	}
}