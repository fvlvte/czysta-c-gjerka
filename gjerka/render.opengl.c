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

			rect->w += (float)g->xadv * scale;

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

}

void loadTexture(texture* out, image* i)
{
	GLint tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)i->width, (GLsizei)i->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, i->data);

	out->pixelHeight = i->height;
	out->pixelWidth = i->width;
	out->systemDescriptor = (void*)tex;
	out->flags = 0;
}

void freeTexture(texture* in)
{
	glDeleteTextures(1, &in->systemDescriptor);
}

void drawRectangle(texture* tex, rect* textureCoords, rect* coords, color* c, uint64_t flags)
{
	if (c != NULL)
	{
		glColor4f(c->r, c->g, c->b, c->a);
	}

	if (tex != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, (GLuint)tex->systemDescriptor);

		if (tex->flags & TEXTURE_FLAG_ALPHA_BLEND)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}


		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glScalef(1.0f, -1.0f, 1.0);

		glEnable(GL_TEXTURE_2D);
	}

	glBegin(GL_QUADS);

	if(tex != NULL)
		glTexCoord2f(textureCoords->x, textureCoords->y);
	glVertex3f(coords->x, coords->y, 0.0f);
	if(tex != NULL)
		glTexCoord2f(textureCoords->x + textureCoords->w, textureCoords->y);
	glVertex3f(coords->x + coords->w, coords->y, 0.0f);
	if(tex != NULL)
		glTexCoord2f(textureCoords->x + textureCoords->w, textureCoords->y + textureCoords->h);
	glVertex3f(coords->x + coords->w, coords->y + coords->h, 0.0f);
	if(tex != NULL)
		glTexCoord2f(textureCoords->x, textureCoords->y + textureCoords->h);
	glVertex3f(coords->x, coords->y + coords->h, 0.0f);

	glEnd();

	if (tex != NULL)
	{
		if (tex->flags & TEXTURE_FLAG_ALPHA_BLEND)
		{
			glDisable(GL_BLEND);
		}

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
