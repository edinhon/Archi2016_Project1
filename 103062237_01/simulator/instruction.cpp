#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "instruction.h"
#include "regfile.h"


instruction::instruction(){
    for(int i = 0 ; i < 256 ; i++){
        I_memory[i] = 0;
    }
}

void instruction::readInstruction(unsigned int *PC){

	FILE *fPtr;
	unsigned char input[4];

	fPtr = fopen("iimage.bin", "rb");

	if (!fPtr) {
        printf("wrong data\n");
        return ;
    }

    fread(&input[0], sizeof(unsigned char), 1, fPtr);
    fread(&input[1], sizeof(unsigned char), 1, fPtr);
    fread(&input[2], sizeof(unsigned char), 1, fPtr);
    fread(&input[3], sizeof(unsigned char), 1, fPtr);
    *PC = ( input[0] << 24 ) + ( input[1] << 16 ) + ( input[2] <<  8 ) + input[3];
	*PC /= 4;
	//printf("%d\n", *PC);

    fread(&input[0], sizeof(unsigned char), 1, fPtr);
    fread(&input[1], sizeof(unsigned char), 1, fPtr);
    fread(&input[2], sizeof(unsigned char), 1, fPtr);
    fread(&input[3], sizeof(unsigned char), 1, fPtr);
    times = ( input[0] << 24 ) + ( input[1] << 16 ) + ( input[2] <<  8 ) + input[3];

    for(int i = 0 ; i < times ; i++){
        fread(&input[0], sizeof(unsigned char), 1, fPtr);
        fread(&input[1], sizeof(unsigned char), 1, fPtr);
        fread(&input[2], sizeof(unsigned char), 1, fPtr);
        fread(&input[3], sizeof(unsigned char), 1, fPtr);
        I_memory[*PC + i] |= ( input[0] << 24 ) + ( input[1] << 16 ) + ( input[2] <<  8 ) + input[3];
        //printf("%0.8X\n", I_memory[*PC + i]);
    }

    fclose(fPtr);
}

void instruction::decode(unsigned int i){
    op = (I_memory[i] >> 26);
    //printf("%X\n", op);
    //R-TYPE
    if(op == 0x00){
        rs = (I_memory[i] << 6);
        rs = (rs >> 27);
        rt = (I_memory[i] << 11);
        rt = (rt >> 27);
        rd = (I_memory[i] << 16);
        rd = (rd >> 27);
        shamt = (I_memory[i] << 21);
        shamt = (shamt >> 27);
        funct = (I_memory[i] << 26);
        funct = (funct >> 26);
        //printf("%X %X %X %X %X\n", rs, rt, rd, shamt, funct);
    }
    //J-TYPE
    else if(op == 0x02){
        address = (I_memory[i] << 6);
        address = (address >> 6);
    }
    else if(op == 0x03){
        address = (I_memory[i] << 6);
        address = (address >> 6);
    }
    //halt
    else if(op == 0x3F){
        address = (I_memory[i] << 6);
        address = (address >> 6);
    }
    //I-TYPE
    else{
        rs = (I_memory[i] << 6);
        rs = (rs >> 27);
        rt = (I_memory[i] << 11);
        rt = (rt >> 27);
        immediate = (I_memory[i] << 16);
        immediate = (immediate >> 16);
    }

}

void instruction::implement(unsigned int *PC, regfile *reg, char Memory[]){
	//printf("%X\n", op);
	//R-TYPE
    if(op == 0x00){
		//add
        if(funct == 0x20){
			reg->add(rs, rt, rd, PC);
		}
		//addu
		else if(funct == 0x21){
            reg->addu(rs, rt, rd, PC);
		}
		//sub
		else if(funct == 0x22){
            reg->sub(rs, rt, rd, PC);
		}
		//and
		else if(funct == 0x24){
            reg->andf(rs, rt, rd, PC);
		}
		//or
		else if(funct == 0x25){
            reg->orf(rs, rt, rd, PC);
		}
		//xor
		else if(funct == 0x26){
            reg->xorf(rs, rt, rd, PC);
		}
		//nor
		else if(funct == 0x27){
            reg->nor(rs, rt, rd, PC);
		}
		//nand
		else if(funct == 0x28){
            reg->nand(rs, rt, rd, PC);
		}
		//slt
		else if(funct == 0x2A){
            reg->slt(rs, rt, rd, PC);
		}
		//sll
		else if(funct == 0x00){
            reg->sll(rt, rd, shamt, PC);
		}
		//srl
		else if(funct == 0x02){
            reg->srl(rt, rd, shamt, PC);
		}
		//sra
		else if(funct == 0x03){
            reg->sra(rt, rd, shamt, PC);
		}
		//jr
		else if(funct == 0x08){
            reg->jr(rs, PC);
		}
    }
    //J-TYPE
    else if(op == 0x02){
        *PC = address;
    }
    else if(op == 0x03){
        reg->jal(address, PC);
    }
    //halt
    else if(op == 0x3F){
        return ;
    }
    //I-TYPE
    else{
		//addi
        if(op == 0x08){
			reg->addi(rs, rt, immediate, PC);
		}
		//addiu
		else if(op == 0x09){
			reg->addiu(rs, rt, immediate, PC);
		}
		//lw
		else if(op == 0x23){
            reg->lw(rs, rt, immediate, PC, Memory);
		}
		//lh
		else if(op == 0x21){
            reg->lh(rs, rt, immediate, PC, Memory);
		}
		//lhu
		else if(op == 0x25){
            reg->lhu(rs, rt, immediate, PC, Memory);
		}
		//lb
		else if(op == 0x20){
            reg->lb(rs, rt, immediate, PC, Memory);
		}
		//lbu
		else if(op == 0x24){
            reg->lbu(rs, rt, immediate, PC, Memory);
		}
		//sw
		else if(op == 0x2B){
            reg->sw(rs, rt, immediate, PC, Memory);
		}
		//sh
		else if(op == 0x29){
            reg->sh(rs, rt, immediate, PC, Memory);
		}
		//sb
		else if(op == 0x28){
            reg->sb(rs, rt, immediate, PC, Memory);
		}
		//lui
		else if(op == 0x0F){
            reg->lui(rt, immediate, PC);
		}
		//andi
		else if(op == 0x0C){
            reg->andi(rs, rt, immediate, PC);
		}
		//ori
		else if(op == 0x0D){
            reg->ori(rs, rt, immediate, PC);
		}
		//nori
		else if(op == 0x0E){
            reg->nori(rs, rt, immediate, PC);
		}
		//slti
		else if(op == 0x0A){
            reg->slti(rs, rt, immediate, PC);
		}
		//beq
		else if(op == 0x04){
            reg->beq(rs, rt, immediate, PC);
		}
		//bne
		else if(op == 0x05){
            reg->bne(rs, rt, immediate, PC);
		}
		//bgtz
		else if(op == 0x07){
            reg->bgtz(rs, immediate, PC);
		}
    }
}

