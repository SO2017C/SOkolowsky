#pragma once
#include "Biblioteki.h"
//#include "Processes.h"
//extern class PCB;

struct PageTableData {// tablica stronic indeks->nrStronicy tworze dla kaødego procesu
	int frame; // ramka w ktorej znajduje sie stronica
	bool bit; // bit sprawdzajacy czy stronica jest w pamieci

	PageTableData(int frame, bool bit) {
		this->frame = frame;
		this->bit = bit;
	}
};

struct Page {//stronica
	char data[16]; // dane stronicy
	Page(std::string data) {
		while (data.size() < 16)
			data += " ";

		for (int i = 0; i < 16; i++) {
			this->data[i] = data[i];
		}
	}
};

struct FramelistData {// struktura znajdujaca sie w liscie ramek ulatwiajaca wymiane stronic (nie trzeba przeszukiwac listy procesow);
	bool free; // zajetosc ramki
	int PID; // PID
	int pageN; // numer_strony
	std::vector<PageTableData> *page_table; // wskaznik na tablice stronic
	FramelistData(bool free, int PID, int pageN, std::vector<PageTableData> *page_table) {
		this->free = free;
		this->PID = PID;
		this->pageN = pageN;
		this->page_table = page_table;
	};
};

class MemoryManager{
private:
	
	std::vector<FramelistData> Frames;//Ramki w pamieci fizycznej
	std::list<int> LRU{ 0,1,2,3,4,5,6,7 };//Lista LRU ostani element wskazuje na najdluzej nieuzywana ramke
	//Plik stronicowania vector par<PID,vector Stron>>>;
	//std::vector<std::pair<int, std::vector<Page>>> PageFile; // zamiana na MAPE
	//Plik stronicowania mapa<ID , Stronice tego procesu>;
	std::map<int, std::vector<Page> > PageFile;

	void FrameOrder(int Frame);//Ustawia na poczatek listy wybrana ramke
	//Laduje dana stronice w podana ramke w pamieci fizycznej
	//Arg: Strona ≥adowana do pamiecie , Ramka w ktorej ma zostac umieszczona,numer stronicy,PID, tablica stronic
	int LoadtoMemory(Page page, int pageN, int PID, std::vector<PageTableData> *page_table);
	//Zamienia stronice zgodnie z algorytmem LRU
	//zwraca numer ramki podmienionej
	int SwapPages(std::vector<PageTableData> *page_table, int pageN, int PID);
	int seekForFreeFrame();	//Szuka wolnej ramki

public:
	char RAM[128];// pamiec fizyczna 
	MemoryManager();

	// Tworze wskaünik do tablicy stronic z odpowiedniπ iloúciπ stronic 
	// frame -1 oraz bit 0 oznacza ze stronica nie znajduje sie w pamieci
	// Page_table znajduje sie w PCB
	std::vector<PageTableData> *createPageTable(int mem, int PID);
	void start();//proces beczynnosci umieszany w pamieci przy starcie systemu wypelnianie RAMu spacjami
	void showPMemory(int start, int amount);//funkcja wyswietlajaca pamiec od podanego miejsca i ilosci
	void showPMemory();//cala pamiec fizyczna
	void ShowPageFile();	//wyswietla plik wymiany
	static void showPageTable(std::vector<PageTableData> *page_table);	//wyswietlam tablice stronic
	void ShowLRUList();// pokazuje liste ostatnio uzytych ramek w kolejnosci uzycia
	void showFrameList();//pokazuje liste ramek wraz z informacja kogo dane znajduja sie w pamieci
	void Remove(int PID);//usuwa z pamieci dane wybranego procesu
	//Procedura ladujaca program`
	//Pierwsza stronica dla programu zostaje zaladowana do RAMU reszta jest w pliku wymiany
	int LoadProgram(std::string path, int mem, int PID);
	int Write(int adress, int PID, char ch, std::vector<PageTableData> *page_table);	//WRITE TO MEMORY

	//Pobiera rozkaz z danego adresu
	//std::string Get(PCB *process, int LR);

	//~MemoryManager();
}

