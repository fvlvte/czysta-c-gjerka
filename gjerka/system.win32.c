#include "system.h"
#include <Windows.h>

void endProcess(int exitCode)
{
	// Zakańcza proces natychmiastowo (WinAPI).
	ExitProcess(exitCode);
}

void* knurmalloc(unsigned int size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void knurfree(void* mem)
{
	HeapFree(GetProcessHeap(), 0, mem);
}