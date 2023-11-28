#include "ext.h"
#include <Windows.h>

static sprintf_t SPRINTF_IMPL = NULL;

void initExtLib()
{
	HMODULE msvcrt = LoadLibraryA("msvcrt.dll");
	FARPROC sprintf = GetProcAddress(msvcrt, "sprintf");

	SPRINTF_IMPL = (sprintf_t)sprintf;
}

sprintf_t getExtSprintf(void)
{
	return SPRINTF_IMPL;
}