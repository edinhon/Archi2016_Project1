#include <iostream>
#include <cstdio>
#include "instruction.h"
#include "memory.h"
#include "regfile.h"

using namespace std;

unsigned int PC = 0;

int main()
{
    instruction inst;
    memory memo;
	regfile reg;
	inst.readInstruction(&PC);
    memo.readMemory(&(reg.Register[29]));

	/*char a = 0x81, b = -0x80;
	printf("%d %d", a, b);*/

	while(inst.op != 0x3F){
        printf("%d\n", PC);
		inst.decode(PC);
		inst.implement(&PC, &reg, memo.D_memory);
		reg.printRegister();
	}

	return 0;
}
