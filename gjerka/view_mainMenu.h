#pragma once

#include "view.h"

struct view_mainMenu
{
	view viewBase;
};
typedef struct view_mainMenu view_mainMenu;

view_mainMenu* view_mainMenu_alloc(void);

void view_mainMenu_free(view_mainMenu* view);