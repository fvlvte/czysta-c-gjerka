#include "window.h"
#include "system.h"
#include "image.h"

#include <Windows.h> // Bibloteka WinAPI - jedyna z której będziemy korzystać.

#include <gl/GL.h>

#define GL_BGRA                           0x80E1

int _fltused = 0;

GLint txx;

// ATOM to customowy typ microsoftu ale generalnie jest to zwykły uchwyt.
// Custom typy są po to żeby programista sie nie pomylił z typem uchwytu generalnie.
// Wywołujemy RegisterClassExA podając wskaźnik do naszej struktury klasy okna żeby poinformować Windows, że chcemy ją stworzyć.
// On zwróci nam do registeredClass właśnie uchwyt do naszej klasy który będzie przydatny w momencie chęci jej wykorzystania.
// Persistent zmienna statyczna zawierająca naszą klase okna lub null.
static ATOM CLASS_INSTANCE = (ATOM)NULL;
static LARGE_INTEGER fq;

unsigned long long _ts() // ms timestamp
{
	LARGE_INTEGER li;
	
	QueryPerformanceCounter(&li);

	li.QuadPart *= 1000;
	li.QuadPart /= fq.QuadPart;
	
	return li.QuadPart;
}


// Funckja typu boilerplate przyjmująca zdarzenia o oknie od systemu operacyjnego.
// Aktualnie mock up, będziemy rozwijać ją w przyszłości.
LRESULT eventHandler(
	HWND unnamedParam1,
	UINT unnamedParam2,
	WPARAM unnamedParam3,
	LPARAM unnamedParam4
)
{
	return DefWindowProcA(unnamedParam1, unnamedParam2, unnamedParam3, unnamedParam4);
}

// Funckja pomocnicza która sprawdza czy nasza klasa okna istnieje.
// Jeśli nie istnieje to tworzy nową.
static void _checkAndRegisterClassIfNotExists()
{
	if (CLASS_INSTANCE == (ATOM)NULL)
	{
		// Okno na Windowsie tworzymy w 2 etapach.
		// Na samym początku musimy stworzyć klasę okna którą w dużym uproszczeniu możemy przyrównać do CSS w HTML-u.
		// Aby stworzyć klasę okna musimy na początku zapełnić strukturę WNDCLASSEXA.
		// To można przyrównać do property w CSS-ie czyli musimy ustawić to co nas interesuje w naszej klasie okna.
		// Z tą różnicą, że musimy generalnie wypełnić wszystkie pola.
		// Dlaczego? Pamięć statyczna jest niezainicjalizowana co oznacza, że jeśli jej nie ustawimy ona może mieć w środku jakieś dziwne wartości.
		// Dlaczego x2? Ponieważ pamięć statyczna jest recyklowana, ona jest zwalniana w taki sposób, że jest dopuszczana do ponownego użycia ale nie jest nigdy zerowana.
		// Więc niezainicjalizowana zmienna (czyli bez przypisanej wartości na sztywno) może mieć jakąś wartość poprzedniej zmiennej.
		// DLATEGO PAMIĘTAJMY O INICJALIZACJI ZMIENNYCH
		// Są od tej reguły pewne wyjątki ale warto sobie to wkuć do głowy.
		// Tworzymy statyczną strukture danych WNDCLASSEX która jest niezainicjalizowana (nie wypełniona). 
		WNDCLASSEXA windowClass;  // Tutaj jej dokumentacja https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa

		// Rozmiar naszej struktury - zawsze sizeof(STRUKTURA) jeśli jest to pole wymagane - to informacja ważna dla systemu operacyjnego.
		windowClass.cbSize = sizeof(WNDCLASSEXA);
		// Style okna które będzie dziedziczyło tą klase 
		windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; // Dokumentacja dozwolonych opcji https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
		// Funckja która ma odbierać zdarzenia od okna - czyli nasza funkcja wyżej.
		windowClass.lpfnWndProc = &eventHandler;
		// Ekstra przestrzeń na dane w uchwycie okna - nie potrzebujemy ich więc 0.
		windowClass.cbClsExtra = 0;
		// Ekstra przestrzeń na dane w uchwycie okna - nie potrzebujemy ich więc 0.
		windowClass.cbWndExtra = 0;
		// Uchwyt do naszej aplikacji - to było mocniej tłumaczone na streamie - zobacz voda jak cie interesuje ale generalnie mało istotne.
		windowClass.hInstance =
			GetModuleHandleA(NULL); // Dokumentacja GetModuleHandleA https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea
		// Ikonka naszego okna, NULL oznacza defaultową ikonkę.
		windowClass.hIcon = NULL;
		// Kursor jaki ma być w naszym oknie - IDC_ARROW to zwykła strzałka/
		windowClass.hCursor =
			LoadCursorA(NULL, IDC_ARROW); // Dokumentacja LoadCursorA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursora
		// Tło naszego okna - jakieś tam defaultowe nieistotne.
		windowClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
		// Nazwa menu - nieistotna jest to bardzo legacy feature okienek więc NULL bo nas to nie interesuje.
		windowClass.lpszMenuName = NULL;
		// Nazwa naszej klasy - dowlona - jest dla nas.
		windowClass.lpszClassName = "uwusnaGiereczkaOkienkoKlasa2137";
		// Ikonka okna w wersji small - też nie interesuje nas więc NULL da defaultową ikonkę.
		windowClass.hIconSm = NULL;

		CLASS_INSTANCE =
			RegisterClassExA(&windowClass); // Dokumentacja RegisterClassExA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
	}
}

