#include "ACL.h"

bool Uprawnienia::uzytkownik_istnieje(std::string nazwa) {
	for (auto e : wszyscy_uzytkownicy)
		if (e.name == nazwa)
			return true;
	return false;
}

bool Uprawnienia::grupa_istnieje(std::string nazwa) {
	for (auto e : wszystkie_grupy)
		if (e.name == nazwa)
			return true;//
	return false;
}

void Uprawnienia::dodaj_uzytkownika(std::string nazwa, std::string haslo) {
	User u;

	u.name = nazwa;
	u.password = haslo;
	u.group = "uzytkownicy";
	wszyscy_uzytkownicy.push_back(u);

	for (int i = 0; i < wszystkie_grupy.size(); i++) {
		if (wszystkie_grupy[i].name == "uzytkownicy")
			wszystkie_grupy[i].users_in_group.push_back(u);
	}

}

void Uprawnienia::dodaj_grupe(std::string nazwa) {
	Group g;

	g.name = nazwa;
	wszystkie_grupy.push_back(g);


}

void Uprawnienia::usun_uzytkownika(std::string nazwa) {
	if (zalogowany_uzytkownik.group == "administratorzy") {
		if (nazwa != "administrator") {
			if (zalogowany_uzytkownik.name != nazwa) {
				std::string group_name;
				int licznik = -1;
				for (int i = 0; i < wszyscy_uzytkownicy.size(); i++) {
					if (wszyscy_uzytkownicy[i].name == nazwa) {

						group_name = wszyscy_uzytkownicy[i].group;
						wszyscy_uzytkownicy.erase(wszyscy_uzytkownicy.begin() + i);
					}
				}
				for (int i = 0; i < wszystkie_grupy.size(); i++) {
					if (wszystkie_grupy[i].name == group_name) {
						for (int j = 0; j < wszystkie_grupy[i].users_in_group.size(); j++) {
							if (wszystkie_grupy[i].users_in_group[j].name == nazwa)
								wszystkie_grupy[i].users_in_group.erase(wszystkie_grupy[i].users_in_group.begin() + j);
						}
					}
				}
				for (auto &z : kontener_ACL) {
					licznik = -1;
					for (auto &x : z.second.users) {
						licznik++;
						if (x.name == nazwa) {
							z.second.users.erase(z.second.users.begin() + licznik);
						}
					}
				}
			}
			else std::cout << "nie mozna usunac zalogowanego uzytkownika\n\n";
		}
		else std::cout << "nie mozna usunac wbudowanego konta administratora\n\n";
	}
	else std::cout << "Aby usunac uzytkownika musisz nalezec do grupy administratorzy\n\n";
}

void Uprawnienia::usun_grupe(std::string nazwa) {
	if (zalogowany_uzytkownik.group == "administratorzy") {
		if (nazwa != "administratorzy" && nazwa != "uzytkownicy") {
			User u;
			std::vector<User> pom{};
			int licznik = -1;

			for (int i = 0; i < wszyscy_uzytkownicy.size(); i++) {
				if (wszyscy_uzytkownicy[i].group == nazwa) {
					wszyscy_uzytkownicy[i].group = "uzytkownicy";
				}
			}
			for (int i = 0; i < wszystkie_grupy.size(); i++) {
				if (wszystkie_grupy[i].name == nazwa) {
					for (int j = 0; j < wszystkie_grupy[i].users_in_group.size(); j++) {
						u.name = wszystkie_grupy[i].users_in_group[j].name;
						u.password = wszystkie_grupy[i].users_in_group[j].password;
						u.group = "uzytkownicy";
						pom.push_back(u);
					}
					wszystkie_grupy.erase(wszystkie_grupy.begin() + i);
				}
			}

			for (int i = 0; i < wszystkie_grupy.size(); i++) {
				if (wszystkie_grupy[i].name == "uzytkownicy") {
					for (int j = 0; j < pom.size(); j++) {
						u.name = pom[j].name;
						u.password = pom[j].password;
						u.group = "uzytkownicy";
						wszystkie_grupy[i].users_in_group.push_back(u);
					}
				}
			}

			for (auto &z : kontener_ACL) {
				licznik = -1;
				for (auto &x : z.second.groups) {
					licznik++;
					if (x.name == nazwa) {
						z.second.groups.erase(z.second.groups.begin() + licznik);
					}
				}
			}
		}
		else std::cout << "\nNie mozna usunac wbudowanych kont administratorzy i uzytkownicy\n\n";
	}
	else std::cout << "\nAby usunac grupe musisz nalezec do grupy administratorzy\n\n";

	//przypisanie uzytkownikow z usunietej grupy do grupy uzytkownikow
}

void Uprawnienia::wyswietl_uzytkownikow() {
	std::cout << "\n";
	for (auto e : wszyscy_uzytkownicy)
		std::cout << e.name << "\n";
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
				std::cout << "\nhaslo nieprawidlowe\n\n";
			}
			flaga = true;
			break;
		}
	}
	if (!flaga)
		std::cout << "\nThe user \"" << name << "\" does not exists\n\n";
}

void Uprawnienia::dodaj_do_grupy(std::string nazwa, std::string grupa) {
	bool flaga1 = false;
	bool flaga2 = false;
	bool flaga3 = false;
	bool flaga4 = false;
	User u;
	for (auto e : wszyscy_uzytkownicy)
		if (e.name == nazwa && e.name != "administrator") {
			flaga3 = true;
			u = e;
		}

	if (zalogowany_uzytkownik.group == "administratorzy")
		flaga1 = true;

	if (u.group != grupa)
		flaga4 = true;

	for (int i = 0; i < wszystkie_grupy.size(); i++)
		if (wszystkie_grupy[i].name == grupa)
			flaga2 = true;

	if (flaga1 == true && flaga2 == true && flaga3 == true && flaga4 == true) {
		for (int j = 0; j < wszystkie_grupy.size(); j++) {
			if (wszystkie_grupy[j].name == u.group) {
				for (int k = 0; k < wszystkie_grupy[j].users_in_group.size(); k++) {
					if (wszystkie_grupy[j].users_in_group[k].name == u.name) {
						wszystkie_grupy[j].users_in_group.erase(wszystkie_grupy[j].users_in_group.begin() + k);
					}
				}
			}
		}
	}
	if (flaga1 == true && flaga2 == true && flaga3 == true && flaga4 == true) {
		u.group = grupa;
		for (int i = 0; i < wszystkie_grupy.size(); i++) {
			if (wszystkie_grupy[i].name == grupa)
				wszystkie_grupy[i].users_in_group.push_back(u);
		}
		for (auto &x : wszyscy_uzytkownicy) {
			if (x.name == nazwa)
				x.group = grupa;
		}
	}
	if (!flaga4)
		std::cout << "\nPodany uzytkownik juz nalezy to tej grupy\n\n";
	if (!flaga3)
		std::cout << "\nPodany uzytkownik nie istnieje\n\n";
	if (!flaga2)
		std::cout << "\nPodana grupa uzytkownikow nie istnieje\n\n";
	if (!flaga1)
		std::cout << "\nBrak uprawnien do wykonania okreslonej operacji.\nZaloguj sie na konto administratora\n\n";
}