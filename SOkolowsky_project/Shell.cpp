﻿#include "Shell.h"


/* FORMATOWANIE DANYCH */
SHELL::spis_funkcji SHELL::str_to_int(const std::string & Funkcja)
{
	///HDD
	if (Funkcja == "CF") return CREATEFILE;
	else if (Funkcja == "RF") return READFILE;
	else if (Funkcja == "WF") return WRITEFILE;
	else if (Funkcja == "DF") return DELETEFILE;
	else if (Funkcja == "RN") return RENAMEFILE;
	else if (Funkcja == "FD") return FORMATDISK;
	else if (Funkcja == "DD") return DISPLAYDISK;
	///ACL
	else if (Funkcja == "USERADD") return USERADD;
	else if (Funkcja == "USERDEL") return USERDEL;
	else if (Funkcja == "GROUPADD") return GROUPADD;
	else if (Funkcja == "GROUPDEL") return GROUPDEL;
	else if (Funkcja == "SU") return SWITCHUSER;
	else if (Funkcja == "DU") return DISPLAYUSERS;
	else if (Funkcja == "DG") return DISPLAYGROUPS;
	else if (Funkcja == "USERMOD") return USERMOD;
	else if (Funkcja == "GETFACL") return GETFACL;
	else if (Funkcja == "SETFACL") return SETFACL;
	else if (Funkcja == "DA") return DISPLAYACLS;
	/// MY
	else if (Funkcja == "HELP") return HELP;
	else if (Funkcja == "CREDITS")return CREDITS;
	else if (Funkcja == "EXIT") return EXIT;
	else return OTHER;
}
bool SHELL::are_there_letters(const std::string &s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] < 65 || s[i] >= 91 && s[i] <= 96 || s[i]>123) // Odsylam do tabeli ASCII
			return false;
	}
	return true;
}
bool SHELL::are_there_numbers(const std::string &s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] < 48 || s[i]>57)// Odsylam do tabeli ASCII
			return false;
	}
	return true;
}
bool SHELL::is_there_number(const char & c)
{
	if (c < 48 || c>57)
		return false;
	else
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
//  std::string login, haslo;
//  while (true)
//  {
//      std::cout << "Podaj login: "; std::cin >> login;
//      std::cout << "Podaj haslo: "; std::cin >> haslo;
//      uprawnienia.przelacz_uzytkownika(login, haslo);
// 
//  }
//}
void SHELL::command()
{

	std::string zdanie;
	std::getline(std::cin, zdanie);                                 // pobranie linijki wypisanej przez uzytkownika
	std::smatch sm;
	std::regex regex("[a-zA-Z0-9_!@#$%\"*^&()\\[\\]+-={}';.,/?]+|[1-9][0-9]*");   // minimum 1 literka, string zaczyna sie od obojetnie czego
																				// liczba zaczyna sie od 1: czyli nie mo¿na zrobiæ 001 - wlasciwie to uzytkownik moze to wpisac, ale my zobaczymy tylko 1...
																				// no i nasz parametr specjalny: /? (wywolanie pomocy)

	while (std::regex_search(zdanie, sm, regex))                    // za pomoca wyrazenia regularnego wyfiltrowanie rzeczy, ktore wypisal uzytkownik
	{
		command_line.push_back(sm.str());
		zdanie = sm.suffix();
	}
	if (command_line.size() < 1)                                    // jesli ktos podal zdanie zawierajacy tylko: " \ / | % ! - = * +, to wtedy size jest rowny 0, gdyz regex go nie wylapie
	{
		std::cout << permissions.return_log_in_user_name().name << "::> ";
		command_line.clear();
	}
	else if (are_there_letters(command_line[0]))                    // jesli w pierwszym wyrazie sa literki
	{
		letters_to_upper(command_line[0]);                          // zmieniamy je na duze litery: cF -> CF / cf -> CF
		if (command_line.size() == 3)                               // rowniez jesli wielkosc komendy jest rowna 3
			segregate();                                            // segregujemy, tak ze int zawsze jest na 3 pozycji --- wytlumaczenie: ping -n 10 8.8.8.8 == ping 8.8.8.8 -n 10 |
	}                                                               // w CMD kolejnosc nie ma znaczenia, rowniez chcialem aby moj shell byl podobny
	else                                                            // jezeli ktos wpisze np. tylko: ";)" "$$" to wtedy command_line.size() == 0, czyli wywolujemy ze ma podac w 1. parametrze funkcje
	{
		error_r();
		std::cout << permissions.return_log_in_user_name().name << "::> ";
		command_line.clear();
	}
}
void SHELL::run()
{
	do {
		std::cout << permissions.return_log_in_user_name().name << "::> ";                                   // wypisanie naszego "znaku poczatku komendy" - czy jak to nazwac
		command_line.clear();                                    // wyczyszczenie command_line z poprzedniej komendy (tej ktora zostala wykonana)
		while (command_line.size() <= 0)                         // jezeli ktos podal np. "||" jako nazwe funkcji, to petla sie powtarza lub jesli sa jakies nieprawidlowe parametry
		{
			command();                                           // wywolanie funkcji pobierajacej komende od uzytkownika
		}
		switch_case();                                           // w przypadku komendy, ktora wydaje sie w miare "poprawna" wywolujemy switch_case()

	} while (running);                                           // program bedzie sie wykonywal w nieskonczonosc dopoki uzytkownik go nie przerwie
}
void SHELL::switch_case()
{
	switch (str_to_int(command_line[0]))
	{

		/// DYSK
	case CREATEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))				// help							
		{
			help_class.CREATEFILE_H();
		}
		else if (command_line.size() == 3 && are_there_numbers(command_line[2]))													// stworz plik bez tekstu
		{
			DISK.create_file(command_line[1], std::stoi(command_line[2]));
			permissions.createACL(command_line[1]);//TU IF CZY SIE UDALO STWORZYC!
												   //uprawnienia.getfacl(command_line[1]);
		}
		else if (command_line.size() >= 4 && are_there_numbers(command_line[2]))													// stworz plik z tekstem
		{
			std::string tekst;
			for (int i = 3; i < command_line.size(); i++)
			{
				if (i == command_line.size() - 1)
					tekst += command_line[i];
				else
					tekst += command_line[i] += ' ';
			}
			if (std::stoi(command_line[2]) >= tekst.size())
			{

				DISK.create_file(command_line[1], std::stoi(command_line[2]));
				DISK.write_file(command_line[1], tekst, 0);
				permissions.createACL(command_line[1]);//TU IF CZY SIE UDALO STWORZYC!
													   //uprawnienia.getfacl(command_line[1]);
			}
			else
			{
				std::cout << "File couldn't be created." << std::endl;
				std::cout << "The amount of characters you typed," << std::endl;
				std::cout << "exceeded the amount that disk dedicated for that file" << std::endl;
			}
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case READFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.READFILE_H();
		}
		else if (command_line.size() == 2)
		{
			if (permissions.read_permission(command_line[1]) == true) {
				std::cout << DISK.read_file(command_line[1]) << std::endl;
			}
			else
			{
				std::cout << "User \"" << permissions.return_log_in_user_name().name << "\" does not have permissions to read that file" << std::endl;
			}

		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case WRITEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))				// help							
		{
			help_class.WRITEFILE_H();
		}
		else if (command_line.size() >= 3)													               // stworz plik z tekstem
		{ // WF test troll
			std::string tekst;
			// if ma uprawnienia
			for (int i = 2; i < command_line.size(); i++)
			{
				if (i == command_line.size() - 1)
					tekst += command_line[i];
				else
					tekst += command_line[i] += ' ';
			}

			if (is_there_number(tekst.at(tekst.size() - 1))) // jesli jest wskaznik
			{
				if (tekst.at(0) == '"'&&tekst.at(tekst.size() - 2))
				{
					char liczba = tekst[tekst.size() - 1];
					std::string wpisz(tekst.begin() + 1, tekst.end() - 3);
					DISK.write_file(command_line[1], wpisz, liczba-48);
				}
			}
			else // bez wskaznika
			{
				std::string wpisz(tekst.begin() + 1, tekst.end() - 1);
				DISK.write_file(command_line[1], wpisz, 0);
			}

			//else
			//{
			//	std::cout << "File couldn't be created." << std::endl;
			//	std::cout << "The amount of characters you typed," << std::endl;
			//	std::cout << "exceeded the amount that disk dedicated for that file" << std::endl;
			//}
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case DELETEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.DELETEFILE_H();
		}
		else if (command_line.size() == 2)
		{
			if (permissions.exec_permission(command_line[1]) == true) {
				DISK.delete_file(command_line[1]);
				permissions.deleteACL(command_line[1]);
			}
			else
			{
				std::cout << "aa" << std::endl;
			}
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case RENAMEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.RENAMEFILE_H();
		}
		else if (command_line.size()==3)
		{
			DISK.rename_file(command_line[1], command_line[2]);
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case FORMATDISK:
	{
		if (command_line.size() == 1)
		{
			DISK.format_disk();
			permissions.delete_all_ACLs();
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case DISPLAYDISK:
	{
		if (command_line.size() == 1)
		{
			DISK.show_all_info();
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	/// ACL
	case USERADD:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.USERADD_H();
		}
		else if (command_line.size() == 3)
		{
			if (permissions.user_exists(command_line[1]) == false)
			{
				permissions.add_user(command_line[1], command_line[2]);
				std::cout << "The user \"" << command_line[1] << "\" has been successfully created." << std::endl << std::endl;
			}
			else
			{
				std::cout << "User with name \"" << command_line[1] << "\" already exists." << std::endl << std::endl;
			}

		}
		else if (command_line.size() == 2)
		{
			if (permissions.user_exists(command_line[1]) == false)
			{
				permissions.add_user(command_line[1], "");
				std::cout << "The user \"" << command_line[1] << "\" has been successfully added." << std::endl << std::endl;
			}
			else
			{
				std::cout << "User with name \"" << command_line[1] << "\" already exists." << std::endl << std::endl;
			}
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case USERDEL:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.USERDEL_H();
		}
		else if (command_line.size() == 2)
		{
			if (permissions.user_exists(command_line[1]))
			{
				permissions.delete_user(command_line[1]);
			}
			else
			{
				std::cout << "User \"" << command_line[1] << "\" was not found" << std::endl << std::endl;
			}
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case DISPLAYUSERS:
	{
		permissions.display_users();
		break;
	}
	case GROUPADD:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.GROUPADD_H();
		}
		if (command_line.size() == 2)
		{
			if (permissions.group_exists(command_line[1]) == false)
			{
				permissions.add_group(command_line[1]);
				std::cout << "The group \"" << command_line[1] << "\" has been successfully created." << std::endl << std::endl;
			}
			else
			{
				std::cout << "Group with name \"" << command_line[1] << "\" already exists." << std::endl << std::endl;
			}

		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case GROUPDEL:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.GROUPDEL_H();
		}
		if (command_line.size() == 2)
		{
			if (permissions.group_exists(command_line[1]))
			{
				permissions.delete_group(command_line[1]);
			}
			else
			{
				std::cout << "Group \"" << command_line[1] << "\" was not found" << std::endl << std::endl;
			}
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case DISPLAYGROUPS:
	{
		permissions.display_groups();
		break;
	}
	case SWITCHUSER:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.SWITCHUSER_H();
		}
		else if (command_line.size() == 3)
		{
			permissions.log_out_and_in(command_line[1], command_line[2]);
			system("cls");
		}
		else if (command_line.size() == 2)
		{
			permissions.log_out_and_in(command_line[1], "");
			system("cls");
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case USERMOD:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.USERMOD_H();
		}
		else if (command_line.size() == 3)
		{
			permissions.add_to_group(command_line[1], command_line[2]);
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case GETFACL:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.GETFACL_H();
		}
		else if (command_line.size() == 2)
		{
			permissions.getfacl(command_line[1]);
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case SETFACL: // porazka totalna ||| setfacl[0] -m[1] u:user:7[2] / m:2 test[3]
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.SETFACL_H();
		}
		else if (command_line.size() == 4 &&
			(command_line[1] == "-m" || command_line[1] == "-x") &&
			(command_line[2].at(0) == 'u' || command_line[2].at(0) == 'g')
			)
		{
			int licznik = 0;
			// todo: bez sensu, zmien
			for (int i = 0; i < command_line[2].size(); i++)
			{
				if (command_line[2].at(i) == ':') licznik++;
			}

			if (licznik == 2)
			{
				int poz_dwu = command_line[2].size() - 2; // u:jan:7 command_line.size()-2 -> ':'
				if (command_line[2].at(poz_dwu) == ':')
				{

					if (command_line[2].at(command_line[2].size() - 1) == '0' ||
						command_line[2].at(command_line[2].size() - 1) == '1' ||
						command_line[2].at(command_line[2].size() - 1) == '2' ||
						command_line[2].at(command_line[2].size() - 1) == '3' ||
						command_line[2].at(command_line[2].size() - 1) == '4' ||
						command_line[2].at(command_line[2].size() - 1) == '5' ||
						command_line[2].at(command_line[2].size() - 1) == '6' ||
						command_line[2].at(command_line[2].size() - 1) == '7'
						)

					{
						char right = command_line[2].at(command_line[2].size() - 1);
						command_line[2].resize(command_line[2].size() - 2);
						permissions.setfacl(command_line[1].at(1), command_line[2], right, command_line[3]);
					}
					else
					{
						help_class.HELP_F();
					}
				}
				else
				{
					help_class.HELP_F();
				}
			}
		}
		else if (command_line.size() == 4 &&
			(command_line[1] == "-m" || command_line[1] == "-x") &&
			(command_line[2].at(0) == 'm' || command_line[2].at(0) == 'o')
			)
		{// pora?ka totalna ||| setfacl[0] -m[1] u:user:7[2] / m:2 test[3]
			if (command_line[2].at(command_line[2].size() - 2) == ':')
			{
				if (command_line[2].at(command_line[2].size() - 1) == '0' ||
					command_line[2].at(command_line[2].size() - 1) == '1' ||
					command_line[2].at(command_line[2].size() - 1) == '2' ||
					command_line[2].at(command_line[2].size() - 1) == '3' ||
					command_line[2].at(command_line[2].size() - 1) == '4' ||
					command_line[2].at(command_line[2].size() - 1) == '5' ||
					command_line[2].at(command_line[2].size() - 1) == '6' ||
					command_line[2].at(command_line[2].size() - 1) == '7'
					)

				{
					char right = command_line[2].at(command_line[2].size() - 1);
					command_line[2].resize(command_line[2].size() - 2);
					permissions.setfacl(command_line[1].at(1), command_line[2], right, command_line[3]);
				}
				else
				{
					help_class.HELP_F();
				}
			}
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case DISPLAYACLS:
	{
		if (command_line.size() == 1)
		{
			permissions.display_all_ACLs();
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	/// MY
	case HELP:
	{
		help();
		break;
	}
	case EXIT:
	{
		exit();
		break;
	}
	case CREDITS:
	{
		credits();
		break;
	}
	case OTHER:
	{                                                                                                               // interpreter.COSTAM(command_line);
		error_r();                                                                                                  // jezeli nie, to wywoluje metode error_r();
		break;
	}

	}
}

/* METODY POMOCNICZE */
void SHELL::error_r()
{
	std::cout << "'" << command_line[0] << "' is not recognized as an internal or external command," << std::endl;
	std::cout << "operable program or batch file." << std::endl;
}
void SHELL::help()
{
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	std::cout << "Available commands: " << std::endl;
	std::cout << "CF - creating a new file" << std::endl;
	std::cout << "RF - reading the content from an existing file" << std::endl;
	std::cout << "WF - writing a content into an existing file" << std::endl;
	std::cout << "DF - deleting an existing file" << std::endl;
	std::cout << "RN - changing the name of an existing file" << std::endl;
	std::cout << "FD - formatting the whole disk" << std::endl;
	std::cout << "DD - displaying the informations of disk" << std::endl;
	std::cout << "USERADD - creating a new user" << std::endl;
	std::cout << "USERDEL - deleting an existing user" << std::endl;
	std::cout << "GROUPADD - creating a new empty group of users" << std::endl;
	std::cout << "GROUPDEL - deleting an existing group of users" << std::endl;
	std::cout << "DU - displaying all known users" << std::endl;
	std::cout << "DG - displaying all known groups of users" << std::endl;
	std::cout << "SU - switching to another available user" << std::endl;
	std::cout << "DA - displaying acls" << std::endl;
	std::cout << "USERMOD - adding an existing user to an existing group" << std::endl;
	std::cout << "GETFACL - getting the file access control list for an existing file" << std::endl;
	std::cout << "SETFACL - changing or deleting the file access control list for an existing file" << std::endl;
	std::cout << "CREDITS - displaying creators of the operation system" << std::endl;
	std::cout << "EXIT - exiting from the operation system" << std::endl << std::endl << std::endl;
	std::cout << "For further informations type: \"FUNCTION /?\"" << std::endl;
	std::cout << "Example: CF /?" << std::endl;
	std::cout << std::endl << std::endl << std::endl << std::endl;
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

	std::cout << "SOkolowsky [Version 0.5.530]" << std::endl;
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
	std::cout << "Adrian Staskiewicz \t Zarzadzanie plikami i katalogami metoda indeksowa" << std::endl;
	std::cout << "Maksymilian Slowinski \t Komunikacja miedzy procesorami" << std::endl << std::endl << std::endl;
}

/* WYJSCIE Z PROGRAMU */
void SHELL::exit()
{
	std::string pom;

	std::cout << "Czy na pewno chcesz opuscic? [Yes/No]: "; std::getline(std::cin, pom);                    // pobranie linijki
	if (are_there_letters(pom))                                                                             // jesli podal literki, to zmieniamy je na wielkie litery
		letters_to_upper(pom);

	if (pom[0] == 'Y' || pom == "YE" || pom == "YES")                                                       // jesli uzytkownik zdecyduje sie wyjsc, wyswietlamy ponowne zapytanie
	{                                                                                                       // w windowsie zauwazylem, ze niewazne czy napiszemy "Y" / "YE" / "YES" to zawsze
		are_you_sure();                                                                                     // dane zapytanie zakonczy sie pozytywnie
	}
	else if (pom[0] == 'N' || pom == "NO")
	{
		std::cout << "Cieszymy sie, ze jeszcze zostaniesz z nami przez chwile :)" << std::endl << std::endl;
	}
	else
	{
		exit();                                                                                             // jezeli uzytkownik wpisze cokolwiek innego, np. $ / B  itp, to ponawiamy zapytanie
	}
}
void SHELL::are_you_sure()
{
	std::string pom;
	std::cout << "Czy jestes 100% pewny, ze chcesz opuscic? [Yes/No]: "; std::getline(std::cin, pom);
	if (are_there_letters(pom))
		letters_to_upper(pom);

	if (pom[0] == 'Y' || pom == "YE" || pom == "YES")                                           // jesli bedzie na 100% pewny, to running = false - czyli wylaczamy system  
	{  
		system("cls");
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

		std::cout << "\t\t\t\t\t\t  Press enter to exit";
		std::cin.ignore(1);																						/*
																								usuwanie zmiennych dynamicznych - aby zapobiec wyciekom pamieci
																								*/
		running = false;                                                                        // gdyz po tej komendzie program sie wylaczy
	}
	else if (pom[0] == 'N' || pom == "NO")
	{
		std::cout << "uff bylo blisko :)" << std::endl << std::endl;
	}
	else
	{
		are_you_sure();                                                                         // jezeli uzytkownik wpisze cokolwiek innego, np. $ / B  itp, to ponawiamy zapytanie
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