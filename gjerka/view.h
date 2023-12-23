#pragma once

#include "render.h"
#include "component.h"
#include "window.h"

#include <stdint.h>

struct _view
{
	void* onDraw;
	void* onUpdate;
	
	void* onInput;

	void* onMount;
	void* onUnmount;

	void* instance;

	component** compontents;

	uint64_t componentCount;
	uint64_t componentMax;
};
typedef struct _view view;

struct _view_manager
{
	view* currentView;
	window* currentWindow;
};
typedef struct _view_manager view_manager;

void view_initBase(view* v);

view_manager* view_manager_init(void);

void view_manager_setView(view_manager* mgr, view* view);

void view_manager_onUpdate(view_manager* mgr);
void view_manager_bindWindow(view_manager* mgr, window* wnd);

void view_manager_shut(view_manager* mgr);