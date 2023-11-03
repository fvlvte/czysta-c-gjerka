#include <Windows.h> // Bibloteka WinAPI - jedyna z której będziemy korzystać.

// Konstrukt funckji
// {typ} {nazwa symbolu}({typ} {nazwa})
// {
//		BODY
//		return {typ}
// }

// Konstrukt struktury
/*
* {struct keyword} {nazwa}
* {
*   {typ} {nazwa}
* }
*/

// Konstrukt preprocesora
/*
#define {nazwa} {wartość}

#define dupeczka 10
#define dupsztal "10"
*/

/*
* Konstrukt zmiennej:
* [typ][*-wskaźnik od tego typu opcjonalnie] [nazwa];
* Konstrukt zmiennje z inicjalizacją:
* [typ][*-wskaźnik od tego typu opcjonalnie] [nazwa] = [wartość];
*/

// Funckja typu boilerplate przyjmująca zdarzenia o oknie od systemu operacyjnego.
// Aktualnie mock up, będziemy rozwijać ją w przyszłości.
LRESULT Wndproc(
	HWND unnamedParam1,
	UINT unnamedParam2,
	WPARAM unnamedParam3,
	LPARAM unnamedParam4
)
{
	return DefWindowProcA(unnamedParam1, unnamedParam2, unnamedParam3, unnamedParam4);
}

// Nasza funkcja boża do tworzenia okienka w Windowsie.
void createWindow()
{
	// Okno na Windowsie tworzymy w 2 etapach.
	// Na samym początku musimy stworzyć klasę okna którą w dużym uproszczeniu możemy przyrównać do CSS w HTML-u.
	// Są to poprostu cechy okna które możemy recyklować i używać w wielu oknach.
	// My stworzymy bardzo generyczne Windowsowe okienko które pozwoli nam na rysowanie w nim OpenGL-em.

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
	windowClass.lpfnWndProc = &Wndproc;
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

	// ATOM to customowy typ microsoftu ale generalnie jest to zwykły uchwyt.
	// Custom typy są po to żeby programista sie nie pomylił z typem uchwytu generalnie.
	// Wywołujemy RegisterClassExA podając wskaźnik do naszej struktury klasy okna żeby poinformować Windows, że chcemy ją stworzyć.
	// On zwróci nam do registeredClass właśnie uchwyt do naszej klasy który będzie przydatny w momencie chęci jej wykorzystania.
	ATOM registeredClass = 
		RegisterClassExA(&windowClass); // Dokumentacja RegisterClassExA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa

	// No w końcu to co kociaki lubią najbardziej czyli tworzenie naszego okienka bożego.
	// Funckja CreateWindowExA utworzy nasze okno i zwróci nam do niego uchwyt.
	// Co to są te uchwyty? To jest po prostu zmienna która jest wskaźnikiem do naszego okna.
	// Ona generalnie mówi o jakie okno chodzi systemowi operacyjnemu jeśli używamy funkcji manipulujących oknem co będzie widać niżej.
	HWND window = CreateWindowExA( // Dokumentacja CreateWindowExA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		0, // Flagi stylów rozszerzonych ale mamy w dupie CSSy.
		(LPCSTR)registeredClass, // Uchwyt albo nazwa naszej klasy okna - ja wybrałem uchwyt - ale castuje go do typu jaki ta funkcja chce żeby kompilator nie płakał.
		"uwu kocham widzuw uwu", // Nazwa naszego okienka.
		WS_TILEDWINDOW, // Styl okna - WS_TILEDWINDOW to takie najbardziej generyczne okno windowsowe.
		500, // Kordynat X gdzie ma pojawić się na ekranie.
		500, // Kordynat Y gdzie ma pojawić się na ekranie.
		500, // Szerokość w px.
		500, // Wysokość w px.
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
		window, // uchwyt do okna o które nam chodzi - bo generalnie możemy z naszego procesu pokazać okna nawet z innego procesu więc te uchwyty są ważne
		SW_SHOW // SW_SHOW to flaga która mówi, że windows ma pokazać to okno, analogicznie SW_HIDE ukryje to okno
	);

	// Już prawie czas na świętowanie ale jeszcze tylko event loop.
	// Musimy jakoś zapętlić nasz program bo jeśli wyjdziemy z tej funkcji nasz proces się skończy i zamknie nasze okno zanim my je nawet zobaczymy.
	// Event loop okna na windowsie wygląda praktycznie zawsze tak samo.
	// Funkcja GetMessageA pobiera eventy które Windows wysłał do naszego procesu.
	// Funckja TranslateMessage tłumaczy customowy event (np kontrolery do gier, klawiature, etc) na ujednolicony event.
	// A funkcja DispatchMessageA dostarcza ten event do event handlera - jeśli będzie to event związany z naszym oknem to trafi do naszego handlera wyżej nazwanego Wndproc.
	// Dlaczego tak? Bo działamy w obrębie jednego wątku i musimy przetwarzać te eventy w miare ujednolicony sposób żeby nasze okienko się nie zafreezowało.
	MSG msg;
	while (GetMessageA(&msg, 0, 0, 0)) // Dokumentacja GetMessageA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessagea
	{
		TranslateMessage(&msg); // Dokumentacja TranslateMessage https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
		DispatchMessageA(&msg); // Dokumentacja DispatchMessageA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessagea
	}
}


// Miejsce gdzie zaczyna wykonywanie się naszego kodu.
// Dlaczego nie main()? Bo nie używamy żadnej bilbioteki standardowej i piszemy w czystym przewidywalnym C które nic nie robi pod maską.
// Funckja main robi kilka rzeczy pod maską i dlatego z niej rezygnujemy - dzięki temu mamy pewność, że tylko linijki naszego kodu są wykonywane i możemy je w pełni zrozumieć.
// Dlaczego nazwa entry? To bez znaczenia, można ją równie dobrze nazwać dupsko (ale należy powiedzieć o tym linkerowi w jego ustawieniach).
// Typ void - brak zwracanych danych - dlaczego? Bo entry point nie zwraca nic bo on jest na początku i na końcu drzewa wywołań także nie ma do czego zwrócić.
// Jeśli chcemy zwrócić status code i prawidłowo zakończyć program musimy wywołać syscall który to dla nas zrobi (na WinAPI jest to ExitProcess(code) na -nixach jest to exit(code)).
void entry()
{
	//Tu zaczyna się nasz program - jeszcze nic nie zrobił poza inicjalizacją pamięci statycznej (stacka).
	
	// Odpalamy naszą funkcje tworzącą okno którą napisaliśmy wyżej.
	createWindow();

	// Kończymy nasz program w sposób elegancki i zwracamy do systemu operacyjnego kod jego wyjścia - 2137.
	// Generalnie statusy nie mają żadnego znaczenia chyba, że inny process korzysta z waszego procesu.
	// Wtedy przez ten exit code możecie się komunikować między procesami informując np. czy operacja się udała czy coś poszło nie tak.
	// Dla przykładu mamy program kopiujący plik A do lokacji B, i może on zwrócić 0 gdy operacja udała się bez błędu a wszystkie wartości poniżej 0 będą definiowały błąd.
	// -1 docelowy plik już istnieje, -2 brak miejsca na dysku, ...
	// I drugi program uruchamiający nasz program kopiujący może sprawdzić z jakim statusem zakończył nasz program kopiujący i podjąć stosowną akcję.
	ExitProcess(2137);
	// Do tej linijki już program nie dojdzie, ExitProcess nie zwraca i natychmiastowo zakańcza działanie programu.
}