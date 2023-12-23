#pragma once

#include <stdint.h>
#include "render.h"

struct _component
{
	uint64_t			id;
	void*				eventHandler;
	rect				screenPosition;
	struct _component** children;

};
typedef struct _component component;