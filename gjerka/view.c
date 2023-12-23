#include "view.h"
#include "system.h"

typedef void (*view_onUnmount)(view_manager* manager, view* view);
typedef void (*view_onMount)(view_manager* manager, view* view);
typedef void (*view_onDraw)(view_manager* manager, view* view);

void view_initBase(view* v)
{

}

view_manager* view_manager_init(void)
{
	view_manager* mgr;

	mgr = knurmalloc(sizeof(*mgr));

	mgr->currentView = NULL;

	return mgr;
}

void view_manager_setView(view_manager* mgr, view* view)
{
	if (mgr->currentView != NULL)
	{
		if (mgr->currentView->onUnmount != NULL)
		{
			((view_onUnmount)mgr->currentView->onUnmount)(mgr, view);
		}
	}
	mgr->currentView = view;
	if (mgr->currentView->onMount != NULL)
	{
		((view_onMount)view->onMount)(mgr, view);
	}
}

void view_manager_bindWindow(view_manager* mgr, window* wnd)
{
	mgr->currentWindow = wnd;
}

void view_manager_onUpdate(view_manager* mgr)
{
	if (mgr->currentWindow != NULL)
	{
		beginFrame(mgr->currentWindow);

		if (mgr->currentView != NULL && mgr->currentView->onDraw != NULL)
		{
			((view_onDraw)mgr->currentView->onDraw)(mgr, mgr->currentView);
		}

		endFrame(mgr->currentWindow);
	}
}

void view_manager_shut(view_manager* mgr)
{
	((view_onUnmount)mgr->currentView->onUnmount)(mgr, mgr->currentView);
	knurfree(mgr);
}