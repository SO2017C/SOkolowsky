#include "Help.h"


/// DYSK
void HELP_CLASS::CREATEFILE_H()
{
	std::cout << "Usage: cf [param1] [param2] [param3]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file." << std::endl;
	std::cout << "\tparam2 - numbers of byte dedicated for the file." << std::endl;
	std::cout << "\tparam3 - optionally text written into the file." << std::endl << std::endl;
}
void HELP_CLASS::READFILE_H()
{
	std::cout << "Usage: rf [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want to read." << std::endl << std::endl;
}

void HELP_CLASS::DELETEFILE_H()
{
	std::cout << "Usage: rf [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want to delete." << std::endl << std::endl;
}


/// ACL
void HELP_CLASS::USERADD_H()
{
	std::cout << "Usage: useradd [param1] [param2]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name for the new user." << std::endl;
	std::cout << "\tparam2 - password for the new user." << std::endl << std::endl;
}

void HELP_CLASS::USERDEL_H()
{
	std::cout << "Usage: userdel [param1] " << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the user you want to delete." << std::endl << std::endl;
}

void HELP_CLASS::GROUPADD_H()
{
	std::cout << "Usage: groupadd [param1] " << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the group you want to add." << std::endl << std::endl;
}

void HELP_CLASS::GROUPDEL_H()
{
	std::cout << "Usage: groupdel [param1] " << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the group you want to delete." << std::endl << std::endl;
}

void HELP_CLASS::SWITCHUSER_H()
{
	std::cout << "Usage: su [param1] [param2] " << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the user you want to switch on." << std::endl;
	std::cout << "\tparam2 - password, leave empty if an user doesn't have one" << std::endl << std::endl;
}

void HELP_CLASS::ADDTOGROUP_H()
{
	std::cout << "Usage: addtogroup [param1] [param2]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of user you want to move." << std::endl;
	std::cout << "\tparam2 - name of the group that you want to put an user in." << std::endl << std::endl;
}

void HELP_CLASS::GETFACL_H()
{
	std::cout << "Usage: getfacl [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file that you want to check." << std::endl << std::endl;
}




///MY
void HELP_CLASS::HELP_F()
{
	std::cout << "You've typed wrong input." << std::endl;
	std::cout << "If you need a help, just type the name of command or the name of command and \"/?\"" << std::endl;
	std::cout << "ask the developers of this Operation System." << std::endl << std::endl;
}
