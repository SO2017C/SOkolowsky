#pragma once
#include "Biblioteki.h"
#include "Help.h"
//#include "HDD.h"

//HDD DISK; //create for checking all functions

class SHELL{
private:
	const int MAX_dl_lit_as = 2;								// zmienna ktora przechowuje maksymalna ilosc liter funkcji,
																// ktore ma przyjmowac interpreter - ma to na celu zapobiegac
																// nadmiernego wywolywania interpretera
	bool running;												// zmienna ktora sprawia, ze petla jest true, wiec dopki ktos nie wpisze "EXIT" to shell bedzie dzialal
	std::vector<std::string> command_line;						// vector w ktorym przechowujemy: FUNKCJE oraz n-parametrow
	enum spis_funkcji { CF, RF, HELP, EXIT, OTHER, CREDITS, USERADD, DISPLAYUSERS, GROUPADD, DISPLAYGROUPS, SU, GETFACL};			// enumerator w ktorym umieszcze wszystkie nazwy funkcji, wiadomo: CF = 0, RF = 1... etc.
																		// (ew jeszcze mamy OTHER, ktory wywoluje sie przy nieznalezieniu danej funkcji - przesylamy wiec to do interpretera)

																		/*DEKLARACJE INNYCH MODU��W*/
	help_functions h_f;											// spis wszystkich funkcji pomocniczych

																/*FORMATOWANIE DANYCH*/
	spis_funkcji str_to_int(const std::string & Funkcja);		// funkcja ktora przyjmuje wartosc STRING, oraz na jej podstawie zwraca liczb�, dzieki temu moge uzywac switch -> case
	bool are_there_letters(const std::string &s);				// sprawdzam czy w danym stringu sa znaki specjalne / liczby	||||| wtedy nie przyjmujemy go (zwracamy false)
	bool are_there_numbers(const std::string &s);				// sprawdzamy czy w danym stringu sa litery / znaki specjalne	||||| wtedy nie przyjmujemy go (zwracamy false)
	void segregate();											// jezeli mamy 3 parametry i wsrod nich sa dane liczbowe, to wtedy umieszczamy je na 3 miejscu
	void SHELL::letters_to_upper(std::string &s);				// zmieniamy literki, np. aBcdEfGHiJ tak aby wszystkie byly wielkie -> ABCDEFGHIJ


																/* POBIERANIE DANYCH ORAZ PRZESYLANIE DALEJ */
	void command();												// funkcja wpisania komendy przez uzytkownika, sprawdzenie jej poprawnosci oraz sformatowanie jej.
	void run();													// nowa glowna funkcja
	void login();
	
	/* METODY POMOCNICZE */
	void error_r();												// wypisanie bledu, ze niepoprawna funkcja badz mala ilosc parametrow
	void help();												// "help" - wypisanie wszystkich dostepnych funkcji
	void boot();												// wyswietlenie loga
	void credits();												// wyswietlenie tworcow systemu

																/*Funkcje startowe*/


																/* WYJSCIE Z PROGRAMU */
	void exit();												// funkcja wyjscia z programu
	void are_you_sure();										// funkcja sprawdzajaca czy uzytkownik jest pewien, ze chce wyjsc z programu

public:
	SHELL();													// inicjalizacja wszystkich innych modulow w konstruktorze
	void switch_case();											// nasz switch case

};

/*
Prezentacja:
Schemat komendy: [NAZWA_FUNKCJI] [PARAM_1] [PARAM_2]

Pola:
MAX_dl_lit_as - zmienna ktora przechowuje maksymalna ilosc liter funkcji, ktore ma przyjmowac interpreter - ma to na celu zapobiegac nadmiernego wywolywania interpretera
bool running - zmienna ktora sprawia, ze petla do/while jest true, wiec program kreci sie w kolko, dopoki ktos nie zdecyduje sie wyjsc
std::vector<std::string> command_line - vector string�w, przyjmuje tam komende od uzytkownika: FUNKCJA | PARAM_1 | PARAM_2 | PARAM_3
enum spis_funkcji{CF,RF,HELP,EXIT,OTHER,CREDITS} -  enumerator w ktorym umieszcze wszystkie nazwy funkcji, wiadomo: CF = 0, RF = 1... etc. /// co za tym idzie? moge uzywac switch/case

Wszystkie deklaracje modu��w

Metody: 2 glowne
1. Command() - pobieram linijke od uzytkownika, za pomoca regexa odfiltrowuje wiekszosc symboli
po czym jest ta linijka formatowana np. literki w [NAZWA_FUNKCJI] sa zamieniane na wielkie - jest to przyklad case insensitive ktory jest obecny w konsoli CMD
jak i rowniez np. jezeli mamy zmienna int, to przenosze ja na trzecie miejsce (command_line[2]) - umowilismy sie tak, ze funkcje jak beda potrzebowaly, to beda mialy parametry "[PARAM_1] [PARAM_2]"
z czego [PARAM_2] - jest to typ int/string

2. switch_case() -


Interfejs:

*/