//
//  Simulator.c
//  Simulator
//
//  Created by 陈志浩 on 16/8/21.
//  Copyright © 2016年 ZhihaoChen. All rights reserved.
//

/*
 *This simulator simulates a simple computer with basic three-
 *stage pipeline - INSTRUCTION FETCH, INSTRUCTION DECODE, EXECUTE.
 *It matches with assembler and only test on MacOS Platform.
 *Because of endianness, it probably won't work well on some
 *other platforms.
 */

#include "Simulator.h"

/*SIMULATOR ENTRY*/
/*************************************************
 Function: main
 Description: program entry
 Input:
 argc - number of arguments that command line passes in
 argv - arguments that command line passes in
 Output: ERROR messages
 Return: int stands for state of program
 Others: program entry
 *************************************************/
int main(int argc, const char * argv[]) {
    //processes command line arguments
    if (argc < 2)
    {
        printf("ERROR:too few arguments!\n");
        return -1;
    }
    //simulate memory
    if ((MEM = malloc(MEMORYSIZE)) == NULL)
    {
        printf("ERROR:fail to allocate memory!\n");
        return -1;
    }
    //open the file
    if ((fpIN = fopen(argv[1], "rb")) == NULL)
    {
        printf("ERROR:fail to open file %s for reading!\n", argv[1]);
        return -1;
    }
    //initialize
    init();
    fclose(fpIN);
    //execute codes
    exec();
    //free memory
    free(MEM);
    
    return 0;
}
/*INITIALIZE Z REGISTER LOAD HEADER AND CS, DS, SS, ES SEGMENT, LOAD CODE AND SYMBOL TO MEMORY*/
/*************************************************
 Function: init
 Description: initialize function -
 set 0 to Z register
 read the CS, DS, SS, ES register
 set PC register
 set SP register
 ...
 read the size of code and data
 load code and data
 Input: void
 Output: none
 Return: void
 Others: none
 *************************************************/
void init(void)
{
    GR[0] = 0;
    
    ret = 1;
    
    fread(&CS, sizeof(unsigned short), 1, fpIN);
    fread(&code_size, sizeof(unsigned short), 1, fpIN);
    
    fread(&DS, sizeof(unsigned short), 1, fpIN);
    fread(&data_size, sizeof(unsigned short), 1, fpIN);
    
    fread(&SS, sizeof(unsigned short), 1, fpIN);
    
    fread(&ES, sizeof(unsigned short), 1, fpIN);
    
    PC = (unsigned int *)(MEM+(CS<<4));
    
    SP = (unsigned short *)(MEM+(SS<<4));
    ESP = (EXTRA_STACK *)(MEM+(ES<<4)+65504);
    
    load_code();
    load_data();
}

/*LOAD CODE TO MEMORY*/
/*************************************************
 Function: load_code
 Description: load code from file to memory
 Input: void
 Output: none
 Return: void
 Others: none
 *************************************************/
void load_code(void)
{
    unsigned int instr;
    unsigned int * CODE_ADDRESS = PC;
    
    unsigned short code_count = 0;
    while (code_count < code_size)
    {
        fread(&instr, sizeof(unsigned int), 1, fpIN);
        memcpy(CODE_ADDRESS, &instr, sizeof(unsigned int));
        CODE_ADDRESS++;
        code_count += 4;
    }
}

/*LOAD SYMBOL TO MEMORY*/
/*************************************************
 Function: load_data
 Description: load data from file to memory
 Input: void
 Output: none
 Return: void
 Others: none
 *************************************************/
void load_data(void)
{
    unsigned char data;
    unsigned char * DATA_ADDRESS = (unsigned char *)(MEM+(DS<<4));
    
    unsigned short data_count = 0;
    while (data_count < data_size)
    {
        fread(&data, sizeof(unsigned char), 1, fpIN);
        memcpy(DATA_ADDRESS, &data, sizeof(unsigned char));
        DATA_ADDRESS++;
        data_count += 1;
    }
}

/*EXECUTE CODE UNTIL HLT*/
/*************************************************
 Function: exec
 Description: execute instructions from the start
 Input: void
 Output: none
 Return: void
 Others: none
 *************************************************/
void exec(void)
{
    while (ret)
    {
        IR = *PC;
        PC++;
        ret = (*instrs[INSTRCODE])();
    }
}
/*INSTRUCTIONS IMPLEMENTATION*/
int HLT(void)
{
    return 0;
}

int JMP(void)
{
    PC = (unsigned int *)(MEM+(CS<<4)+ADDRESS);
    return 1;
}

int CJMP(void)
{
    if (PSW.compare_flag)
    {
        PC = (unsigned int *)(MEM+(CS<<4)+ADDRESS);
    }
    return 1;
}

int OJMP(void)
{
    if (PSW.overflow_flag)
    {
        PC = (unsigned int *)(MEM+(CS<<4)+ADDRESS);
    }
    return 1;
}

