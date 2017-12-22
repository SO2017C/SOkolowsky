#include "Shell.h"
#include "HDD.h"

HDD test;

void zapisz() {
std::cout << "TWORZENIE PLIKU\n";
std::string name;
int size;
std::string text;
std::cout << "Enter name of file.\n";
std::getline(std::cin, name);
std::cout << "Enter text\n";
std::getline(std::cin, text);
size = text.size();

test.create_file(name, size, text);
}

void usun() {
std::cout << "USUWANIE PLIKU\n";
std::string name;

std::cout << "Enter name of file.\n";
std::getline(std::cin, name);

test.delete_file(name);
}

void czytaj() {
std::cout << "ODCZYTYWANIE PLIKU\n";
std::string name;

std::cout << "Enter name of file.\n";
std::getline(std::cin, name);

std::cout << test.read_file(name) << std::endl;
}

void zmien() {
std::cout << "ZMIENIANIE NAZWY PLIKU\n";
std::string name_1;
std::string name_2;
std::cout << "Enter name of file.\n";
std::getline(std::cin, name_1);
std::cout << "Enter new name\n";
std::getline(std::cin, name_2);

test.rename_file(name_1, name_2);

}

void formatuj() {
test.format_disk();
}

void pokaz() {
test.show_all_info();
}

//MAIN
int main() {
	SHELL shell;
}