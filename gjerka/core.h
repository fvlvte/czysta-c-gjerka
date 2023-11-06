#pragma once

// Główny kontroler gry.

#include <stdint.h>

#include "window.h"

struct _core
{
	window*			targetWindow;
};
typedef struct _core core;

void coreMain(void);