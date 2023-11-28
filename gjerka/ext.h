#pragma once

typedef int (*sprintf_t)(
	char* buffer,
	const char* format,
	...
	);

void initExtLib();
sprintf_t getExtSprintf(void);