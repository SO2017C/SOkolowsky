#ifndef ACL_h
#define ACL_h

#include<list>
#include<string>
#include<map>
#include<iostream>
#include<vector>

struct User {
	std::string name;
	std::string password;
	std::string group;
	std::string permissions;
	std::string effective_permissions;

	User() {}
	User(std::string _name, std::string _pass) {
		this->name = _name;
		this->password = _pass;
	}
};

struct Group {
	std::string name;
	std::vector<User> users_in_group;
	std::string permissions;
	std::string effective_permissions;

	Group() {}
	Group(std::string _name) {
		this->name = _name;
	}
};

struct ACL {
	std::string owner;
	std::string group;
	std::string user_owner_permissions;
	std::string group_owner_permissions;
	std::vector<User> users;
	std::vector<Group> groups;
	std::string mask;
	std::string others;

	ACL() {}
};

class Uprawnienia {
private:
	std::map<std::string, ACL> kontener_ACL;
	std::vector<User> wszyscy_uzytkownicy;
	std::vector<Group> wszystkie_grupy;
	User zalogowany_uzytkownik;

public:
	Uprawnienia() {
		User u;
		Group g1, g2;
		g1.name = "administratorzy";
		g2.name = "uzytkownicy";
		u.name = "administrator";
		u.group = g1.name;
		g1.users_in_group.push_back(u);
		wszystkie_grupy.push_back(g1);
		wszystkie_grupy.push_back(g2);
		wszyscy_uzytkownicy.push_back(u);
		zalogowany_uzytkownik = u;
	}

	User zwroc_zalogowanego_uzytkownika() {
		return zalogowany_uzytkownik;
	}

	void dodaj_uzytkownika(std::string nazwa, std::string haslo);

	void dodaj_grupe(std::string nazwa);

	void wyswietl_uzytkownikow();

	void wyswietl_grupy();

	void przelacz_uzytkownika(std::string name, std::string haslo);

	void dodaj_do_grupy(std::string nazwa, std::string grupa);


	void stworzACL(std::string nazwa_pliku) {
		std::pair<std::string, ACL> para;
		para.first = nazwa_pliku;
		para.second.owner = zalogowany_uzytkownik.name;
		para.second.group = zalogowany_uzytkownik.group;
		para.second.user_owner_permissions = "rwx";
		para.second.group_owner_permissions = "rwx";
		para.second.others = "r--";
		para.second.mask = "rwx";
		kontener_ACL.insert(para);
	}

	void getfacl(std::string nazwa_pliku) {
		std::cout << "\n";
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			std::cout << "# file: " << nazwa_pliku << "\n";
			std::cout << "# owner: " << kontener_ACL[nazwa_pliku].owner << "\n";
			std::cout << "# group: " << kontener_ACL[nazwa_pliku].group << "\n";
			std::cout << "user::" << kontener_ACL[nazwa_pliku].user_owner_permissions << "\n";
			for (auto p : kontener_ACL[nazwa_pliku].users)
				std::cout << "user:" << p.name << ":" << "\n";
			std::cout << "group::" << kontener_ACL[nazwa_pliku].group_owner_permissions << "\n";
			for (auto p : kontener_ACL[nazwa_pliku].groups)
				std::cout << "group:" << p.name << ":" << "\n";
			std::cout << "mask::" << kontener_ACL[nazwa_pliku].mask << "\n";
			std::cout << "other::" << kontener_ACL[nazwa_pliku].others << "\n\n";
		}
		else std::cout << "\nPlik nie istnieje\n";
	}

	bool zgoda_na_odczyt(std::string nazwa_pliku) {
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (zalogowany_uzytkownik.name == kontener_ACL[nazwa_pliku].owner)
				return true;
		}
		std::cout << "You don't have a permissions!\n";
		return false;
	}

	bool zgoda_na_modyfikacje(std::string nazwa_pliku) {
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (zalogowany_uzytkownik.name == kontener_ACL[nazwa_pliku].owner)
				return true;
		}
		std::cout << "You don't have a permissions!\n";
		return false;
	}

	bool zgoda_na_usuniecie(std::string nazwa_pliku) {
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (zalogowany_uzytkownik.name == kontener_ACL[nazwa_pliku].owner)
				return true;
		}
		std::cout << "You don't have a permissions!\n";
		return false;
	}
};

#endif
