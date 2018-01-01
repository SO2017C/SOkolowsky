#ifndef ACL_modul3_h
#define ACL_modul3_h

#include"Biblioteki.h"

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
	std::string group_owner_effective_permissions;
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
	bool mask_defined = false;

public:
	Uprawnienia() {
		User u;
		Group g1, g2;
		g1.name = "administratorzy";
		g2.name = "uzytkownicy";
		u.name = "administrator";
		u.password = "admin";
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

	bool uzytkownik_istnieje(std::string nazwa);

	bool grupa_istnieje(std::string nazwa);

	void dodaj_uzytkownika(std::string nazwa, std::string haslo);

	void dodaj_grupe(std::string nazwa);

	void usun_uzytkownika(std::string nazwa);

	void usun_grupe(std::string nazwa);

	void wyswietl_uzytkownikow();

	void wyswietl_grupy();

	void przelacz_uzytkownika(std::string name, std::string haslo);

	void dodaj_do_grupy(std::string nazwa, std::string grupa);

	std::string numbers_to_rwx(char right) {
		std::string permissions;
		switch (right) {
		case '0':
			permissions = "---";
			return permissions;
		case '1':
			permissions = "--x";
			return permissions;
		case '2':
			permissions = "-w-";
			return permissions;
		case '3':
			permissions = "-wx";
			return permissions;
		case '4':
			permissions = "r--";
			return permissions;
		case '5':
			permissions = "r-x";
			return permissions;
		case '6':
			permissions = "rw-";
			return permissions;
		case '7':
			permissions = "rwx";
			return permissions;
		}
	}

	void stworzACL(std::string nazwa_pliku) {
		std::pair<std::string, ACL> para;
		para.first = nazwa_pliku;
		para.second.owner = zalogowany_uzytkownik.name;
		para.second.group = zalogowany_uzytkownik.group;
		para.second.user_owner_permissions = "rwx";
		para.second.group_owner_permissions = "rwx";
		para.second.mask = "rwx";
		para.second.others = "r--";
		kontener_ACL.insert(para);
	}

	void getfacl(std::string nazwa_pliku) {
		std::cout << "\n";
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (kontener_ACL[nazwa_pliku].mask == "rwx") {
				std::cout << "# file: " << nazwa_pliku << "\n";
				std::cout << "# owner: " << kontener_ACL[nazwa_pliku].owner << "\n";
				std::cout << "# group: " << kontener_ACL[nazwa_pliku].group << "\n";
				std::cout << "user::" << kontener_ACL[nazwa_pliku].user_owner_permissions << "\n";
				for (auto p : kontener_ACL[nazwa_pliku].users)
					std::cout << "user:" << p.name << ":" << p.permissions << "\n";
				std::cout << "group::" << kontener_ACL[nazwa_pliku].group_owner_permissions << "\n";
				for (auto p : kontener_ACL[nazwa_pliku].groups)
					std::cout << "group:" << p.name << ":" << p.permissions << "\n";
				std::cout << "mask::" << kontener_ACL[nazwa_pliku].mask << "\n";
				std::cout << "other::" << kontener_ACL[nazwa_pliku].others << "\n\n";
			}
			else {
				std::cout << "# file: " << nazwa_pliku << "\n";
				std::cout << "# owner: " << kontener_ACL[nazwa_pliku].owner << "\n";
				std::cout << "# group: " << kontener_ACL[nazwa_pliku].group << "\n";
				std::cout << "user::" << kontener_ACL[nazwa_pliku].user_owner_permissions << "\n";
				for (auto p : kontener_ACL[nazwa_pliku].users)
					std::cout << "user:" << p.name << ":" << p.permissions << "\t\t\t#effective:" << p.effective_permissions << "\n";
				std::cout << "group::" << kontener_ACL[nazwa_pliku].group_owner_permissions << "\t\t\t#effective:" << kontener_ACL[nazwa_pliku].group_owner_effective_permissions << "\n";
				for (auto p : kontener_ACL[nazwa_pliku].groups)
					std::cout << "group:" << p.name << ":" << p.permissions << "\t\t\t#effective:" << p.effective_permissions << "\n";
				std::cout << "mask::" << kontener_ACL[nazwa_pliku].mask << "\n";
				std::cout << "other::" << kontener_ACL[nazwa_pliku].others << "\n\n";
			}
		}
		else std::cout << "Plik nie istnieje\n\n";
	}

	bool jest_user_na_ACL(std::string name, std::string file_name) {
		for (auto z : kontener_ACL) {
			if (z.first == file_name) {
				for (auto x : z.second.users)
					if (x.name == name)
						return true;
			}
		}
		return false;
	}

	bool jest_grupa_na_ACL(std::string name, std::string file_name) {
		for (auto z : kontener_ACL) {
			if (z.first == file_name) {
				for (auto x : z.second.groups)
					if (x.name == name)
						return true;
			}
		}
		return false;
	}

	void usun_uzytkownika_z_ACL(std::string name, std::string file_name) {
		for (int i = 0; i < kontener_ACL[file_name].users.size(); i++) {
			if (kontener_ACL[file_name].users[i].name == name)
				kontener_ACL[file_name].users.erase(kontener_ACL[file_name].users.begin() + i);
		}
	}

	void usun_grupe_z_ACL(std::string name, std::string file_name) {
		for (int i = 0; i < kontener_ACL[file_name].groups.size(); i++) {
			if (kontener_ACL[file_name].groups[i].name == name)
				kontener_ACL[file_name].groups.erase(kontener_ACL[file_name].groups.begin() + i);
		}
	}

	std::string from_normal_to_effective(std::string mask, std::string permissions) {
		std::string effective_permissions = "";
		for (int i = 0; i < 3; i++) {
			if (permissions[i] != mask[i])
				effective_permissions += '-';
			else
				effective_permissions += mask[i];
		}
		return effective_permissions;
	}

	void set_effective_permissions(std::string permissions, std::string file_name) {
		std::string new_permissions = "";
		std::string mask = permissions;
		for (auto &x : kontener_ACL[file_name].users) {
			new_permissions = from_normal_to_effective(mask, x.permissions);
			x.effective_permissions = new_permissions;
		}
		new_permissions = from_normal_to_effective(mask, kontener_ACL[file_name].group_owner_permissions);
		kontener_ACL[file_name].group_owner_effective_permissions = new_permissions;

		for (auto &y : kontener_ACL[file_name].groups) {
			new_permissions = from_normal_to_effective(mask, y.permissions);
			y.effective_permissions = new_permissions;
		}
	}

	void setfacl(char parameter, std::string command, char right, std::string file_name) {
		char mark = command[0];
		User u; Group g;
		std::string name = "", permissions = "";
		if (parameter == 'm') {
			switch (mark) {
			case 'u':
				name = command.substr(2, command.size() - 2);
				if (uzytkownik_istnieje(name)) {
					permissions = numbers_to_rwx(right);
					if (!mask_defined) {
						if (!jest_user_na_ACL(name, file_name)) {
							u.name = name;
							u.permissions = permissions;
							kontener_ACL[file_name].users.push_back(u);
						}
						else {
							for (int i = 0; i < kontener_ACL[file_name].users.size(); i++) {
								if (kontener_ACL[file_name].users[i].name == name) {
									kontener_ACL[file_name].users[i].permissions = permissions;
								}
							}
						}
					}
					else {
						if (!jest_user_na_ACL(name, file_name)) {
							u.name = name;
							u.permissions = permissions;
							u.effective_permissions = from_normal_to_effective(kontener_ACL[file_name].mask, permissions);
							kontener_ACL[file_name].users.push_back(u);
						}
						else {
							for (int i = 0; i < kontener_ACL[file_name].users.size(); i++) {
								if (kontener_ACL[file_name].users[i].name == name) {
									kontener_ACL[file_name].users[i].permissions = permissions;
									kontener_ACL[file_name].users[i].effective_permissions = from_normal_to_effective(kontener_ACL[file_name].mask, permissions);
								}
							}
						}
					}
				}
				else {
					std::cout << "\nPodany uzytkownik nie istnieje\n\n";
				}
				break;
			case 'g':
				name = command.substr(2, command.size() - 2);
				if (grupa_istnieje(name)) {
					permissions = numbers_to_rwx(right);
					if (!jest_grupa_na_ACL(name, file_name)) {
						g.name = name;
						g.permissions = permissions;
						kontener_ACL[file_name].groups.push_back(g);
					}
					else {
						for (int i = 0; i < kontener_ACL[file_name].groups.size(); i++) {
							if (kontener_ACL[file_name].groups[i].name == name)
								kontener_ACL[file_name].groups[i].permissions = permissions;
						}
					}
				}
				else {
					std::cout << "\nPodana grupa nie istnieje\n\n";
				}
				break;
			case 'm':
				if (right < 55) {
					permissions = numbers_to_rwx(right);
					set_effective_permissions(permissions, file_name);
					kontener_ACL[file_name].mask = permissions;
					mask_defined = true;
				}
				else {
					permissions = numbers_to_rwx(right);
					kontener_ACL[file_name].mask = permissions;
				}
				break;
			case 'o':
				permissions = numbers_to_rwx(right);
				kontener_ACL[file_name].others = permissions;
				break;
			}
		}
		else if (parameter == 'x') {
			switch (mark) {
			case 'u':
				name = command.substr(2, command.size() - 2);
				if (uzytkownik_istnieje(name))
					if (jest_user_na_ACL(name, file_name))
						usun_uzytkownika_z_ACL(name, file_name);
				break;
			case 'g':
				name = command.substr(2, command.size() - 2);
				if (grupa_istnieje(name))
					if (jest_grupa_na_ACL(name, file_name))
						usun_grupe_z_ACL(name, file_name);
				break;
			}
		}
	}

	void deleteACL(std::string file_name) {
		if (kontener_ACL.find(file_name) != kontener_ACL.end()) {
			std::map<std::string, ACL>::iterator it;
			it = kontener_ACL.find(file_name);
			kontener_ACL.erase(it);
		}
		else std::cout << "Plik nie istnieje\n\n";

	}

	bool zgoda_na_odczyt(std::string nazwa_pliku) {
		bool others1 = false, others2 = false, others3 = false;
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (zalogowany_uzytkownik.name == kontener_ACL[nazwa_pliku].owner)
				return true;

			if (mask_defined) {
				for (auto x : kontener_ACL[nazwa_pliku].users) {
					if (x.name == zalogowany_uzytkownik.name) {
						if (x.effective_permissions[0] == 'r') {
							return true;
						}
					}
					else
						others1 = true;
				}
				if (zalogowany_uzytkownik.group == kontener_ACL[nazwa_pliku].group) {
					if (kontener_ACL[nazwa_pliku].group_owner_effective_permissions[0] == 'r')
						return true;
					else
						return false;
				}
				else others2 = true;

				for (auto x : kontener_ACL[nazwa_pliku].groups) {
					if (x.name == zalogowany_uzytkownik.group) {
						if (x.effective_permissions[0] == 'r')
							return true;
						else
							return false;
					}
					else others3 = true;
				}

				if (others1 == true && others2 == true && others3 == true) {
					if (kontener_ACL[nazwa_pliku].others[0] == 'r')
						return true;
					else
						return false;
				}
			}
			else {
				for (auto x : kontener_ACL[nazwa_pliku].users) {
					if (x.name == zalogowany_uzytkownik.name) {
						if (x.permissions[0] == 'r') {
							return true;
						}
					}
					else
						others1 = true;
				}
				if (zalogowany_uzytkownik.group == kontener_ACL[nazwa_pliku].group) {
					if (kontener_ACL[nazwa_pliku].group_owner_permissions[0] == 'r')
						return true;
					else
						return false;
				}
				else others2 = true;

				for (auto x : kontener_ACL[nazwa_pliku].groups) {
					if (x.name == zalogowany_uzytkownik.group) {
						if (x.permissions[0] == 'r')
							return true;
						else
							return false;
					}
					else others3 = true;
				}

				if (others1 == true && others2 == true && others3 == true) {
					if (kontener_ACL[nazwa_pliku].others[0] == 'r')
						return true;
					else
						return false;
				}
			}
		}
	}

	bool zgoda_na_modyfikacje(std::string nazwa_pliku) {
		bool others1 = false, others2 = false, others3 = false;
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (zalogowany_uzytkownik.name == kontener_ACL[nazwa_pliku].owner)
				return true;

			if (mask_defined) {
				for (auto x : kontener_ACL[nazwa_pliku].users) {
					if (x.name == zalogowany_uzytkownik.name) {
						if (x.effective_permissions[1] == 'w') {
							return true;
						}
					}
					else
						others1 = true;
				}
				if (zalogowany_uzytkownik.group == kontener_ACL[nazwa_pliku].group) {
					if (kontener_ACL[nazwa_pliku].group_owner_effective_permissions[1] == 'w')
						return true;
					else
						return false;
				}
				else others2 = true;

				for (auto x : kontener_ACL[nazwa_pliku].groups) {
					if (x.name == zalogowany_uzytkownik.group) {
						if (x.effective_permissions[1] == 'w')
							return true;
						else
							return false;
					}
					else others3 = true;
				}

				if (others1 == true && others2 == true && others3 == true) {
					if (kontener_ACL[nazwa_pliku].others[1] == 'w')
						return true;
					else
						return false;
				}
			}
			else {
				for (auto x : kontener_ACL[nazwa_pliku].users) {
					if (x.name == zalogowany_uzytkownik.name) {
						if (x.permissions[1] == 'w') {
							return true;
						}
					}
					else
						others1 = true;
				}
				if (zalogowany_uzytkownik.group == kontener_ACL[nazwa_pliku].group) {
					if (kontener_ACL[nazwa_pliku].group_owner_permissions[1] == 'w')
						return true;
					else
						return false;
				}
				else others2 = true;

				for (auto x : kontener_ACL[nazwa_pliku].groups) {
					if (x.name == zalogowany_uzytkownik.group) {
						if (x.permissions[1] == 'w')
							return true;
						else
							return false;
					}
					else others3 = true;
				}

				if (others1 == true && others2 == true && others3 == true) {
					if (kontener_ACL[nazwa_pliku].others[1] == 'w')
						return true;
					else
						return false;
				}
			}
		}
	}

	bool zgoda_na_usuniecie(std::string nazwa_pliku) {
		bool others1 = false, others2 = false, others3 = false;
		if (kontener_ACL.find(nazwa_pliku) != kontener_ACL.end()) {
			if (zalogowany_uzytkownik.name == kontener_ACL[nazwa_pliku].owner)
				return true;

			if (mask_defined) {
				for (auto x : kontener_ACL[nazwa_pliku].users) {
					if (x.name == zalogowany_uzytkownik.name) {
						if (x.effective_permissions[2] == 'x') {
							return true;
						}
					}
					else
						others1 = true;
				}
				if (zalogowany_uzytkownik.group == kontener_ACL[nazwa_pliku].group) {
					if (kontener_ACL[nazwa_pliku].group_owner_effective_permissions[2] == 'x')
						return true;
					else
						return false;
				}
				else others2 = true;

				for (auto x : kontener_ACL[nazwa_pliku].groups) {
					if (x.name == zalogowany_uzytkownik.group) {
						if (x.effective_permissions[2] == 'x')
							return true;
						else
							return false;
					}
					else others3 = true;
				}

				if (others1 == true && others2 == true && others3 == true) {
					if (kontener_ACL[nazwa_pliku].others[2] == 'x')
						return true;
					else
						return false;
				}
			}
			else {
				for (auto x : kontener_ACL[nazwa_pliku].users) {
					if (x.name == zalogowany_uzytkownik.name) {
						if (x.permissions[2] == 'x') {
							return true;
						}
					}
					else
						others1 = true;
				}
				if (zalogowany_uzytkownik.group == kontener_ACL[nazwa_pliku].group) {
					if (kontener_ACL[nazwa_pliku].group_owner_permissions[2] == 'x')
						return true;
					else
						return false;
				}
				else others2 = true;

				for (auto x : kontener_ACL[nazwa_pliku].groups) {
					if (x.name == zalogowany_uzytkownik.group) {
						if (x.permissions[2] == 'x')
							return true;
						else
							return false;
					}
					else others3 = true;
				}

				if (others1 == true && others2 == true && others3 == true) {
					if (kontener_ACL[nazwa_pliku].others[2] == 'x')
						return true;
					else
						return false;
				}
			}
		}
	}
};

#endif