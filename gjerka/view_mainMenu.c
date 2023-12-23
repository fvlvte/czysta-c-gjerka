#include "view_mainMenu.h"
#include "system.h"
#include "render.h"
#include "font.h"

void view_mainMenu_onDraw(view_manager* mgr, view_mainMenu* self)
{

	color clearMask = { 1.0, 1.0, 1.0, 1.0 };
	rect onScreenTarget = { 0.0, 0.0, 0.0, 0.0 };
	drawUtf8text(mgr->currentWindow, getDefaultFont(), 1.0f, &clearMask, "SUPER AUTO KNURS", &onScreenTarget, 1);
}

view_mainMenu* view_mainMenu_alloc(void)
{
	view_mainMenu* v;

	v = knurmalloc(sizeof(*v));

	knurZeroMemory(v, sizeof(*v));

	view_initBase(&v->viewBase);

	v->viewBase.onDraw = &view_mainMenu_onDraw;

	return v;
}



void view_mainMenu_free(view_mainMenu* view)
{
	knurfree(view);
}