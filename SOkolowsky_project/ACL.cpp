// ACL_modul3.cpp : Defines the entry point for the console application.
//

#include"ACL.h"

void Uprawnienia::dodaj_uzytkownika(std::string nazwa, std::string haslo) {
	User u;
	bool nie_powtarza_sie = true;
	for (auto e : wszyscy_uzytkownicy)
		if (e.name == nazwa)
			nie_powtarza_sie = false;
	if (nie_powtarza_sie) {
		u.name = nazwa;
		u.password = haslo;
		u.group = "uzytkownicy";
		wszyscy_uzytkownicy.push_back(u);

		for (auto z : wszystkie_grupy)
			if (z.name == "uzytkownicy")
				z.users_in_group.push_back(u);
	}
	else std::cout << "\nUzytkownik o tej nazwie juz istnieje\n\n";

}

void Uprawnienia::dodaj_grupe(std::string nazwa) {
	Group g;
	bool nie_powtarza_sie = true;
	for (auto e : wszystkie_grupy)
		if (e.name == nazwa)
			nie_powtarza_sie = false;
	if (nie_powtarza_sie) {
		g.name = nazwa;
		wszystkie_grupy.push_back(g);
	}

}

void Uprawnienia::wyswietl_uzytkownikow() {
	std::cout << "\n";
	for (auto e : wszyscy_uzytkownicy)
		std::cout << e.name << " " << e.password << "\n";
	std::cout << "\n";
}

