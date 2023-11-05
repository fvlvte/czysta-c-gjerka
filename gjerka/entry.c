#include "window.h"
#include "system.h"
#include "image.h"

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


// Miejsce gdzie zaczyna wykonywanie się naszego kodu.
// Dlaczego nie main()? Bo nie używamy żadnej bilbioteki standardowej i piszemy w czystym przewidywalnym C które nic nie robi pod maską.
// Funckja main robi kilka rzeczy pod maską i dlatego z niej rezygnujemy - dzięki temu mamy pewność, że tylko linijki naszego kodu są wykonywane i możemy je w pełni zrozumieć.
// Dlaczego nazwa entry? To bez znaczenia, można ją równie dobrze nazwać dupsko (ale należy powiedzieć o tym linkerowi w jego ustawieniach).
// Typ void - brak zwracanych danych - dlaczego? Bo entry point nie zwraca nic bo on jest na początku i na końcu drzewa wywołań także nie ma do czego zwrócić.
// Jeśli chcemy zwrócić status code i prawidłowo zakończyć program musimy wywołać syscall który to dla nas zrobi (na WinAPI jest to ExitProcess(code) na -nixach jest to exit(code)).
void entry()
{
	//Tu zaczyna się nasz program - jeszcze nic nie zrobił poza inicjalizacją pamięci statycznej (stacka).
	window* window = createWindow("uwu kocham widzuw uwu", 500, 500);


	// Pętla w której przyjmujemy eventy od okna i aktualizujemy je żeby się nie zfreezowało.
	while (isWindowOpen(window)) // Przerywamy tą pętle gdy okno zostanie zamknięte.
	{
		updateWindow(window); // Funkcja która przetwarza eventy które okno otrzymało od systemu operacyjnego i na nie reaguje.
		renderFrame(window);
	}

	// Kończymy nasz program w sposób elegancki i zwracamy do systemu operacyjnego kod jego wyjścia - 2137.
	// Generalnie statusy nie mają żadnego znaczenia chyba, że inny process korzysta z waszego procesu.
	// Wtedy przez ten exit code możecie się komunikować między procesami informując np. czy operacja się udała czy coś poszło nie tak.
	// Dla przykładu mamy program kopiujący plik A do lokacji B, i może on zwrócić 0 gdy operacja udała się bez błędu a wszystkie wartości poniżej 0 będą definiowały błąd.
	// -1 docelowy plik już istnieje, -2 brak miejsca na dysku, ...
	// I drugi program uruchamiający nasz program kopiujący może sprawdzić z jakim statusem zakończył nasz program kopiujący i podjąć stosowną akcję.
	endProcess(2137); // Cross-platfomowy wrapper do zakończenia procesu.
	// Do tej linijki już program nie dojdzie, endProcess nie zwraca i natychmiastowo zakańcza działanie programu.
}