#pragma once

#include <stdint.h>

// Kończy natychmiastowo proces z podanym exitCode.
void endProcess(int exitCode);

void* knurmalloc(uint64_t size);
void knurfree(void* mem);

typedef void* file;

file openFile(const char* path);
void closeFile(file file);

int64_t readFile(file f, void* out, uint64_t size);
int64_t writeFile(file f, void* in, uint64_t size);
uint64_t fileSize(file f);

unsigned long long highResolutionTimestamp();