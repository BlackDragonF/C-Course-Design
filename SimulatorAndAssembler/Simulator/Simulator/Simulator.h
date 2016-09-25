//
//  Simulator.h
//  Simulator
//
//  Created by 陈志浩 on 16/8/21.
//  Copyright © 2016年 ZhihaoChen. All rights reserved.
//

#ifndef Simulator_h
#define Simulator_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 4*64kb */
#define MEMORYSIZE 262144

/*TRANSLATE*/
#define REG0 ((IR>>24)&0x7)
#define REG1 ((IR>>20)&0x7)
#define REG2 ((IR>>16)&0x7)
#define IMMEDIATE (IR&0xffff)
#define ADDRESS (IR&0xffffff)
#define PORT (IR&0xff)
#define INSTRCODE ((IR>>27)&0x1f)

typedef struct PROG_STATE_WORD {
    unsigned short overflow_flag: 1;
    unsigned short compare_flag: 1;
    unsigned short reserve: 14;
} PROG_STATE_WORD;

/*POINTER TO EXTRA SEGMENT BASE ADDRESS*/
typedef struct EXTRA_STACK {
    PROG_STATE_WORD PSW;
    unsigned int * PC;
    short GR[8];
} EXTRA_STACK;

/*POINTER TO MEMORY 0x0*/
unsigned char * MEM;

//SEGMENT REGISTER
unsigned short CS;
unsigned short DS;
unsigned short SS;
unsigned short ES;
//PC REGISTER
unsigned int * PC;
//IR REGISTER
unsigned int IR;
//SP REGISTER
unsigned short * SP;
//POINTER TO ES
EXTRA_STACK * ESP;
//GENERAL REGISTER
short GR[8];
//PSW
PROG_STATE_WORD PSW;

//FILE POINTER
FILE * fpIN;

//HEADER INFORMATION
unsigned short code_size;
unsigned short data_size;

//INSTRUCTIONS
int HLT(void);
int JMP(void);
int CJMP(void);
int OJMP(void);
int CALL(void);
int RET(void);
int PUSH(void);
int POP(void);
int LOADB(void);
int LOADW(void);
int STOREB(void);
int STOREW(void);
int LOADI(void);
int NOP(void);
int IN(void);
int OUT(void);
int ADD(void);
int ADDI(void);
int SUB(void);
int SUBI(void);
int MUL(void);
int DIV(void);
int AND(void);
int OR(void);
int NOR(void);
int NOTB(void);
int SAL(void);
int SAR(void);
int EQU(void);
int LT(void);
int LTE(void);
int NOTC(void);

//NECESSARY FUNCTION DECLEARATIONS
void init(void);
void load_code(void);
void load_data(void);
void exec(void);

/*ASSEMBLE PROGRAM STATE*/
int ret = 1;

/*FUNCTION POINTER ARRAY*/
int (* instrs[])(void)={HLT, JMP, CJMP, OJMP, CALL, RET, PUSH, POP, LOADB, LOADW, STOREB, STOREW, LOADI, NOP, IN, OUT, ADD, ADDI, SUB, SUBI, MUL, DIV, AND, OR, NOR, NOTB, SAL, SAR, EQU, LT, LTE, NOTC};

#endif /* Simulator_h */
