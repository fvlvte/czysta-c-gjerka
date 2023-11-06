#include <Windows.h>
#include <gl/GL.h>
#include "render.h"

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
