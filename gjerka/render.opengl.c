#include <Windows.h>
#include <gl/GL.h>
#include "render.h"
#include "font.h"

void initRenderer(window* w)
{
	fontManagerInit();
}

void beginFrame(window* w)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0l, (double)w->width, (double)w->height, 0.0l, -1.0l, 1.0l);
	glViewport(0, 0, w->width, w->height);
}

void endFrame(window* w)
{
	glFlush();
	flushRender(w);
}

void drawUtf8text(window* wnd,
	font* f,
	float scale,
	color* c,
	const char* text,
	rect* rect,
	uint64_t shouldRender)
{
	glyph* prev_g = NULL;
	glyph* g = NULL;

	float x = rect->x;
	float y = rect->y;

	float curx = (float)x;
	float cury = (float)y;
	uint64_t len = knurstrlen(text);
	uint64_t i, z;
	kerning* k;

	if (shouldRender == 0)
	{
		rect->h = f->baseScale * scale;
		rect->w = 0.0f;
	}

	for (i = 0; i < len; i++)
	{

		if (text[i] == '\n')
		{
			curx = rect->x;
			y = y + f->baseScale * scale;
			cury = y;

			if (shouldRender == 0)
			{
				rect->h += f->baseScale * scale;
			}
			continue;
		}

		g = f->resolver(text[i]);

		curx += (float)g->xoff * scale;
		cury += (float)g->yoff * scale;

		if (prev_g != NULL && prev_g->kernings != NULL)
		{
			z = 0;
			k = &prev_g->kernings[z++];
			while (k->nextGlyph != 0)
			{
				if (k->nextGlyph == text[i])
				{
					curx += (float)k->adjustment * scale;
					break;
				}

				k = &prev_g->kernings[z++];
			}
		}

		if (shouldRender == 0)
		{
			cury = (float)y;
			curx += (float)g->xadv * scale;

			prev_g = g;

			continue;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, (GLuint)f->texArr[g->page].systemDescriptor);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glScalef(1.0f, -1.0f, 1.0f);

		glBegin(GL_QUADS);

		if (c != NULL)
		{
			glColor4f(c->r, c->g, c->b, c->a);
		}

		float basegX = g->x;

		glTexCoord2f((float)basegX / (float)f->texArr[g->page].pixelWidth, (float)g->y / (float)f->texArr[g->page].pixelHeight);
		glVertex3f(curx, cury, 0.0f);

		glTexCoord2f((float)basegX / (float)f->texArr[g->page].pixelWidth + (float)g->width / (float)f->texArr[g->page].pixelWidth, (float)g->y / (float)f->texArr[g->page].pixelHeight);
		glVertex3f((float)curx + scale * g->width, (float)cury, 0.0f);

		glTexCoord2f((float)basegX / (float)f->texArr[g->page].pixelWidth + (float)g->width / (float)f->texArr[g->page].pixelWidth , (float)g->y / (float)f->texArr[g->page].pixelHeight + (float)g->height / (float)f->texArr[g->page].pixelHeight);
		glVertex3f((float)curx + scale * g->width, (float)cury + scale * g->height, 0.0f);

		glTexCoord2f((float)basegX / (float)f->texArr[g->page].pixelWidth, (float)g->y / (float)f->texArr[g->page].pixelHeight + (float)g->height / (float)f->texArr[g->page].pixelHeight);
		glVertex3f(curx, (float)cury + scale * g->height, 0.0f);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		cury = (float)y;
		curx += (float)g->xadv * scale;

		prev_g = g;
	}

	if (shouldRender == 0)
	{
		rect->w = curx;
		rect->h = cury + f->baseScale * scale;
	}	
}

// Funckja ładująca nasz obiekt image do vRAM-u.
// Po załadowaniu nasz obiekt image można zwolnić.
void loadTexture(
	texture* out,			// Uchwyt typu out, stkurtura którą wypełnimy - pasywna inicjalizacja.
	image* i				// Uchwyt do naszego obiektu image.
)
{
	GLint tex; // ID tekstury.

	glGenTextures(1, &tex); // Generujemy ID tekstury.

	glBindTexture(GL_TEXTURE_2D, tex); // Mówimy OpenGL-owi z której tekstury chcemy korzystać.

	// Ustawiamy filtr skalowania w górę.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Ustawiamy filtr skalowania w dół.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Kopiujemy z RAM-u do vRAMU surowe piskele obrazu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)i->width, (GLsizei)i->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, i->data);

	// Ustawiamy pomocnicze informacje w naszej strukturze texture.
	out->pixelHeight = i->height;
	out->pixelWidth = i->width;
	out->systemDescriptor = (void*)tex; // Uchwyt systemowy do tekstury.
	out->flags = 0;

	// Gucci mamy teksture zuploadowaną do vRAM i gotową do użycia.
}

