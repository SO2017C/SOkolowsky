#include "Shell.h"
#include "HDD.h"
#include "ACL.h"


HDD DISK;
Uprawnienia uprawnienia;

//User administartor = { "admin","admin" };

/* FORMATOWANIE DANYCH */
SHELL::spis_funkcji SHELL::str_to_int(const std::string & Funkcja)
{
	if (Funkcja == "CF") return CF;
	else if (Funkcja == "RF") return RF;
	else if (Funkcja == "HELP") return HELP;
	else if (Funkcja == "CREDITS")return CREDITS;
	else if (Funkcja == "EXIT") return EXIT;
	else if (Funkcja == "USERADD") return USERADD;
	else if (Funkcja == "DISPLAYUSERS") return DISPLAYUSERS;
	else if (Funkcja == "DISPLAYGROUPS") return DISPLAYGROUPS;
	else if (Funkcja == "SU") return SU;
	else if (Funkcja == "GETFACL") return GETFACL;
	else return OTHER;
}
bool SHELL::are_there_letters(const std::string &s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] < 65 || s[i] >= 91 && s[i] <= 96 || s[i]>123) // Odsy³am do tabeli ASCII
			return false;
	}
	return true;
}
bool SHELL::are_there_numbers(const std::string &s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] < 48 || s[i]>57)// Odsy³am do tabeli ASCII
			return false;
	}
	return true;
}
void SHELL::segregate()
{
	if (are_there_numbers(command_line[1])) // jesli cyfry sa w drugim miejscu command_line (przyp. 1 - funkcja, 2 - parametr, 3 paramter), to przekladamy je na trzecie miejsce
		std::swap(command_line[1], command_line[2]);
}
void SHELL::letters_to_upper(std::string &s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::toupper); // przechodzi po kazdej literce oraz jezeli jest mala - zmienia ja na wielka
}

