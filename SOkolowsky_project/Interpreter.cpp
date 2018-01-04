#include "Interpreter.h"

/*extern class MemoryManager;
extern class PCB;
extern class Pipe;
extern class Pipeline;
extern class Tree;
extern class Planista;*/

	/*interpreter::interpreter() {
		PID = 0;
		rejA = 0;
		rejB = 0;
		rejC = 0;
		rejD = 0;
		liczRoz = 0;
	}

	interpreter::~interpreter() {
	}
	*/
	void interpreter::PobierzRejestry(Planista &planista, PCB &pcb, Tree &tree){ //pobieranie rejestr�w z procesu kt�ry jest w stanie running

		//planista.check(pcb, tree); //ustawianie procesu running
		PID = pcb.PID;
		rejA = pcb.Reg1;
		rejB = pcb.Reg2;
		rejC = pcb.Reg3;
		rejD = pcb.Reg4;
		liczRoz = pcb.Command_counter;
	}

	void interpreter::StanRej()
	{
		std::cout << "PID: " << PID << std::endl;
		std::cout << "A: " << rejA << std::endl;
		std::cout << "B: " << rejB << std::endl;
		std::cout << "C: " << rejC << std::endl;
		std::cout << "Licznik Rozkazow: " << liczRoz << std::endl;
		std::cout << "--------------------------------" << std::endl << std::endl;
	}

	void interpreter::zapiszRejestry(PCB &pcb){

		pcb.PID = PID;
		pcb.Reg1 = rejA;
		pcb.Reg2 = rejB;
		pcb.Reg3 = rejC;
		pcb.Command_counter = liczRoz;
		pcb.CPU+=liczRoz;
	}

	std::string interpreter::pobierzRozkaz(MemoryManager &mm, PCB &pcb){

		std::string rozkaz;
		rozkaz = mm.Get(&pcb, liczRoz);
		liczRoz += rozkaz.size() + 1;
		return rozkaz;

	}

	void interpreter::WykonajProgram(MemoryManager &mm, PCB &pcb, Planista &planista, Tree &tree, Pipeline &pipeline){
		/////////////////////////////////////
		PobierzRejestry(planista, pcb, tree);
		StanRej();
		std::string operacja;
		operacja.clear();
		operacja = pobierzRozkaz(mm, pcb);
		////planista.wykonanie_rozkazu(operacja.size());
		std::cout << "Operacja: " << operacja << std::endl;
		std::string rej1 = " ", rej2 = " ";

		int liczba = 0;

		///////////////////////////////////// dodawanie

		if (operacja == "AD"){

			rej1 = pobierzRozkaz(mm, pcb);
			////planista.wykonanie_rozkazu(rej1.size());
			rej2 = pobierzRozkaz(mm, pcb);
			////planista.wykonanie_rozkazu(rej2.size());

			if (rej2 != "A" && rej2 != "B" && rej2 != "C")
			{
				liczba = stoi(rej2);
			}
			//////

			if (rej1 == "A" && liczba != 0)
			{
				rejA += liczba;
			}
			else if (rej1 == "A" && rej2 == "B")
			{
				rejA += rejB;
			}
			else if (rej1 == "A" && rej2 == "C")
			{
				rejA += rejC;
			}
			else if (rej1 == "A" && rej2 == "A")
			{
				rejA += rejA;
			}

			//////////////////
			if (rej1 == "B" && liczba != 0)
			{
				rejB += liczba;
			}
			else if (rej1 == "B" && rej2 == "A")
			{
				rejB += rejA;
			}
			else if (rej1 == "B" && rej2 == "C")
			{
				rejB += rejC;
			}
			else if (rej1 == "B" && rej2 == "B")
			{
				rejB += rejB;
			}
			///////////////////
			if (rej1 == "C" && liczba != 0)
			{
				rejC += liczba;
			}
			else if (rej1 == "C" && rej2 == "B")
			{
				rejC += rejB;
			}
			else if (rej1 == "C" && rej2 == "A")
			{
				rejC += rejA;
			}
			else if (rej1 == "C" && rej2 == "C")
			{
				rejC += rejC;
			}
			StanRej();
			zapiszRejestry(pcb);
		}
		////////////////////////////////////////// odejmowanie
		if (operacja == "SB")
		{

			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			rej2 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej2.size());

			if (rej2 != "A" && rej2 != "B" && rej2 != "C") liczba = stoi(rej2);

			/////

			if (rej1 == "A" && liczba != 0)
			{
				rejA -= liczba;
			}
			else if (rej1 == "A" && rej2 == "B")
			{
				rejA -= rejB;
			}
			else if (rej1 == "A" && rej2 == "C")
			{
				rejA -= rejC;
			}
			else if (rej1 == "A" && rej2 == "A")
			{
				rejA -= rejA;
			}
			//////////////////
			if (rej1 == "B" && liczba != 0)
			{
				rejB -= liczba;
			}
			else if (rej1 == "B" && rej2 == "A")
			{
				rejB -= rejA;
			}
			else if (rej1 == "B" && rej2 == "C")
			{
				rejB -= rejC;
			}
			else if (rej1 == "B" && rej2 == "B")
			{
				rejB -= rejB;
			}
			///////////////////
			if (rej1 == "C" && liczba != 0)
			{
				rejC -= liczba;
			}
			else if (rej1 == "C" && rej2 == "B")
			{
				rejC -= rejB;
			}
			else if (rej1 == "C" && rej2 == "A")
			{
				rejC -= rejA;
			}
			else if (rej1 == "C" && rej2 == "C")
			{
				rejC -= rejC;
			}
			StanRej();
			zapiszRejestry(pcb);
		}
		////////////////////////////////////////////mnozenie
		if (operacja == "MU")
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			rej2 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej2.size());

			if (rej2 != "A" && rej2 != "B" && rej2 != "C") liczba = stoi(rej2);

			//////

			if (rej1 == "A" && liczba != 0)
			{
				rejA *= liczba;
			}
			else if (rej1 == "A" && rej2 == "B")
			{
				rejA *= rejB;
			}
			else if (rej1 == "A" && rej2 == "C")
			{
				rejA *= rejC;
			}
			else if (rej1 == "A" && rej2 == "A")
			{
				rejA *= rejA;
			}
			//////////////////
			if (rej1 == "B" && liczba != 0)
			{
				rejB *= liczba;
			}
			else if (rej1 == "B" && rej2 == "A")
			{
				rejB *= rejA;
			}
			else if (rej1 == "B" && rej2 == "C")
			{
				rejB *= rejC;
			}
			else if (rej1 == "B" && rej2 == "B")
			{
				rejB *= rejB;
			}
			///////////////////
			if (rej1 == "C" && liczba != 0)
			{
				rejC *= liczba;
			}
			else if (rej1 == "C" && rej2 == "B")
			{
				rejC *= rejB;
			}
			else if (rej1 == "C" && rej2 == "A")
			{
				rejC *= rejA;
			}
			else if (rej1 == "C" && rej2 == "C")
			{
				rejC *= rejC;
			}
			StanRej();
			zapiszRejestry(pcb);
		}
		//////////////////////////////////////dzielenie
		if (operacja == "DV")
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			rej2 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej2.size());

			if (rej2 != "A" && rej2 != "B" && rej2 != "C") liczba = stoi(rej2);

			/////

			if (rej1 == "A" && liczba != 0)
			{
				rejA /= liczba;
			}
			else if (rej1 == "A" && rej2 == "B")
			{
				rejA /= rejB;
			}
			else if (rej1 == "A" && rej2 == "C")
			{
				rejA /= rejC;
			}
			else if (rej1 == "A" && rej2 == "A")
			{
				rejA /= rejA;
			}
			//////////////////
			if (rej1 == "B" && liczba != 0)
			{
				rejB /= liczba;
			}
			else if (rej1 == "B" && rej2 == "A")
			{
				rejB /= rejA;
			}
			else if (rej1 == "B" && rej2 == "C")
			{
				rejB /= rejC;
			}
			else if (rej1 == "B" && rej2 == "B")
			{
				rejB /= rejB;
			}
			///////////////////
			if (rej1 == "C" && liczba != 0)
			{
				rejC /= liczba;
			}
			else if (rej1 == "C" && rej2 == "B")
			{
				rejC /= rejB;
			}
			else if (rej1 == "C" && rej2 == "A")
			{
				rejC /= rejA;
			}
			else if (rej1 == "C" && rej2 == "C")
			{
				rejC /= rejC;
			}
			StanRej();
			zapiszRejestry(pcb);
		}
		/////////////////////////////////////// zapis warto�ci do rejestru
		if (operacja == "MO")
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			rej2 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej2.size());

			if (rej2 != "A" && rej2 != "B" && rej2 != "C") liczba = stoi(rej2);

			//////////

			if (rej1 == "A" && liczba != 0)
			{
				rejA = liczba;
			}
			else if (rej1 == "A" && rej2 == "B")
			{
				rejA = rejB;
			}
			else if (rej1 == "A" && rej2 == "C")
			{
				rejA = rejC;
			}
			//////////////////
			if (rej1 == "B" && liczba != 0)
			{
				rejB = liczba;
			}
			else if (rej1 == "B" && rej2 == "A")
			{
				rejB = rejA;
			}
			else if (rej1 == "B" && rej2 == "C")
			{
				rejB = rejC;
			}

			///////////////////
			if (rej1 == "C" && liczba != 0)
			{
				rejC = liczba;
			}
			else if (rej1 == "C" && rej2 == "B")
			{
				rejC = rejB;
			}
			else if (rej1 == "C" && rej2 == "A")
			{
				rejC = rejA;
			}
			StanRej();
			zapiszRejestry(pcb);
		}
		/////////////////////// inkrementacja i dekrementacja rejestr�w

		if (operacja == "IC")
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			if (rej1 == "A")
				rejA++;
			if (rej1 == "B")
				rejB++;
			if (rej1 == "C")
				rejC++;

			StanRej();
			zapiszRejestry(pcb);
		}

		if (operacja == "DC")
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			if (rej1 == "A")
				rejA--;
			if (rej1 == "B")
				rejB--;
			if (rej1 == "C")
				rejC--;

			StanRej();
			zapiszRejestry(pcb);
		}


		/////////////////////////////// pobieranie wartosci z pamici

		if (operacja == "MV")
		{

			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			rej2 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej2.size());
			liczba = stoi(rej2);
			//rej2 = pobierzRozkaz(mm);
			if (rej1 == "A");
			{
				rejA = stoi(mm.Get(&pcb, liczba));
			}
			if (rej1 == "B")
			{
				rejB = stoi(mm.Get(&pcb, liczba));
			}
			if (rej1 == "C")
			{
				rejC = stoi(mm.Get(&pcb, liczba));
			}

			StanRej();
			zapiszRejestry(pcb);

		}

		////////////////////////////////////////// skoki

		if (operacja == "JP") //skok do adresu
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			liczRoz = stoi(rej1);

			StanRej();
			zapiszRejestry(pcb);
		}
		//////

		if (operacja == "JM") // skok do pami�ci pod warunkiem
		{
			rej1 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			int adres = stoi(rej1);
			//rej2 = pobierzRozkaz(mm, pcb);
			////planista.wykonanie_rozkazu(rej2.size());
			if (rejD > 0)
			{
				liczRoz = adres;
			}

			StanRej();
			zapiszRejestry(pcb);
			rejD--;

		}




		////////////////////////////////////////// potoki i komunikacja

		if (operacja == "SP") // stworzenie potoku
		{
			int id1, id2;
			id1 = stoi(pobierzRozkaz(mm, pcb));
			//planista.wykonanie_rozkazu(sizeof(id1));
			id2 = stoi(pobierzRozkaz(mm, pcb));
			//planista.wykonanie_rozkazu(sizeof(id2));
			std::cout << "id_1: " << id1 << " id2: " << id2 << std::endl;
			pipeline.createPipe(tree.Get_process(id1), tree.Get_process(id2));
			std::cout << "utworzono potok" << std::endl;
			tree.Display_PCB(mm, &(tree.Get_process(id1)));
			std::cout << std::endl;
			tree.Display_PCB(mm, &(tree.Get_process(id2)));
			std::cout << std::endl;
			//StanRej();
			zapiszRejestry(pcb);

		}
		////////
		if (operacja == "UP") //usuniecie potoku
		{
			int id;
			id = stoi(pobierzRozkaz(mm, pcb));
			std::cout << "id: " << id << std::endl;
			//planista.wykonanie_rozkazu(sizeof(id));
			pipeline.deletePipe(tree.Get_process(id));
			//StanRej();
			zapiszRejestry(pcb);
			std::cout << "usunieto potok " << std::endl;
		}

		if (operacja == "SM") //wys�anie wiadomosci SM id wiadomosc
		{
			int id;
			id = stoi(pobierzRozkaz(mm, pcb));
			//planista.wykonanie_rozkazu(sizeof(id));
			rej1 = pobierzRozkaz(mm, pcb);
			std::cout << "id: " << id << std::endl;
			//planista.wykonanie_rozkazu(rej1.size());
			PCB p1;
			p1 = tree.Get_process(id);
			if (p1.Descriptor[0] >= 0)
			{
				pipeline.pipes[p1.Descriptor[0]]->write(rej1);
			}
			else
			{
				std::cout << "Proces nie przypisany do potoku" << std::endl;
			}
			//StanRej();
			zapiszRejestry(pcb);
		}

		/////

		if (operacja == "RM") //odczytanie wiadomosci RM id ile_chcesz_odczyta�
		{
			int id;
			id = stoi(pobierzRozkaz(mm, pcb));
			std::cout << "id: " << id << std::endl;
			//planista.wykonanie_rozkazu(sizeof(id));
			rej2 = pobierzRozkaz(mm, pcb);
			//planista.wykonanie_rozkazu(rej1.size());
			PCB p1;
			p1 = tree.Get_process(id);
			if (p1.Descriptor[0] >= 0)
			{
				rej1 = pipeline.pipes[p1.Descriptor[0]]->read(stoi(rej2));
			}
			else
			{
				std::cout << "Proces nie przypisany do potoku" << std::endl;
			}
			//StanRej();
			zapiszRejestry(pcb);
		}

		////////////////////////////////////////

		if (operacja == "CL") //czyszczenie ekranu
		{
			system("cls");
		}

		//////////////////////////////////// dysk

		if (operacja == "WP") //wypisz dysk
		{

		}
		if (operacja == "WF") //wypisz fat
		{

		}
		if (operacja == "CF") //utworz plik
		{

		}
		if (operacja == "ZP") // zapisz do pliku
		{

		}
		if (operacja == "CP") //czytaj plik
		{

		}

		//////////////////////////////////// konta




		/////////////////////////////pamiec

		if (operacja == "PP")
		{
			mm.showPMemory();
			std::cout << std::endl;
			StanRej();
			zapiszRejestry(pcb);
		}


		////////////////////////////procesy

		if (operacja == "XW") //wyswietlanie drzewa proces�w
		{
			tree.Display_tree();
			std::cout << std::endl;
			StanRej();
			zapiszRejestry(pcb);
		}

		//////////////////

		if (operacja == "HT") //koniec pliku, zapis rej do procesu
		{
			zapiszRejestry(pcb);
		}




	}
