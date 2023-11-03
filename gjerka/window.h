#pragma once // Strażnik - pilnuje żeby dwa razy nie zaimportować tego samego pliku okok.

// Ta struktura to nasze okno okok.
struct window
{
	void* system_impl; // Uchwyt systemow - inny dla windowsa / linuxa / maca / etc ...
};
typedef struct window window;

// Tworzy okno i zwraca do niego uchwyt.
window* createWindow(const char* title, unsigned int width, unsigned int height);

// Zamyka okno i zwalnia pamięć.
void closeWindow(window* instance);

// 1 jeśli okno jest otwarte, 0 zamknięte
int isWindowOpen(window* instance);

// Przetwarza eventy z okna.
void updateWindow(window* instance);