// Funckja zwalniająca teksturę z vRAM-u.
void freeTexture(
	texture* in					// Uchwyt do tekstury do zwolnienia.
)
{
	glDeleteTextures(1, &in->systemDescriptor); // Wywalamy z vRAM i zwalniamy deskryptor.
}

// Funckja do narysowania prostokąta (opcjonalnie oteksturowanego/z nałożonym kolorem/lub z teksturą i filtrem koloru),
void drawRectangle(
		texture* tex,			// Uchwyt do tekstury którą chcemy nałożyć (opcjonalna) - NULL jeśli nie chcemy tekstury.
		rect* textureCoords,	// Uchwyt do struktury rect (prostokąta) tekstury do nałożenia na naszym prostokącie (opcjonalny) - NULL jeśli nie używamy tekstury.
		rect* coords,			// Uchwyt do struktury rect (prostokąta) względem naszego okna (w pikselach).
		color* c,				// Uchwyt do struktury color z maską RGBA koloru (opcjonalny) - NULL jeśli nie chcemu używać filtra koloru.
		uint64_t flags			// Flagi połączone operatorem | (możliwe opcje: ) lub 0 jeśli nie chcemy używać żadnych flag.
)
{
	// Nakładamy maskę koloru lub resetujemy ją do braku maski - 1,1,1,1.
	if (c != NULL)
	{
		glColor4f(c->r, c->g, c->b, c->a);
	}
	else
	{
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	// Jeśli podaliśmy teksturę przygotujmy OpenGL do wyrenderowania jej na prostokącie.
	if (tex != NULL)
	{
		// Ustawmy o którą teksturę nam chodzi.
		glBindTexture(GL_TEXTURE_2D, (GLuint)tex->systemDescriptor);

		// Włączmy alpha blending (przeźroczystość) jeśli tekstura ją obsługuje.
		if (tex->flags & TEXTURE_FLAG_ALPHA_BLEND)
		{
			// Włączamy funckje blendowania.
			glEnable(GL_BLEND);
			// Funckja wyliczania przeźroczystości 1 - kanał alfa tekstury.
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		// Odbijmy teksturę po osi Y bo specyfikacja OpenGL ma odwróconą oś Y w stosunku do bitmapy naszej tekstury.
		glMatrixMode(GL_TEXTURE); // Ustawiamy macierz na którym będziemy operować (GL_TEXTURE).
		glLoadIdentity(); // Wczytujemy macierz {1, 0, 0, 0} {0, 1, 0, 0} {0, 0, 1, 0} {0, 0, 0, 1} (identity).
		glScalef(1.0f, -1.0f, 1.0); // Mnożymy identity matrix przez 1 -1 1 czyli odbijamy oś Y.

		// Uruchamiamy moduł teksturowania.
		glEnable(GL_TEXTURE_2D);
	}

	// Rozpoczynamy rysowanie prostokąta.
	glBegin(GL_QUADS);

	// Nakładamy wierzchołek tekstury jeśli istnieje tX, tY.
	if (tex != NULL)
	{
		glTexCoord2f(textureCoords->x, textureCoords->y);
	}
	// Tworzymy pierwszy wierzchołek ptostokąta X, Y.
	glVertex3f(coords->x, coords->y, 0.0f);

	// Analogicznie tekstura na tX + tW, tY.
	if (tex != NULL)
	{
		glTexCoord2f(textureCoords->x + textureCoords->w, textureCoords->y);
	}
	// Wierzchołek X + W, Y.
	glVertex3f(coords->x + coords->w, coords->y, 0.0f);

	// Tekstura na tX + tW, tY + tW.
	if (tex != NULL)
	{
		glTexCoord2f(textureCoords->x + textureCoords->w, textureCoords->y + textureCoords->h);
	}
	// Wierzchołek X + W, Y + H.
	glVertex3f(coords->x + coords->w, coords->y + coords->h, 0.0f);

	// Tekstura na tX, tY + tH.
	if(tex != NULL)
	{
		glTexCoord2f(textureCoords->x, textureCoords->y + textureCoords->h);
	}
	glVertex3f(coords->x, coords->y + coords->h, 0.0f);

	// Kończymy renderowanie prostokąta.
	glEnd();

	// Pora na clean up.
	// Jeśli używaliśmy tekstur / blendowania (przeźroczystości) musimy je wyłączyć.
	if (tex != NULL)
	{
		if (tex->flags & TEXTURE_FLAG_ALPHA_BLEND)
		{
			// Wyłączamy moduł blendowania.
			glDisable(GL_BLEND);
		}

		// Wyłączamy moduł teksturowania.
		glDisable(GL_TEXTURE_2D);
		// Resetujemy używaną teksture (ustawiamy na 0 = brak tekstury).
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
