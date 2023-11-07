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

struct _world
{
	rect cameraPosition;

	entityBase* playerEntity;

	float movingVectorX;
	float movingVectorY;

	uint64_t previousUpdateTick;

	uint64_t entityCount;
	entityBase entityTable[ENTITY_MAX];
};
typedef struct _world world;

#define PLAYER_MOMENTUM_MUL_MAX 2.137

struct _playerBase
{
	float speedPerSecond;
	float momentum;
};
typedef struct _playerBase playerBase;