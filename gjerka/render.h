#pragma once

#include "window.h"
#include "image.h"
#include <stdint.h>

#define TEXTURE_FLAG_ALPHA_BLEND 0b1

#define RENDER_TEX_OFFSET_PX 0b1

#define ENTITY_PLAYER 1

struct _abstractTexture
{
	uint64_t	pixelWidth;
	uint64_t	pixelHeight;
	uint64_t	flags;
	void*		systemDescriptor;

};
typedef struct _abstractTexture texture;

struct _color
{
	float r;
	float g;
	float b;
	float a;
};
typedef struct _color color;

struct _rect
{
	float x;
	float y;
	float w;
	float h;
};
typedef struct _rect rect;

void initRenderer(window* w);
void beginFrame(window* w);
void endFrame(window* w);

void drawUtf8text(window* wnd,
	void* f,
	float scale,
	color* c,
	const char* text,
	rect* rect,
	uint64_t shouldRender);

#define TEXTURE_PIXEL_FORMAT_RGBA 0b1

// Funckja ładująca nasz obiekt image do vRAM-u.
// Po załadowaniu nasz obiekt image można zwolnić.
void loadTexture(
	texture* out,			// Uchwyt typu out, stkurtura którą wypełnimy - pasywna inicjalizacja.
	image* i				// Uchwyt do naszego obiektu image.
);

// Funckja zwalniająca teksturę z vRAM-u.
void freeTexture(
	texture* in					// Uchwyt do tekstury do zwolnienia.
);

// Funckja do narysowania prostokąta (opcjonalnie oteksturowanego/z nałożonym kolorem/lub z teksturą i filtrem koloru),
void drawRectangle(
	texture* tex,			// tekstura którą chcemy nałożyć (opcjonalna) - NULL jeśli nie chcemy tekstury
	rect* textureCoords,	// wskaźnik do struktury rect (prostokąta) tekstury do nałożenia na naszym prostokącie (opcjonalny) - NULL jeśli nie używamy tekstury
	rect* coords,			// wskaźnik do struktury rect (prostokąta) względem naszego okna (w pikselach)
	color* c,				// wskaźnik do struktury color z maską RGBA koloru (opcjonalny) - NULL jeśli nie chcemu używać filtra koloru
	uint64_t flags			// flagi połączone operatorem | (możliwe opcje: ) lub 0 jeśli nie chcemy używać żadnych flag
);