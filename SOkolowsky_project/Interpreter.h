#pragma once


#include <iostream>
#include <string>
#include <vector>
#include "Pipeline.h"
#include "Procesy.h"
#include "MemoryManager.h"
#include "Planista.h"



extern class MemoryManager;
extern class PCB;
extern class Pipe;
extern class Pipeline;
extern class Tree;
extern class Planista;




class interpreter
{
public:

	interpreter();

	void PobierzRejestry(Planista &planista, PCB &pcb, Tree &tree);  
	
	void StanRej();

	void zapiszRejestry(PCB &pcb);

	std::string pobierzRozkaz(MemoryManager &mm, PCB &pcb);

	void WykonajProgram(MemoryManager &mm, PCB &pcb, Planista &planista, Tree &tree, Pipeline &pipeline);
	



};