/* POBIERANIE DANYCH ORAZ PRZESYLANIE DALEJ */
//void login()
//{
//	std::string login, haslo;
//	while (true)
//	{
//		std::cout << "Podaj login: "; std::cin >> login;
//		std::cout << "Podaj haslo: "; std::cin >> haslo;
//		uprawnienia.przelacz_uzytkownika(login, haslo);
//	
//	}
//}
void SHELL::command()
{

	std::string zdanie;
	std::getline(std::cin, zdanie);									// pobranie linijki wypisanej przez uzytkownika
	std::smatch sm;
	std::regex regex("[a-zA-Z][a-zA-Z0-9_]*|[1-9][0-9]*|[/][?]");	// minimum 1 literka, string zaczyna sie od litery moze zawierac "_" oraz liczby, np. program_1 | p_1 | p
																	// liczba zaczyna sie od 1: czyli nie mo¿na zrobiæ 001 - wlasciwie to uzytkownik moze to wpisac, ale my zobaczymy tylko 1...
																	// no i nasz parametr specjalny: /? (wywolanie pomocy)

	while (std::regex_search(zdanie, sm, regex))					// za pomoca wyrazenia regularnego wyfiltrowanie rzeczy, ktore wypisal uzytkownik
	{
		command_line.push_back(sm.str());
		zdanie = sm.suffix();
	}
	if (command_line.size() < 1)									// jesli ktos podal zdanie zawierajacy tylko: " \ / | % ! - = * +, to wtedy size jest rowny 0, gdyz regex go nie wylapie
	{
		error_r();
		std::cout << "::> ";
		command_line.clear();
	}
	else if (are_there_letters(command_line[0]))					// jesli w pierwszym wyrazie sa literki
	{
		letters_to_upper(command_line[0]);							// zmieniamy je na duze litery: cF -> CF / cf -> CF
		if (command_line.size() == 3)								// rowniez jesli wielkosc komendy jest rowna 3
			segregate();											// segregujemy, tak ze int zawsze jest na 3 pozycji --- wytlumaczenie: ping -n 10 8.8.8.8 == ping 8.8.8.8 -n 10 | 
	}																// w CMD kolejnosc nie ma znaczenia, rowniez chcialem aby moj shell byl podobny
	else															// jezeli ktos wpisze np. tylko: ";)" "$$" to wtedy command_line.size() == 0, czyli wywolujemy ze ma podac w 1. parametrze funkcje
	{
		std::cout << "Prosze podac w pierwszym parametrze funkcje" << std::endl << std::endl;
		std::cout << "::> ";
		command_line.clear();
	}
}
void SHELL::run()
{
	do {
		std::cout <<uprawnienia.zwroc_zalogowanego_uzytkownika().name << "::> ";									 // wypisanie naszego "znaku poczatku komendy" - czy jak to nazwac
		command_line.clear();									 // wyczyszczenie command_line z poprzedniej komendy (tej ktora zostala wykonana)
		while (command_line.size() <= 0)						 // jezeli ktos podal np. "||" jako nazwe funkcji, to petla sie powtarza lub jesli sa jakies nieprawidlowe parametry
		{
			command();											 // wywolanie funkcji pobierajacej komende od uzytkownika
		}
		switch_case();											 // w przypadku komendy, ktora wydaje sie w miare "poprawna" wywolujemy switch_case()

	} while (running);											 // program bedzie sie wykonywal w nieskonczonosc dopoki uzytkownik go nie przerwie
}
void SHELL::switch_case()
{
	switch (str_to_int(command_line[0]))
	{
		/// DYSK
	case CF:
		if (command_line.size() == 2 && command_line[1] == "/?")									// CF + 1 parametr, ktorym jest "/?"
		{
			h_f.CreateFile_inf();
		}
		else if (command_line.size() == 4 && are_there_numbers(command_line[2]))							// CF + 2 parametry ( z czego jeden parametr to int - wielkosc pliku a drugi to string - nazwa pliku)
		{
			DISK.create_file(command_line[1], std::stoi(command_line[2]), command_line[3]);						// w powyzszym przypadku, bede odwolywal sie do modulu "DISK" oraz wywolywal funkcje "Create_file"
			uprawnienia.stworzACL(command_line[1]);//TU IF CZY SIE UDALO STWORZYC!
			//uprawnienia.getfacl(command_line[1]);
		}
		else
		{
			error_r();																				// Jezeli mamy 1 lub >2 parametry, to wtedy wywolujemy funkcje error_r();
		}
		break;
	case RF:																						// tak samo bede robil dla kazdej innej funkcji
		if (command_line.size() == 2 && command_line[1] == "/?")
		{
			//h_f.ReadFile_inf();
		}
		else if (command_line.size() == 2 && command_line[1] != "/?")
		{
			if (uprawnienia.zgoda_na_odczyt(command_line[1]) == true) {
				std::cout << DISK.read_file(command_line[1]) << std::endl;
			}

		}
		else
		{
			error_r();																				// Jezeli mamy 1 lub >2 parametry, to wtedy wywolujemy funkcje error_r();
		}

		break;


		/// Kamil
	case USERADD:
		if (command_line.size() == 3)
		{
			uprawnienia.dodaj_uzytkownika(command_line[1], command_line[2]);
		}
		else if (command_line.size() == 2)
		{
			uprawnienia.dodaj_uzytkownika(command_line[1], "");
		}
		else
		{
			error_r();																				// Jezeli mamy 1 lub >2 parametry, to wtedy wywolujemy funkcje error_r();
		}
		break;

	case GROUPADD:
		if (command_line.size() == 3 || command_line.size() == 2)
		{
			uprawnienia.dodaj_uzytkownika(command_line[1], command_line[2]);
		}
		else
		{
			error_r();																				// Jezeli mamy 1 lub >2 parametry, to wtedy wywolujemy funkcje error_r();
		}
		break;
	case DISPLAYUSERS:
		uprawnienia.wyswietl_uzytkownikow();
		break;
	case DISPLAYGROUPS:
		uprawnienia.wyswietl_grupy();
		break;
	case SU:
		if (command_line.size() == 3)
			uprawnienia.przelacz_uzytkownika(command_line[1], command_line[2]);
		else if (command_line.size() == 2)
			uprawnienia.przelacz_uzytkownika(command_line[1], "");
		else
		{
			error_r();																				// Jezeli mamy 1 lub >2 parametry, to wtedy wywolujemy funkcje error_r();
		}
		break;

	case GETFACL:
		uprawnienia.getfacl(command_line[1]);
		break;
	case HELP:
		help();
		break;
	case EXIT:
		exit();
		break;
	case CREDITS:
		credits();
		break;
	case OTHER:																											// szczegolny przypadek gdy nie znajde funkcji u siebie
																														// interpreter.COSTAM(command_line);
		if (command_line[0].size() == MAX_dl_lit_as)																	// jezeli nazwa funkcji ma 2 znaki, to przesylam do interpretera
			std::cout << "Przeslanie komendy wpisanej przez uzytkownika do interpretera" << std::endl << std::endl;
		else
			error_r();																									// jezeli nie, to wywoluje metode error_r();
		break;
	}
}

