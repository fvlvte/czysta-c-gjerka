#include "system.h"
#include <Windows.h>

static HANDLE HEAP_HANDLE_CACHE = INVALID_HANDLE_VALUE;

void endProcess(int exitCode)
{
	// Zakańcza proces natychmiastowo (WinAPI).
	ExitProcess(exitCode);
}

inline void _heapHandleCache()
{
	if (HEAP_HANDLE_CACHE == INVALID_HANDLE_VALUE)
	{
		HEAP_HANDLE_CACHE = GetProcessHeap();
	}
}

void* knurmalloc(uint64_t size)
{
	_heapHandleCache();
	return HeapAlloc(HEAP_HANDLE_CACHE, 0, size);
}

void knurfree(void* mem)
{
	_heapHandleCache();
	HeapFree(HEAP_HANDLE_CACHE, 0, mem);
}

file openFile(const char* path)
{
	HANDLE h = (file)CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return h == INVALID_HANDLE_VALUE ? NULL : h;
}

void closeFile(file f)
{
	CloseHandle((HANDLE)f);
}

int64_t readFile(file f, void* out, uint64_t size)
{
	DWORD os;
	if (ReadFile((HANDLE)f, out, (DWORD)size, &os, NULL) == TRUE)
	{
		return (int64_t)os;
	}
	return -1;
}

int64_t writeFile(file f, void* in, uint64_t size)
{
	DWORD is;
	if (WriteFile((HANDLE)f, in, (DWORD)size, &is, NULL) == TRUE)
	{
		return (int64_t)is;
	}

	return -1;
}

uint64_t fileSize(file f)
{
	LARGE_INTEGER li;
	GetFileSizeEx((HANDLE)f, &li);

	return (uint64_t)li.QuadPart;
}

unsigned long long highResolutionTimestamp() // ms timestamp
{
	LARGE_INTEGER li;
	LARGE_INTEGER fq;

	QueryPerformanceFrequency(&fq);
	QueryPerformanceCounter(&li);

	li.QuadPart *= 1000;
	li.QuadPart /= fq.QuadPart;

	return li.QuadPart;
}