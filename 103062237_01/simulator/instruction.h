#ifndef LAB1_INSTRUCTION_H
#define LAB1_INSTRUCTION_H

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include "regfile.h"

class instruction{

public:

    instruction();

	void readInstruction(unsigned int *PC);

	void decode(unsigned int i);

	void implement(unsigned int *PC, regfile *reg, char Memory[]);

	unsigned int I_memory[256] = {0};
	int times = 0;
	unsigned int op, rs, rt, rd, shamt, funct, address;
	int immediate;
};

#endif LAB1_INSTRUCTION_H