/* METODY POMOCNICZE */
void SHELL::error_r()
{
	std::cout << "Prosze wpisac poprawna funkcje oraz jej parametry, jezeli nie wiesz jakie sa napisz: HELP,\npo dokladniejsze informacje napisz: NAZWAFUNKCJI /?" << std::endl << std::endl;
}
void SHELL::help()
{
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	std::cout << "Dostepne sa podane funkcje" << std::endl;
	std::cout << "CF   RF   HELP   CREDITS   EXIT" << std::endl << std::endl;
	std::cout << "Aby poznac parametry wpisz: \"FUNKCJA /?\" np. CF /?" << std::endl;
	std::cout << std::endl << std::endl;
}
void SHELL::boot() // wyswietlenie loga
{
	std::cout << "___________________$$$$$$______$$$$$$$$________________________________________________________________________________" << std::endl;
	std::cout << "_________________$$______$$__$$______$$________________________________________________________________________________" << std::endl;
	std::cout << "________________$$__________$$______$$_________________________________________________________________________________" << std::endl;
	std::cout << "_______________$$__________$$______$$____$$______________________________________________________$$____________________" << std::endl;
	std::cout << "________________$$$$$$____$$______$$____$$___$$__________$$_____________________________________$$___$$________________" << std::endl;
	std::cout << "_____________________$$__$$______$$____$$_$$____$$$$$$__$$______$$$$$$__$$________$$___$$$$$$__$$_$$___________________" << std::endl;
	std::cout << "____________________$$__$$______$$____$$$$_____$$__$$__$$______$$__$$___$$______$$___$$___$$__$$$$____$$____$$_________" << std::endl;
	std::cout << "___________$$_____$$___$$______$$____$$__$$___$$__$$__$$______$$__$$____$$_$__$$_________$$__$$__$$____$$__$$__________" << std::endl;
	std::cout << "____________$$$$$$______$$$$$$$$____$$____$$_$$$$$$__$$$$$$__$$$$$$_____$$__$$______$$$$$$__$$____$$____$$_____________" << std::endl;
	std::cout << "_______________________________________________________________________________________________________$$______________" << std::endl;
	std::cout << "_________$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$_______________" << std::endl;
	std::cout << "_______________________________________________________________________________________________________________________" << std::endl;
	std::cout << "                                                                             .                                         " << std::endl;
	std::cout << "                                        `#.:`                            `+.#,;                                        " << std::endl;
	std::cout << "                                       `+#+'+'`',                     '.;''++',                                        " << std::endl;
	std::cout << "                                       ,+++'''''''                  ,;;'''''+#:                                        " << std::endl;
	std::cout << "                                       .+#+''';;;;;:              ,;;;;;;''''',                                        " << std::endl;
	std::cout << "                                       `++''''';;;;;::          ,:;;;;;;;''''+'                                        " << std::endl;
	std::cout << "                                        ;++'''+'';;;;;;:  `.` ,:;;;;;;;''''++#`                                        " << std::endl;
	std::cout << "                                        `,;'''';';;;;;;';;;;;;';;;;;;;';;''''.                                         " << std::endl;
	std::cout << "                                         `:+'''''';;;;;;;';;';;;;;;;';;'';'+.                                          " << std::endl;
	std::cout << "                                          `,++''';;';;;;;'''';''';''''';'+,.                                           " << std::endl;
	std::cout << "                                            `,':+'';;;:;;';+;':;;';'''':,`                                             " << std::endl;
	std::cout << "                                              ``.,:;;;,..;';;;``,,,,,.`                                                " << std::endl;
	std::cout << "                                                   ``  .;':;:';;                                                       " << std::endl;
	std::cout << "                                                      `';;:':;;;                                                       " << std::endl;
	std::cout << "                                                       .+';';'+`                                                       " << std::endl;
	std::cout << "                                                         `'+:.                                                         " << std::endl;
	std::cout << "                                                          ```                                                          " << std::endl;

	std::cout << "\t\t\t\t\t\t  Press enter to start";
	std::cin.get();
	system("cls");

	std::cout << "SOkolowsky [Version 0.4.263]" << std::endl;
	std::cout << "<c> 2018 SOkolowsky Project Group. Wszelkie prawa zastrzezone." << std::endl;
}
void SHELL::credits()
{
	system("cls");
	std::cout << "SOkolowsky, zostal zaprojektowany przez: " << std::endl << std::endl;
	std::cout << "Karol Rosiak \t\t Mechanizm synchronizacji" << std::endl;
	std::cout << "Marcin Stasiak \t\t Zarzadzanie procesami" << std::endl;
	std::cout << "Kamil Rogowski \t\t Konta/grupy" << std::endl;
	std::cout << "Kamil Zielinski \t Shell" << std::endl;
	std::cout << "Patryk Romaniak \t Zarzadzanie pamiecia" << std::endl;
	std::cout << "Eryk Sokolowski \t Zadania w assemblerze" << std::endl;
	std::cout << "Damian Reczulski \t Zarzadzanie procesorem" << std::endl;
	std::cout << "Maksymilian Slowinski \t Komunikacja miedzy procesorami" << std::endl << std::endl << std::endl;
}

