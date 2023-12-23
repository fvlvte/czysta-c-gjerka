#pragma once
#include <stdint.h>

struct _text_component
{
	char*		textPtr;
	uint64_t	length;
};
typedef struct _text_component text_component;