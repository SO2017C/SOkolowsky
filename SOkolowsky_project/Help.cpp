#include "Help.h"

void help_functions::CreateFile(std::string txt, int ilosc_bitow)
{
	std::cout << "Stworzylem plik o nazwie " << txt << " o wielkosci " << ilosc_bitow << " bitow" << std::endl << std::endl;
}
void help_functions::CreateFile_inf()
{
	std::cout << "[CF] [PARAM1] [PARAM2] | gdzie, PARAM1 lub PARAM2 to nazwa pliku oraz ilosc bajtow" << std::endl << std::endl;
}
