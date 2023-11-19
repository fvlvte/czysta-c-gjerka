#include "image.h"
#include "system.h"

image* loadBitmap(const char* path)
{
	file f = openFile(path);
	int64_t szIn, szIn2;

	if (f == NULL)
		return NULL;

	image* i = knurmalloc(sizeof(*i));

	struct _header h;
	struct _info _i;

	szIn = readFile(f, &h, sizeof(h));
	szIn2 = readFile(f, &_i, sizeof(_i));

	if (
		_i.planeCount != 1 || 
		_i.compression != 3 || // Color encoding.
		_i.bitCount != 32 || 
		_i.width != _i.height || 
		h.magic1 != 'B' || 
		h.magic2 != 'M' ||
		szIn != sizeof(h) ||
		szIn2 != sizeof(_i)
	)
	{
		knurfree(i);
		closeFile(f);

		return NULL;
	}

	uint32_t sz = _i.width * _i.height * sizeof(uint32_t);

	i->data = knurmalloc(sz);
	i->format = IMAGE_FORMAT_RGBA8;
	i->height = _i.height;
	i->width = _i.width;

	seekFile(f, h.offBits);

	szIn = readFile(f, i->data, sz);

	if (szIn != sz)
	{
		knurfree(i->data);
		knurfree(i);
		closeFile(f);

		return NULL;
	}

	closeFile(f);

	return i;
}

void freeImage(image* img)
{
	knurfree(img->data);
	knurfree(img);
}