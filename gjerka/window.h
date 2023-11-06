#pragma once // Strażnik - pilnuje żeby dwa razy nie zaimportować tego samego pliku okok.

#include <stdint.h>

#define MAX_QUEUE_SIZE 256

#define EVENT_KEYDOWN	1
#define EVENT_KEYUP		2

struct _input
{
	uint64_t	timestamp;
	uint64_t	type;
	uint64_t	paramA;
	uint64_t	paramB;
	void*		paramPtr;
};
typedef struct _input input;

// Ta struktura to nasze okno okok.
struct window
{
	void* system_impl; // Uchwyt systemow - inny dla windowsa / linuxa / maca / etc ...
	void* system_param;
	unsigned long long lastFrame;
	float animationState;
	float animModifier;
	float width;
	float height;

	input			inputStack[MAX_QUEUE_SIZE];
	uint64_t		inputStackSize;
};
typedef struct window window;

// Tworzy okno i zwraca do niego uchwyt.
window* createWindow(const char* title, unsigned int width, unsigned int height);

// Zamyka okno i zwalnia pamięć.
void closeWindow(window* instance);

// 1 jeśli okno jest otwarte, 0 zamknięte
int isWindowOpen(window* instance);

void renderFrame(window* instance);

void flushRender(window* instance);

// Przetwarza eventy z okna.
void updateWindow(window* instance);