void Uprawnienia::wyswietl_grupy() {
	std::cout << "\n";
	for (auto e : wszystkie_grupy) {
		std::cout << e.name << ": ";
		for (auto z : e.users_in_group)
			std::cout << z.name << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Uprawnienia::przelacz_uzytkownika(std::string name, std::string haslo) {
	bool flaga = false;
	for (int i = 0; i < wszyscy_uzytkownicy.size(); i++) {
		if (wszyscy_uzytkownicy[i].name == name) {
			if (wszyscy_uzytkownicy[i].password == haslo) {
				zalogowany_uzytkownik = wszyscy_uzytkownicy[i];
			}
			else {
				std::cout << "\nhaslo nieprawidlowe\n";
			}
			flaga = true;
			break;
		}
	}
	if (!flaga)
		std::cout << "\nuzytkownik " << name << " nie istnieje\n\n";
}

void Uprawnienia::dodaj_do_grupy(std::string nazwa, std::string grupa) {
	bool flaga1 = false;
	bool flaga2 = false;
	User u;
	for (auto e : wszyscy_uzytkownicy)
		if (e.name == nazwa)
			u = e;

	for (int i = 0; i < wszystkie_grupy.size(); i++) {
		if (wszystkie_grupy[i].name == "administratorzy") {
			for (auto e : wszystkie_grupy[i].users_in_group) {
				if (e.name == zalogowany_uzytkownik.name) {
					flaga1 = true;
					break;
				}
			}
		}
	}
	for (int i = 0; i < wszystkie_grupy.size(); i++) {
		if (wszystkie_grupy[i].name == grupa) {
			flaga2 = true;
			if (flaga1)
				wszystkie_grupy[i].users_in_group.push_back(u);
			break;
		}
	}
	if (!flaga2)
		std::cout << "\nPodana grupa uzytkownikow nie istnieje\n";

	if (!flaga1)
		std::cout << "\nBrak uprawnien do wykonania okreslonej operacji.\nZaloguj sie na konto administratora\n";
}

//int main()
//{
//	Uprawnienia uprawnienia;
//
//	std::string polecenie = "";
//	std::string wyraz = "";
//	std::string haslo = "";
//	std::string grupa_uzytkownika = "";
//	int licznik = 0;
//	bool flaga1 = false;
//
//	while (polecenie != "exit") {
//		std::cout << uprawnienia.zwroc_nazwe_zalogowanego_uzytkownika() << "::>";
//		std::getline(std::cin, polecenie);
//
//		if (polecenie[0] == 'g' && polecenie[1] == 'r' && polecenie[2] == 'o' &&			// np. groupadd uczniowie
//			polecenie[3] == 'u' && polecenie[4] == 'p' && polecenie[5] == 'a'
//			&& polecenie[6] == 'd' && polecenie[7] == 'd') {
//			flaga1 = true;
//			if (polecenie.size() > 9) {
//				for (int i = 9; i < polecenie.size(); i++) {
//					if (polecenie[i] == 32)
//						break;
//					wyraz += polecenie[i];
//				}
//				uprawnienia.dodaj_grupe(wyraz);
//				wyraz = "";
//			}
//			else std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//				"\noperable program or batch file.\n";
//
//		}
//
//		if (polecenie[0] == 'u' && polecenie[1] == 's' && polecenie[2] == 'e' &&			// np. useradd jan 123
//			polecenie[3] == 'r' && polecenie[4] == 'a' && polecenie[5] == 'd'
//			&& polecenie[6] == 'd') {
//			flaga1 = true;
//			if (polecenie.size() > 8) {
//				licznik = 7;
//				for (int i = 8; i < polecenie.size(); i++) {
//					licznik++;
//					if (polecenie[i] == 32)
//						break;
//					wyraz += polecenie[i];
//				}
//				licznik++;
//				if (polecenie.size()>licznik) {
//					for (int j = licznik; j < polecenie.size(); j++) {
//						if (polecenie[j] == 32)
//							break;
//						haslo += polecenie[j];
//					}
//					uprawnienia.dodaj_uzytkownika(wyraz, haslo);
//					wyraz = "";
//					haslo = "";
//				}
//				else if (polecenie.size() <= licznik) {
//					haslo = "";
//					uprawnienia.dodaj_uzytkownika(wyraz, haslo);
//					wyraz = "";
//				}
//
//				//else std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//				//"\noperable program or batch file.\n";
//
//				wyraz = "";
//				haslo = "";
//			}
//			else std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//				"\noperable program or batch file.\n\n";
//		}
//
//		if (polecenie == "display_groups") {		// wyswietlanie grup na razie w takiej formie
//
//			flaga1 = true;
//			uprawnienia.wyswietl_grupy();
//		}
//
//		if (polecenie == "display_users") {			// wyswietlanie uzytkownikow na razie w takiej formie
//			flaga1 = true;
//			uprawnienia.wyswietl_uzytkownikow();
//		}
//
//		if (polecenie[0] == 'c' && polecenie[1] == 'f') { // tworzenie pliku zrobilem tak dla sprawdzenia u siebie np. cf plik1
//			flaga1 = true;
//			if (polecenie.size() > 3) {
//				for (int i = 3; i < polecenie.size(); i++) {
//					if (polecenie[i] == 32)
//						break;
//					wyraz += polecenie[i];
//				}
//				uprawnienia.stworzACL(wyraz);
//				wyraz = "";
//			}
//			else std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//				"\noperable program or batch file.\n\n";
//		}
//		if (polecenie[0] == 's' && polecenie[1] == 'u') {			//zmiana uzytkownika np. su janusz 123
//			flaga1 = true;
//			if (polecenie.size() > 3) {
//				licznik = 2;
//				for (int i = 3; i < polecenie.size(); i++) {
//					licznik++;
//					if (polecenie[i] == 32)
//						break;
//					wyraz += polecenie[i];
//				}
//				licznik++;
//				if (polecenie.size() > licznik) {
//					for (int j = licznik; j < polecenie.size(); j++) {
//						if (polecenie[j] == 32)
//							break;
//						haslo += polecenie[j];
//					}
//
//				}
//				if (polecenie.size() <= licznik)
//					haslo = "";
//				uprawnienia.przelacz_uzytkownika(wyraz, haslo);
//				wyraz = "";
//				haslo = "";
//			}
//			else std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//				"\noperable program or batch file.\n\n";
//		}
//
//		if (polecenie[0] == 'u' && polecenie[1] == 's' && polecenie[2] == 'e' &&		// tylko uzytkownicy z grupy administratorzy moga																												
//			polecenie[3] == 'r' && polecenie[4] == 'm' && polecenie[5] == 'o'			// przydzielac innych uzytkownikow do okreslonych grup
//			&& polecenie[6] == 'd') {													// np usermod janusz uczniowie
//			flaga1 = true;
//			if (polecenie.size() > 8) {
//				licznik = 7;
//				for (int i = 8; i < polecenie.size(); i++) {
//					licznik++;
//					if (polecenie[i] == 32)
//						break;
//					wyraz += polecenie[i];
//				}
//				licznik++;
//				if (polecenie.size() > licznik) {
//					for (int j = licznik; j < polecenie.size(); j++) {
//						if (polecenie[j] == 32)
//							break;
//						grupa_uzytkownika += polecenie[j];
//					}
//					uprawnienia.dodaj_do_grupy(wyraz, grupa_uzytkownika);
//					wyraz = "";
//					grupa_uzytkownika = "";
//				}
//				else {
//					wyraz = "";
//					std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//						"\noperable program or batch file.\n\n";
//				}
//			}
//			else std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//				"\noperable program or batch file.\n\n";
//		}
//
//		if (polecenie[0] == 'g' && polecenie[1] == 'e' && polecenie[2] == 't' &&			//wyswietlanie listy kontroli dostepu (ACL) konkretnego pliku 
//			polecenie[3] == 'f' && polecenie[4] == 'a' && polecenie[5] == 'c'				// np. getfacl plik1
//			&& polecenie[6] == 'l') {
//			flaga1 = true;																	//UWAGA! bedzie jeszcze polecenie o schemacie: setfacl okreslone_uprawnienia nazwa_pliku
//			if (polecenie.size() > 8) {														// setfacl sluzy do ustalania uprawnien do pliku
//				for (int i = 8; i < polecenie.size(); i++) {								// ciezko bedzie rozpoznac jego parametry przy wprowadzaniu w konsoli, ale jakos sie to zrobi
//					if (polecenie[i] == 32)
//						break;
//					wyraz += polecenie[i];
//				}
//				uprawnienia.getfacl(wyraz);
//				wyraz = "";
//			}
//			else {
//				std::cout << "\n" << "'" << polecenie << "'" << "is not recognized as an internal or external command, " <<
//					"\noperable program or batch file.\n\n";
//			}
//		}
//		else if (!flaga1)
//		{
//			std::cout << "\n" << "'" << polecenie << "'" << " is not recognized as an internal or external command, " <<
//				"\noperable program or batch file.\n\n";
//		}
//		flaga1 = false;
//	}
//
//	return 0;
//}