int CALL(void)
{
    ESP--;
    ESP->PSW = PSW;
    ESP->PC = PC;
    memcpy(ESP->GR, GR, sizeof(GR));
    
    PC = (unsigned int *)(MEM+(CS<<4)+ADDRESS);
    return 1;
}

int RET(void)
{
    PSW = ESP->PSW;
    PC = ESP->PC;
    memcpy(GR, ESP->GR, sizeof(GR));
    ESP++;
    return 1;
}

int PUSH(void)
{
    SP++;
    *SP = GR[REG0];
    return 1;
}

int POP(void)
{
    GR[REG0] = *SP;
    SP--;
    return 1;
}

int LOADB(void)
{
    GR[REG0] = *((unsigned char *)(MEM+(DS<<4)+ADDRESS+GR[7]));
    return 1;
}

int LOADW(void)

{
    GR[REG0] = *((unsigned short *)(MEM+(DS<<4)+ADDRESS+2*GR[7]));
    return 1;
}

int STOREB(void)
{
    *((unsigned char *)(MEM+(DS<<4)+ADDRESS+GR[7])) = GR[REG0];
    return 1;
}

int STOREW(void)
{
    *((unsigned short *)(MEM+(DS<<4)+ADDRESS+2*GR[7])) = GR[REG0];
    return 1;
}

int LOADI(void)
{
    GR[REG0] = (short)(IMMEDIATE);
    return 1;
}

int NOP(void)
{
    return 1;
}

int IN(void)
{
    *(GR+REG0) = getchar();
    return 1;
}

int OUT(void)
{
    putchar(*(char *)(GR+REG0));
    return 1;
}

int ADD(void)
{
    GR[REG0] = GR[REG1] + GR[REG2];
    if (GR[REG2] > 0)
    {
        if (GR[REG0] < GR[REG1])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else if (GR[REG2] < 0)
    {
        if (GR[REG0] > GR[REG1])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else
    {
        PSW.overflow_flag = 0;
    }
    return 1;
}

int ADDI(void)
{
    if ((short)IMMEDIATE > 0)
    {
        if (GR[REG0] + (short)IMMEDIATE < GR[REG0])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else if ((short)IMMEDIATE < 0)
    {
        if (GR[REG0] + (short)IMMEDIATE > GR[REG0])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else
    {
        PSW.overflow_flag = 0;
    }
    GR[REG0] = GR[REG0] + (short)IMMEDIATE;
    return 1;
}

int SUB(void)
{
    GR[REG0] = GR[REG1] - GR[REG2];
    if (GR[REG2] > 0)
    {
        if (GR[REG0] > GR[REG1])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else if (GR[REG2] < 0)
    {
        if (GR[REG0] < GR[REG1])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else
    {
        PSW.overflow_flag = 0;
    }
    return 1;
}

int SUBI(void)
{
    if ((short)IMMEDIATE > 0)
    {
        if (GR[REG0] - (short)IMMEDIATE > GR[REG0])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else if ((short)IMMEDIATE < 0)
    {
        if (GR[REG0] - (short)IMMEDIATE < GR[REG0])
        {
            PSW.overflow_flag = 1;
        }
        else
        {
            PSW.overflow_flag = 0;
        }
    }
    else
    {
        PSW.overflow_flag = 0;
    }
    GR[REG0] = GR[REG0] - (short)IMMEDIATE;
    return 1;
}

int MUL(void)
{
    GR[REG0] = GR[REG1] * GR[REG2];
    if (abs(GR[REG2]) > 1)
    {
        if (abs(GR[REG0]) > abs(GR[REG1]))
        {
            PSW.overflow_flag = 0;
        }
        else
        {
            PSW.overflow_flag = 1;
        }
    }
    else
    {
        PSW.overflow_flag = 0;
    }
    return 1;
}

int DIV(void)
{
    if (GR[REG2] == 0)
    {
        printf("ERROR:use 0 as divisor!\n");
        exit(-1);
    }
    GR[REG0] = GR[REG1]/GR[REG2];
    return 1;
}

int AND(void)
{
    GR[REG0] = GR[REG1]&GR[REG2];
    return 1;
}

int OR(void)
{
    GR[REG0] = GR[REG1]|GR[REG2];
    return 1;
}

int NOR(void)
{
    GR[REG0] = GR[REG1]^GR[REG2];
    return 1;
}

int NOTB(void)
{
    GR[REG0] = ~GR[REG1];
    return 1;
}

int SAL(void)
{
    GR[REG0] = GR[REG1]<<GR[REG2];
    return 1;
}

int SAR(void)
{
    GR[REG0] = GR[REG1]>>GR[REG2];
    return 1;
}

int EQU(void)
{
    PSW.compare_flag=(GR[REG0]==GR[REG1]);
    return 1;
}

int LT(void)
{
    PSW.compare_flag = (GR[REG0]<GR[REG1]);
    return 1;
}

int LTE(void)
{
    PSW.compare_flag = (GR[REG0]<=GR[REG1]);
    return 1;
}

int NOTC(void)
{
    PSW.compare_flag = !PSW.compare_flag;
    return 1;
}
