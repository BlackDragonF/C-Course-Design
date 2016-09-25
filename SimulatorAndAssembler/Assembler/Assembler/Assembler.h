//
//  Assembler.h
//  Assembler
//
//  Created by 陈志浩 on 16/8/20.
//  Copyright © 2016年 ZhihaoChen. All rights reserved.
//

#ifndef Assembler_h
#define Assembler_h

/*INCLUDE NECESSARY LIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*MACRO DEFINIATION*/
//define the max characters of one line
#define MAX_LINE 301
//define the number of instructions count
#define INSTRS_COUNT 32

//define base address of four segments
#define CS_BASE_ADDRESS 0x0000
#define DS_BASE_ADDRESS 0x1000
#define SS_BASE_ADDRESS 0x2000
#define ES_BASE_ADDRESS 0x3000

/*LINKED LIST*/
typedef struct Label{
    char identifier[33];
    int address;
    struct Label * next;
}Label;

typedef struct Symbol{
    char identifier[33];
    int size;
    int number;
    unsigned char * value;
    int base_address;
    struct Symbol * next;
}Symbol;

/*HEAD OF LINKED LIST*/
Label * label_head;
Symbol * symbol_head;

/*FILE POINTERS*/
FILE * fpIN, * fpOUT, *fpTMP;

/*CODE AND DATA STASTICS*/
int code_address = 0, data_address = 0;

/*TWO PHRASES*/
void code_analysis(void);
void code_synthesis(void);

/*THREE SUB PROCEDURES OF PHRASE 2*/
void header_generate(void);
void code_generate(void);
void data_generate(void);

//translate from line to code
unsigned int trans_to_code(char * line);

//get instruction code
int get_instr_code(const char * instr);
//get register number based on character
int get_reg_number(char * line, char * reg);

//get label node based on specific identifier
Label * label_with_identifier(Label * head, const char * identifier);
//get symbol node based on specific identifier
Symbol * symbol_with_identifier(Symbol * head, const char *identifier);

/*IMPORTANT CONST*/
//INSTRUCTIONS NAME
const char * instrs_name[] = {"HLT", "JMP", "CJMP", "OJMP", "CALL", "RET", "PUSH", "POP", "LOADB", "LOADW", "STOREB", "STOREW", "LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB", "SUBI", "MUL", "DIV", "AND", "OR", "NOR", "NOTB", "SAL", "SAR", "EQU", "LT", "LTE", "NOTC"};
//INSTRUCTIONS FORMAT
const char instr_format[33] = "12222133444451667575777778778881";

#endif /* Assembler_h */
