#include <iostream>
#include <cstdio>
#include "instruction.h"
#include "memory.h"
#include "regfile.h"

using namespace std;

unsigned int PC = 0;

int main()
{
    int i = 0;
    instruction inst;
    memory memo;
	regfile reg;
	inst.readInstruction(&PC);
    memo.readMemory(&(reg.Register[29]));
	printf("cycle %d\n", i);
	reg.printRegister();
	printf("PC: 0x%0.8X\n", PC*4);
    i++;

	while(inst.op != 0x3F){
		inst.decode(PC);
		inst.implement(&PC, &reg, memo.D_memory);
		if(inst.op != 0x3F){
			printf("cycle %d\n", i);
			reg.printRegister();
			printf("PC: 0x%0.8X\n", PC*4);
			i++;
		}
	}

	return 0;
}