window* createWindow(const char* title, unsigned int width, unsigned int height)
{
	QueryPerformanceFrequency(&fq);
	_checkAndRegisterClassIfNotExists(); // Zobacz co robi wyżej.

	// Allokujemy obiekt dla naszego okna.
	// Dlaczego? Bo żeby to było cross platformowe to musimy je owrapować w naszą strukture window.
	window* instance = knurmalloc(sizeof(*instance));

	// Funckja CreateWindowExA utworzy nasze okno i zwróci nam do niego uchwyt.
	// Co to są te uchwyty? To jest po prostu zmienna która jest wskaźnikiem do naszego okna.
	// Ona generalnie mówi o jakie okno chodzi systemowi operacyjnemu jeśli używamy funkcji manipulujących oknem co będzie widać niżej.
	HWND wnd = CreateWindowExA( // Dokumentacja CreateWindowExA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		0, // Flagi stylów rozszerzonych ale mamy w dupie CSSy.
		(LPCSTR)CLASS_INSTANCE, // Uchwyt albo nazwa naszej klasy okna - ja wybrałem uchwyt - ale castuje go do typu jaki ta funkcja chce żeby kompilator nie płakał.
		title, // Nazwa naszego okienka.
		WS_TILEDWINDOW, // Styl okna - WS_TILEDWINDOW to takie najbardziej generyczne okno windowsowe.
		500, // Kordynat X gdzie ma pojawić się na ekranie.
		500, // Kordynat Y gdzie ma pojawić się na ekranie.
		width, // Szerokość w px.
		height, // Wysokość w px.
		NULL, // Okno rodzic - nie mamy więc NULL.
		NULL, // Legacy menu - nie używamy więc NULL.
		NULL, // Nieistotny parametr więc NULL.
		NULL // Nieistotny parametr więc NULL.
	);

	// No i już prawie sukces boży.
	// Tylko nasze okno które się utworzyło jest niewidoczne.
	// Bo domyślnie na windowsie tak jest i luj.
	// Więc musimy wykorzystać naszego nowopowstałego uchwytu żeby powiedzieć windowsowi że czas pokazać światu nasze piękne okno.
	// Po to jest funkcja ShowWindow.
	ShowWindow( // Dokumetnacja ShowWindow https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
		wnd, // uchwyt do okna o które nam chodzi - bo generalnie możemy z naszego procesu pokazać okna nawet z innego procesu więc te uchwyty są ważne
		SW_SHOW // SW_SHOW to flaga która mówi, że windows ma pokazać to okno, analogicznie SW_HIDE ukryje to okno
	);

	HDC context = GetDC(wnd);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), // Size of this structure,
		1, // Structure version
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
		32, // Color bits
		8, // R
		0, // R-S
		8, // G
		0, // G-S
		8, // B
		0, // B-S
		8, // A
		0, // A-S
		32, // Accum
		8, 8, 8, 8, // Depth
		24,
		8, // Stencil
		0, // AUX
		PFD_MAIN_PLANE,
		0, // Visible mask
		0 // Damage mask
	};

	int result = ChoosePixelFormat(context, &pfd);

	BOOL ret = SetPixelFormat(context, result, &pfd);

	HGLRC glInstance = wglCreateContext(context);

	ret = wglMakeCurrent(context, glInstance);

	instance->system_impl = (void*)wnd;
	instance->lastFrame = _ts();
	instance->animationState = 0.0;
	instance->animModifier = 1.0;

	RECT    rcCli;
	GetClientRect(WindowFromDC(context), &rcCli);
	instance->width = (float)rcCli.right;
	instance->height = (float)rcCli.bottom;

	GLint tex[1];

	image* i = loadBitmap("uwu.bmp");


	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i->width, i->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, i->data);

	glDisable(GL_TEXTURE_2D);
	txx = tex[0];

	return instance;
}

