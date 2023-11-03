#include "system.h"
#include <Windows.h>

void endProcess(int exitCode)
{
	// Zakańcza proces natychmiastowo (WinAPI).
	ExitProcess(exitCode);
}