/* WYJSCIE Z PROGRAMU */
void SHELL::exit()
{
	std::string pom;

	std::cout << "Czy na pewno chcesz opuscic? [Yes/No]: "; std::getline(std::cin, pom);					// pobranie linijki
	if (are_there_letters(pom))																				// jesli podal literki, to zmieniamy je na wielkie litery
		letters_to_upper(pom);

	if (pom[0] == 'Y' || pom == "YE" || pom == "YES")														// jesli uzytkownik zdecyduje sie wyjsc, wyswietlamy ponowne zapytanie
	{																										// w windowsie zauwazylem, ze niewazne czy napiszemy "Y" / "YE" / "YES" to zawsze 
		are_you_sure();																						// dane zapytanie zakonczy sie pozytywnie
	}
	else if (pom[0] == 'N' || pom == "NO")
	{
		std::cout << "Cieszymy sie, ze jeszcze zostaniesz z nami przez chwile :)" << std::endl << std::endl;
	}
	else
	{
		exit();																								// jezeli uzytkownik wpisze cokolwiek innego, np. $ / B  itp, to ponawiamy zapytanie
	}
}
void SHELL::are_you_sure()
{
	std::string pom;
	std::cout << "Czy jestes 100% pewny, ze chcesz opuscic? [Yes/No]: "; std::getline(std::cin, pom);
	if (are_there_letters(pom))
		letters_to_upper(pom);

	if (pom[0] == 'Y' || pom == "YE" || pom == "YES")											// jesli bedzie na 100% pewny, to running = false - czyli wylaczamy system  
	{																							// rowniez tutaj bedziemy musieli wszystkie zmienne dynamiczne usunac, zeby wyciekow pamieci nie bylo
																								/*
																								usuwanie zmiennych dynamicznych - aby zapobiec wyciekom pamieci
																								*/
		running = false;																		// gdyz po tej komendzie program sie wylaczy
	}
	else if (pom[0] == 'N' || pom == "NO")
	{
		std::cout << "uff bylo blisko :)" << std::endl << std::endl;
	}
	else
	{
		are_you_sure();																			// jezeli uzytkownik wpisze cokolwiek innego, np. $ / B  itp, to ponawiamy zapytanie
	}
}

