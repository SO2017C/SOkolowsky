#include "Pipeline.h"

//znaczenia desktryporów:
// -1 - proces nie chce dzia³aæ ju¿ na tym potoku
// 1 - proces ma prawo zapisywaæ
// 0 - proces ma prawo odczytywaæ
//sam dysktyprot jest umieszczony w PCB i przechowuje inkeds potoku w tablicy potoków oraz prawid³owy deskryptor

Pipe::Pipe(PCB & p1, PCB& p2, Pipeline& pl)
{
	this->p1 = &p1;
	this->p2 = &p2;
	this->pl = &pl;
}

Pipe::~Pipe()
{
	p1->deskryptor[0] = -1;
	p2->deskryptor[0] = -1;
}


//bool test_and_set(bool*lock);	//tymczasowa funkcja synchronizacji procesu
//bool lock = false;


std::string Pipe::read(unsigned int l)
{
	//while (test_and_set(&lock))	//synchronizacaj procesów
	//{
		std::string temp;
		if (p2->deskryptor[1] != 0)	//sprawdzanie czy dany proces ma prawa do tej funkcji
		{
			std::cout << "Nie mo¿esz tego zrobiæ" << std::endl;
		}
		else if (p2->deskryptor[1] == 0)
		{
			if (l > buffer.size())	//sprawdznie czy wiadomoœc która chcemy odczytaæ jest d³u¿sza ni¿ wiadomoœc w potoku
			{
				for (int i = 0; i < buffer.size(); i++)
				{
					temp.push_back(buffer.front());
					buffer.pop();
				}
				pl->deletePipe((*p1));
			}
			else
			{
				for (int i = 0; i < l; i++)
				{
					temp.push_back(buffer.front());
					buffer.pop();
				}
			}

		}
		//lock = false;
				
		return temp;
	//}
}

void Pipe::write(const std::string& msg)
{
	//while (test_and_set(&lock)) //synchronizacaj procesów
	//{
		if (p1->deskryptor[1] != 1)	//sprawdzanie czy dany proces ma prawa do tej funkcji
		{
			std::cout << "Nie mo¿esz tego zrobiæ" << std::endl;
		}
		else if (p1->deskryptor[1] == 1)
		{
			for (auto x : msg)	//zapisywanie wiadomoœci do potoku
			{
				buffer.push(x);
			}
		}
	//	lock = false;
	//}
}

Pipeline::Pipeline()
{
}

void Pipeline::createPipe(PCB& p1,PCB& p2)	//tworzenie potoku
{
	bool check=false;
	int i;
	for (i = 0; i < pipes.size(); i++)	//szukanie pustego miejsca w tablicy potoków
	{
		if (pipes[i] == NULL)
		{
			check = true;
			break;
		}
	}
	if (check)	//dodawanie potoku w puste miejsce
	{
		Pipe* temp = new Pipe(p1, p2, (*this));
		pipes[i] = temp;
		p1.deskryptor[0] = i;
		p1.deskryptor[1] = 1;
		p2.deskryptor[0] = i;
		p2.deskryptor[1] = 0;
	}
	else	//dodawanie potoku na koñcu tablicy
	{
		Pipe* temp = new Pipe(p1, p2, (*this));
		pipes.push_back(temp);
		p1.deskryptor[0] = pipes.size() - 1;
		p1.deskryptor[1] = 1;
		p2.deskryptor[0] = pipes.size() - 1;
		p2.deskryptor[1] = 0;
	}
}

void Pipeline::deletePipe(PCB& p1)	//usuwanie potoku
{
	int temp = p1.deskryptor[0];
	delete(pipes[temp]);
	pipes[temp] = NULL;
}

bool Pipeline::Pipe_exist(PCB & p1, PCB & p2)
{
	if (p1.deskryptor[0] == p2.deskryptor[0] && p1.deskryptor[1]==1 &&p1.deskryptor[0]!=-1)
	{
		return true;
	}
	else
	{
		createPipe(p1, p2);
		return true;
	}
}
