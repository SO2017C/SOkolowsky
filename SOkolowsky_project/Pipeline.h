#ifndef Pipeline_h
#define Pipeline_h
#include "Biblioteki.h";
#include "Synchronization.h";

//extern class PCB;
class Pipe;
class PCB;
class Tree;

class Pipeline {
public:
	Pipeline();
	void createPipe(PCB &p1, PCB &p2); //funkcja tworząca pokot gdzie p1 to referencja do procesu wysyłającego a p2 do odbierającego
	void deletePipe(PCB& p1);   // funckja usuwająca potok
	bool Pipe_exist(PCB& p1, PCB &p2);
	std::vector<Pipe*> pipes;   //wektor przechowujący wszystkie aktywne potoki

};

class Pipe {
public:
	Pipe(PCB& p1, PCB& p2, Pipeline& pl);
	~Pipe();
	std::string read(unsigned int l);   //funkcja odczytu z parametrem długości wiadomości którą chcemy zapisać
	void write(const std::string& msg); //funkcja zapisu z parametrem wiadomości do przesłania
private:
	std::queue<char> buffer; //potok jako kolejka
	PCB  *p1;   //wskaźnik na proces wysyłający komunikaty
	PCB  *p2;   //wskaźnik na proces odczytujący
	Pipeline *pl;   //wskaźnik do klasy nadrzędnej
};
#endif