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
	printf("cycle %d\n", i);
    memo.readMemory(&(reg.Register[29]));
	reg.printRegister();
	printf("$PC: 0x%0.8X\n", PC*4);
    i++;
	/*char a = 0x81, b = -0x80;
	printf("%d %d", a, b);*/


	while(inst.op != 0x3F){
		inst.decode(PC);
		inst.implement(&PC, &reg, memo.D_memory);
		if(inst.op != 0x3F){
			printf("cycle %d\n", i);
			reg.printRegister();
			printf("$PC: 0x%0.8X\n", PC*4);
			i++;
		}
	}

	return 0;
}
