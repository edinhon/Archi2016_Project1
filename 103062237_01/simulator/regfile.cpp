#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "regfile.h"

regfile::regfile(){
    for(int i = 0 ; i < 32 ; i++){
        Register[i] = 0;
    }
    Register[29] = 0x400;
}

void regfile::printRegister(){
	for(int i = 0 ; i < 32 ; i++){
		printf("$%0.2d: 0x%0.8X\n", i, Register[i]);
	}
}

//R-TYPE
void regfile::add (unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = Register[rs] + Register[rt];
	*PC += 1;
}
void regfile::addu(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = Register[rs] + (unsigned int)Register[rt];
	*PC += 1;
}
void regfile::sub (unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = Register[rs] - Register[rt];
	*PC += 1;
}
void regfile::andf(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = Register[rs] & Register[rt];
	*PC += 1;
}
void regfile::orf (unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = Register[rs] | Register[rt];
	*PC += 1;
}
void regfile::xorf(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = Register[rs] ^ Register[rt];
	*PC += 1;
}
void regfile::nor (unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = ~(Register[rs] | Register[rt]);
	*PC += 1;
}
void regfile::nand(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = ~(Register[rs] & Register[rt]);
	*PC += 1;
}
void regfile::slt (unsigned int rs, unsigned int rt, unsigned int rd, unsigned int *PC){
	Register[rd] = (Register[rt] > Register[rs]) ? 1 : 0;
	*PC += 1;
}
void regfile::sll (unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int *PC){
	Register[rd] = Register[rt] << shamt;
	*PC += 1;
}
void regfile::srl (unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int *PC){
	Register[rd] = Register[rt] >> shamt;
	*PC += 1;
}
void regfile::sra (unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int *PC){
	if((Register[rt] >> 31) == 0x00000001) {
		Register[rd] = (0xFFFFFFFF << (32 - shamt)) | (Register[rt] >> shamt);
	}
	else{
		Register[rd] = Register[rt] >> shamt;
	}
	*PC += 1;
}
void regfile::jr  (unsigned int rs, unsigned int *PC){
	*PC = Register[rs] >> 2;
}


//I-TYPE
void regfile::addi (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = Register[rs] + immediate;
	*PC += 1;
}
void regfile::addiu(unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = Register[rs] + (unsigned int)immediate;
	*PC += 1;
}
void regfile::lw   (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Register[rt] = (( Memory[Register[rs] + immediate] << 24 ) & 0xFF000000) | (( Memory[Register[rs] + immediate + 1] << 16 ) & 0x00FF0000) |
					(( Memory[Register[rs] + immediate + 2] << 8 ) & 0x0000FF00) | (( Memory[Register[rs] + immediate + 3] ) & 0x000000FF);
	*PC += 1;
}
void regfile::lh   (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Register[rt] = (((Memory[Register[rs] + immediate] << 24 ) >> 16) ) | ((( Memory[Register[rs] + immediate + 1] << 24) >> 24) );
	//Register[rt] = Register[rt] & 0x0000FFFF;
	*PC += 1;
}
void regfile::lhu  (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Register[rt] = (((Memory[Register[rs] + immediate] << 24 ) >> 16) & 0x0000FF00) | ((( Memory[Register[rs] + immediate + 1] << 24) >> 24) & 0x000000FF);
	//Register[rt] = Register[rt] & 0x0000FFFF;
	Register[rt] = (unsigned int)Register[rt];
	*PC += 1;
}
void regfile::lb   (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Register[rt] = ((( Memory[Register[rs] + immediate] << 24) >> 24));
	//Register[rt] = Register[rt] & 0x000000FF;
	*PC += 1;
}
void regfile::lbu  (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Register[rt] = ((( Memory[Register[rs] + immediate] << 24) >> 24) & 0x000000FF);
	//Register[rt] = Register[rt] & 0x000000FF;
	Register[rt] = (unsigned int)Register[rt];
	*PC += 1;
}
void regfile::sw   (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Memory[Register[rs] + immediate] = ( Register[rt] >> 24 ) & 0x000000FF;
	Memory[Register[rs] + immediate + 1] = ( Register[rt] << 8 ) >> 24 & 0x000000FF;
	Memory[Register[rs] + immediate + 2] = ( Register[rt] << 16 ) >> 24 & 0x000000FF;
	Memory[Register[rs] + immediate + 3] = ( Register[rt] << 24 ) >> 24 & 0x000000FF;
	*PC += 1;
}
void regfile::sh   (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Memory[Register[rs] + immediate] = ( (Register[rt]&0x0000FFFF) << 16 ) >> 24;
	Memory[Register[rs] + immediate] = Memory[Register[rs] + immediate] & 0x000000FF;
	Memory[Register[rs] + immediate + 1] = ( (Register[rt]&0x0000FFFF) << 24 ) >> 24;
	Memory[Register[rs] + immediate + 1] = Memory[Register[rs] + immediate + 1] & 0x000000FF;
	*PC += 1;
}
void regfile::sb   (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC, char Memory[]){
	Memory[Register[rs] + immediate] = ( (Register[rt]&0x000000FF) << 24 ) >> 24 & 0x000000FF;
	Memory[Register[rs] + immediate] = Memory[Register[rs] + immediate] & 0x000000FF;
	*PC += 1;
}
void regfile::lui  (unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = (immediate << 16);
	*PC += 1;
}
void regfile::andi (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = Register[rs] & (unsigned int)immediate;
	*PC += 1;
}
void regfile::ori  (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = Register[rs] | (unsigned int)immediate;
	*PC += 1;
}
void regfile::nori (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = ~(Register[rs] & (unsigned int)immediate);
	*PC += 1;
}
void regfile::slti (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	Register[rt] = (Register[rs] < immediate) ? 1 : 0;
	*PC += 1;
}
void regfile::beq  (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	if(Register[rs] == Register[rt]) *PC += (1 + immediate);
	else *PC += 1;
}
void regfile::bne  (unsigned int rs, unsigned int rt, int immediate, unsigned int *PC){
	if(Register[rs] != Register[rt]) *PC += (1 + immediate);
	else *PC += 1;
}
void regfile::bgtz (unsigned int rs, int immediate, unsigned int *PC){
	if(Register[rs] > 0) *PC += (1 + immediate);
	else *PC += 1;
}


//J-TYPE
void regfile::jal  (unsigned int address, unsigned int *PC){
	Register[31] = (*PC + 1);
	Register[31] = Register[31] << 2;
	*PC = address;
}
