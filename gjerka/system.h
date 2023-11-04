#pragma once

// Kończy natychmiastowo proces z podanym exitCode.
void endProcess(int exitCode);

void* knurmalloc(unsigned int size);
void knurfree(void* mem);