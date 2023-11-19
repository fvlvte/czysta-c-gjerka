#pragma once
#include "render.h"

#define ENTITY_MAX 1024

struct _entityBase
{
	uint64_t id;
	uint64_t classId;

	float vX;
	float vY;
	float width;
	float height;

	rect textureDimensions;

	texture* texture;

	void* entityClass;
};
typedef struct _entityBase entityBase;

#define STATE_GAME		0x00
#define STATE_EDITOR	0x01

struct _game
{
	uint64_t state;

	rect cameraPosition;

	entityBase* playerEntity;

	float movingVectorX;
	float movingVectorY;

	uint64_t previousUpdateTick;

	uint64_t entityCount;
	entityBase entityTable[ENTITY_MAX];
};
typedef struct _game game;

#define PLAYER_MOMENTUM_MUL_MAX 2.137

struct _playerBase
{
	float speedPerSecond;
	float momentum;
};
typedef struct _playerBase playerBase;