void closeWindow(window* instance)
{
	// Zamykamy okno.
	CloseWindow((HWND)instance->system_impl);

	// Zwalniamy pamięć zaalokowaną dynamicznie.
	knurfree(instance);
}

void renderRect(float x, float y, float w, float h)
{
	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txx);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_P);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x, y, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x + w, y, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x + w, y + h, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x, y + h, 0.0f);

	glEnd();


	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_BLEND);
}

void renderFrame(window* instance)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0.0l, 500.0l, 500.0l, 0.0l, -1.0l, 1.0l);
	glViewport(0, 0, 500, 500);

	unsigned long long ts = _ts();

	float delta = ((float)(ts - instance->lastFrame)) / 1000.f;
	instance->lastFrame = ts;
	instance->animationState = instance->animationState + (60.0f * delta * instance->animModifier);


	if (instance->animationState + 200.0 >= instance->width)
	{
		instance->animModifier = -1.0;
	}
	else if (instance->animationState <= 0.0)
	{
		instance->animModifier = 1.0;
	}

	renderRect(0.0 + instance->animationState, 50.0, 200.0, 200.0);

	HDC dc = GetDC(instance->system_impl);
	SwapBuffers(dc);
	ReleaseDC(instance->system_impl, dc);
}

void updateWindow(window* instance)
{
	// Funkcja PeekMessageA pobiera eventy które Windows wysłał do naszego procesu.
	// Funckja TranslateMessage tłumaczy customowy event (np kontrolery do gier, klawiature, etc) na ujednolicony event.
	// A funkcja DispatchMessageA dostarcza ten event do event handlera - jeśli będzie to event związany z naszym oknem to trafi do naszego handlera wyżej nazwanego Wndproc.
	// Dlaczego tak? Bo działamy w obrębie jednego wątku i musimy przetwarzać te eventy w miare ujednolicony sposób żeby nasze okienko się nie zafreezowało.
	MSG msg;

	while (PeekMessageA(&msg, 0, 0, 0, 1)) // Dokumentacja PeekMessageA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
	{
		TranslateMessage(&msg); // Dokumentacja TranslateMessage https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
		DispatchMessageA(&msg); // Dokumentacja DispatchMessageA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessagea
	}
}

int isWindowOpen(window* instance)
{
	if (IsWindow((HWND)instance->system_impl) == TRUE) return 1;
	return 0;
}