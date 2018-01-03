#include "Planista.h"

PCB running;
//extern class Tree;
Planista::Planista()
{
	for (int i = 0; i < 8; i++)
	{
		kolejka_bool.push_back(false);
	}
}

Planista::~Planista()
{
}

void Planista::dodaj_do_kolejki(PCB &x) {
	if (x.Dynamic_priority == NULL)
	{
		x.Dynamic_priority = x.Priority;
	}
	int nr = x.Dynamic_priority / 4;
	mapa_kolejek[nr].push_back(x);
	kolejka_bool.at(nr) = true;
}

void Planista::dzielnik_cpu() {

	for (int i = 0; i < 8; i++)												//pêtla sprawdzaj¹ca mape kolejek
	{
		for (std::list<PCB>::iterator it = mapa_kolejek[i].begin(); it != mapa_kolejek[i].end(); it++)
		{
			it->CPU = it->CPU / 2; //std::cout << "Podzielono! ", it->Process_name, " \n";

		}
	}

}
void Planista::powrot_do_kolejki(PCB &x) {
	x.Dynamic_priority = x.Priority + (x.CPU / 2);
	int nr = x.Priority / 4;
	mapa_kolejek[nr].push_back(x);
	kolejka_bool.at(nr) = true;
}

void Planista::run(Tree &t) {
	std::vector<PCB> r = t.Ready_processes();
	/*int j = 0;*/
	//std::cout<<r.size();
	//while (r.empty() != true)
	//{
	//	dodaj_do_kolejki(r[j]);
	//	j++;
	//}
	for (int i = 0; i < r.size(); i++)
	{
		dodaj_do_kolejki(r[i]);
	}
	int i = 0;
	while (kolejka_bool.at(i) == false)				//sprawdzanie pierwszej dostêpnej kolejki w mapie
	{												//sprawdzanie pierwszej dostêpnej kolejki w mapie
		if (kolejka_bool.at(i) == true)				//sprawdzanie pierwszej dostêpnej kolejki w mapie
		{											//sprawdzanie pierwszej dostêpnej kolejki w mapie
			break;									//sprawdzanie pierwszej dostêpnej kolejki w mapie
		}											//sprawdzanie pierwszej dostêpnej kolejki w mapie
		i++;
		if (i == 8)
		{
			return;
		}

	}										//sprawdzanie pierwszej dostêpnej kolejki w mapie

	PCB x = mapa_kolejek[i].front();
	mapa_kolejek[i].pop_front();
	//x.Command_counter = 3;
	running = x;
	running.Change_process_state(Running);
}



void Planista::wykonanie_rozkazu(int a) {
	running.Command_counter -= a - 1; //-1 bo spacja jest pomijana
									  //std::cout << "wykonano rozkazy\n";
}

void Planista::make_zombie() {

	//running.PID = NULL; !!!!!!!!!!!!!!!!!!!!!
	running.Change_process_state(Zombie);
	running.Change_process_state(Terminated);
	running.PID = NULL;
}

void Planista::check(PCB &actual, Tree &t) {
	actual.Change_process_state(Ready);
	if (running.PID == NULL)
	{
		std::cout << "brak wybranego procesu running\n";
		dzielnik_cpu();
		run(t);
		actual = running;
		return;
	}
	if (running.PID != NULL)
	{
		running = actual;
		//std::cout << "odk;adanie procesu\n";
		powrot_do_kolejki(running);
		dzielnik_cpu();
		run(t);
		/*actual = running;*/
		return;
	}
	else
	{
		//std::cout << "Proces w Procesorze\n";
		actual = running;
		return;
	}

}
int Planista::get_A()
{
	return running.Reg1;
}

int Planista::get_B()
{
	return running.Reg2;
}

int Planista::get_C()
{
	return running.Reg3;
}
int get_PID()
{
	return running.PID;
}

void Planista::ret(int a, int b, int c)
{
	running.Reg1 = a;
	running.Reg2 = b;
	running.Reg3 = c;

}