/* KONSTRUKTOR */
SHELL::SHELL()
{
	running = true;
	boot(); // wyswietlenie loga
	run();
}





//"[a-zA-Z_!@#$%^&()+-={}';.,][a-zA-Z0-9_!@#$%^&()+-={}';.,]*




/*

case AD:
case MV:
case RX:
...
case SV:
interpreter(command_line);
break;
case OTHER:
error_r();

*/








/*
std::cout << "                                                                             .                                         " << std::endl;
std::cout << "                                        `#.:`                            `+.#,;                                        " << std::endl;
std::cout << "                                       `+#+'+'`',                     '.;''++',                                        " << std::endl;
std::cout << "                                       ,+++'''''''                  ,;;'''''+#:                                        " << std::endl;
std::cout << "                                       .+#+''';;;;;:              ,;;;;;;''''',                                        " << std::endl;
std::cout << "                                       `++''''';;;;;::          ,:;;;;;;;''''+'                                        " << std::endl;
std::cout << "                                        ;++'''+'';;;;;;:  `.` ,:;;;;;;;''''++#`                                        " << std::endl;
std::cout << "                                        `,;'''';';;;;;;';;;;;;';;;;;;;';;''''.                                         " << std::endl;
std::cout << "                                         `:+'''''';;;;;;;';;';;;;;;;';;'';'+.                                          " << std::endl;
std::cout << "                                          `,++''';;';;;;;'''';''';''''';'+,.                                           " << std::endl;
std::cout << "                                            `,':+'';;;:;;';+;':;;';'''':,`                                             " << std::endl;
std::cout << "                                              ``.,:;;;,..;';;;``,,,,,.`                                                " << std::endl;
std::cout << "                                                   ``  .;':;:';;                                                       " << std::endl;
std::cout << "                                                      `';;:':;;;                                                       " << std::endl;
std::cout << "                                                       .+';';'+`                                                       " << std::endl;
std::cout << "                                                         `'+:.                                                         " << std::endl;
std::cout << "                                                          ```                                                          " << std::endl;

std::cout << "_____________________________________________________________________________._________________________________________" << std::endl;
std::cout << "________________________________________`#.:`____________________________`+.#,;________________________________________" << std::endl;
std::cout << "_______________________________________`+#+'+'`',_____________________'.;''++',________________________________________" << std::endl;
std::cout << "_______________________________________,+++'''''''__________________,;;'''''+#:________________________________________" << std::endl;
std::cout << "_______________________________________.+#+''';;;;;:______________,;;;;;;''''',________________________________________" << std::endl;
std::cout << "_______________________________________`++''''';;;;;::__________,:;;;;;;;''''+'________________________________________" << std::endl;
std::cout << "________________________________________;++'''+'';;;;;;:__`.`_,:;;;;;;;''''++#`________________________________________" << std::endl;
std::cout << "________________________________________`,;'''';';;;;;;';;;;;;';;;;;;;';;''''._________________________________________" << std::endl;
std::cout << "_________________________________________`:+'''''';;;;;;;';;';;;;;;;';;'';'+.__________________________________________" << std::endl;
std::cout << "__________________________________________`,++''';;';;;;;'''';''';''''';'+,.___________________________________________" << std::endl;
std::cout << "____________________________________________`,':+'';;;:;;';+;':;;';'''':,`_____________________________________________" << std::endl;
std::cout << "______________________________________________``.,:;;;,..;';;;``,,,,,.`________________________________________________" << std::endl;
std::cout << "___________________________________________________``__.;':;:';;_______________________________________________________" << std::endl;
std::cout << "______________________________________________________`';;:':;;;_______________________________________________________" << std::endl;
std::cout << "_______________________________________________________.+';';'+`_______________________________________________________" << std::endl;
std::cout << "_________________________________________________________`'+:._________________________________________________________" << std::endl;
std::cout << "__________________________________________________________```__________________________________________________________" << std::endl;

*/











