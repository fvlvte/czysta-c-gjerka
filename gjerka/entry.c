#include <Windows.h>

// {typ} {nazwa symbolu}({typ} {nazwa})
// {
//		BODY
//		return {typ}
// }

/*
* {struct keyword} {nazwa}
* {
*   {typ} {nazwa}
* }
*/

/*
#define {nazwa} {wartość}

#define dupeczka 10
#define dupsztal "10"
*/

struct stopy
{
	int lewa;
	int prawa;
};


LRESULT Wndproc(
	HWND unnamedParam1,
	UINT unnamedParam2,
	WPARAM unnamedParam3,
	LPARAM unnamedParam4
)
{
	return DefWindowProcA(unnamedParam1, unnamedParam2, unnamedParam3, unnamedParam4);
}

void createWindow()
{
	WNDCLASSEXA windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = &Wndproc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "uwusnaGiereczkaOkienkoKlasa2137";
	windowClass.hIconSm = NULL;

	ATOM registeredClass = RegisterClassExA(&windowClass);

	HWND window = CreateWindowExA(
		0,
		(LPCSTR)registeredClass,
		"uwu kocham widzuw uwu",
		WS_TILEDWINDOW,
		500,
		500,
		500,
		500,
		NULL,
		NULL,
		NULL,
		NULL
	);

	ShowWindow(window, SW_SHOW);

	MSG msg;
	while (GetMessageA(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void entry()
{
	createWindow();

	ExitProcess(2137);
}