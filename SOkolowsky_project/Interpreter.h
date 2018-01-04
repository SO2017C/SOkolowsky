#pragma once
#include "Biblioteki.h"
#include "Pipeline.h"
#include "Processes.h"
#include "MemoryManager.h"
#include "Planner.h"

extern class MemoryManager;
extern class PCB;
extern class Pipe;
extern class Pipeline;
extern class Tree;
extern class Planista;

class interpreter
{
public:
	int PID;
	int rejA;
	int rejB;
	int rejC;
	int rejD;
	int liczRoz;

	//interpreter();

	//~interpreter();

	void PobierzRejestry(Planista &planista, PCB &pcb, Tree &tree);

	void StanRej();

	void zapiszRejestry(PCB &pcb);

	std::string pobierzRozkaz(MemoryManager &mm, PCB &pcb);

	void WykonajProgram(MemoryManager &mm, PCB &pcb, Planista &planista, Tree &tree, Pipeline &pipeline);

};