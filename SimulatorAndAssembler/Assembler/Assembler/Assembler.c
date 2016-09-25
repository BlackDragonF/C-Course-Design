//
//  main.c
//  Assembler
//
//  Created by 陈志浩 on 16/8/15.
//  Copyright © 2016年 ZhihaoChen. All rights reserved.
//

/*
 *The assembler mainly contains two phrases-Phrase 1 and Phrase 2.
 *In each phrase, the assembler will only scan the source code on-
 *ce.In Phrase 1, the assembler will allocate address for each in-
 *struction and symbol as well as using linked list to manage inf-
 *ormation about symbol and label. In Phrase 2, the assembler will
 *translate from source code to target code.
 */

/*Each segment in simulator is 16-bit*/

/*
 *The output file will contain the following data -
 *----------------------Header----------------------
 *code segment base address - 16bit default 0x0000
 *code size - 16bit
 *data segment base address - 16bit default 0x1000
 *data size - 16bit
 *stack segment base address - 16bit default 0x2000
 *extra segment base address - 16bit default 0x3000
 *----------------------Source----------------------
 *code...
 *...
 *data...
 *...
 */

/*And the simulator will allocate memory according to the header*/

#include "Assembler.h"

/*ASSEMBLER ENTRY*/
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
int main(int argc, const char * argv[])
{
    //arguments handler
    if (argc < 3)
    {
        printf("ERROR:too few arguments!\n");
        return 1;
    }
    //open files
    if ((fpIN = fopen(argv[1], "r")) == NULL)
    {
        printf("ERROR:cannot open file %s for reading!\n", argv[1]);
        return 1;
    }
    if ((fpOUT = fopen(argv[2], "wb")) == NULL)
    {
        printf("ERROR:cannot open file %s for writing!\n", argv[2]);
        return 1;
    }
    
    //initialize linked lists
    label_head = malloc(sizeof(Label));
    label_head->next = NULL;
    symbol_head = malloc(sizeof(Symbol));
    symbol_head->next = NULL;

    //create and open temp file
    char temp_name[100];
    strcpy(temp_name, argv[2]);
    strncat(temp_name, ".temp", 5);
    
    if ((fpTMP = fopen(temp_name, "w")) == NULL)
    {
        printf("ERROR:failed to create temp file %s!\n", temp_name);
        return 1;
    }
    
    //phrase 1
    code_analysis();
    
    //close input file and temp file
    fclose(fpTMP);
    fclose(fpIN);
    
    //reopen temp file
    if ((fpTMP = fopen(temp_name, "r")) == NULL)
    {
        printf("ERROR:failed to load temp file %s!\n", temp_name);
        return 1;
    }
    
    //phrase 2
    code_synthesis();
    
    //close files
    fclose(fpTMP);
    fclose(fpOUT);
    
    return 0;
}

/*PHRASE 1-CODE ANALYSIS*/
/*************************************************
 Function: code_analysis
 Description: phrase 1
 Input: void
 Output: none
 Return: void
 Others: scan input file once and mainly analyse labels,
 symbols and annotations.
 *************************************************/
void code_analysis(void)
{
    char line[MAX_LINE];
    char label[34], instr[8];
    char * cpPOS;
    
    int result;
    
    Label * label_pos = label_head;
    Symbol * symbol_pos = symbol_head;
    
    while (!feof(fpIN))
    {
        fgets(line, MAX_LINE, fpIN);
        //clear annotations
        if((cpPOS = strchr(line, '#')) != NULL)
        {
            *cpPOS = '\0';
        }
        //skip empty lines
        if ((result = sscanf(line, "%s", label)) < 1)
        {
            continue;
        }
        //label handler
        if ((cpPOS = strchr(label, ':')) != NULL)
        {
            char * cpINSTR = cpPOS+1;
            while(isspace(*cpINSTR) == 0)
            {
                cpINSTR++;
            }
            
            fprintf(fpTMP, "%s\n", cpINSTR);
            
            *cpPOS = '\0';
            label_pos->next = malloc(sizeof(Label));
            label_pos = label_pos->next;
            label_pos->address = code_address;
            strcpy(label_pos->identifier,label);
            label_pos->next = NULL;
            
            code_address+=4;
        }
        else
        {
            strcpy(instr, label);
            //WORD and BYTE handlers
            if (strcmp(instr, "WORD") == 0)
            {
                char value[MAX_LINE];
                char number_string[MAX_LINE];
                char identifier[MAX_LINE];
                
                sscanf(line, "%s%s", instr, identifier);
                
                int number;
                
                symbol_pos->next = malloc(sizeof(Symbol));
                symbol_pos = symbol_pos->next;
                symbol_pos->size = 2;
                symbol_pos->base_address = data_address;
                //match {} -> match [] -> match = -> default
                //data blocks with initial value -> data blocks without initial value -> data with ininial value -> data without initial value
                if ((strchr(line, '{') != NULL) && (strrchr(line, '}') != NULL))
                {
                    int count;
                    short * target;
                    
                    strncpy(value, strchr(line, '{')+1, strrchr(line, '}')-strchr(line, '{')-1);
                    value[strrchr(line, '}')-strchr(line, '{')-1] = '\0';
                    
                    strncpy(number_string, strchr(line, '[')+1, strrchr(line, ']')-strchr(line, '[')-1);
                    number_string[strrchr(line, ']')-strchr(line, ']')-1] = '\0';
                    number = atoi(number_string);
                    
                    symbol_pos->number = number;
                    
                    *strchr(identifier, '[') = '\0';
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = malloc(sizeof(short) * number);
                    target = (short *)symbol_pos->value;
                    for(count = 0 ; count < number ; count++)
                    {
                        strncpy(number_string, strrchr(value, ',')+1, strrchr(value, '\0')-strrchr(value, ',')-1);
                        *target = atoi(number_string);
                        target++;
                    }
                }
                else if ((strchr(line, '[') != NULL) && (strrchr(line, ']') != NULL))
                {
                    strncpy(number_string, strchr(line, '[')+1, strrchr(line, ']')-strchr(line, '[')-1);
                    number_string[strrchr(line, ']')-strchr(line, ']')-1] = '\0';
                    number = atoi(number_string);
                    
                    symbol_pos->number = number;
                    
                    *strchr(identifier, '[') = '\0';
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = NULL;
                }
                else if (strchr(line, '=') != NULL)
                {
                    symbol_pos->number = 1;
                    
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = malloc(sizeof(short));
                    sscanf(line, "%*s%*s%*s%s", number_string);
                    *(short *)symbol_pos->value = atoi(number_string);
                }
                else
                {
                    symbol_pos->number = 1;
                    
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = NULL;
                }
                data_address += symbol_pos->number*2;
            }
            else if (strcmp(instr, "BYTE") == 0)
            {
                char value[MAX_LINE];
                char number_string[MAX_LINE];
                char identifier[MAX_LINE];
                
                sscanf(line, "%s%s", instr, identifier);
                
                int number;
                
                symbol_pos->next = malloc(sizeof(Symbol));
                symbol_pos = symbol_pos->next;
                symbol_pos->size = 1;
                symbol_pos->base_address = data_address;
                symbol_pos->next = NULL;
                //similar with WORD handler-> addtionally matches ""
                if ((strchr(line, '{') != NULL) && (strrchr(line, '}') != NULL))
                {
                    int count;
                    char * target;
                    
                    strncpy(value, strchr(line, '{')+1, strrchr(line, '}')-strchr(line, '{')-1);
                    value[strrchr(line, '}')-strchr(line, '{')-1] = '\0';
                    
                    strncpy(number_string, strchr(line, '[')+1, strrchr(line, ']')-strchr(line, '[')-1);
                    number_string[strrchr(line, ']')-strchr(line, ']')-1] = '\0';
                    number = atoi(number_string);
                    
                    symbol_pos->number = number;
                    
                    *strchr(identifier, '[') = '\0';
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = malloc(sizeof(char) * number);
                    target = (char *)symbol_pos->value;
                    for(count = 0 ; count < number ; count++)
                    {
                        strncpy(number_string, strrchr(value, ',')+1, strrchr(value, '\0')-strrchr(value, ',')-1);
                        *target = atoi(number_string);
                        target++;
                    }
                }
                else if ((strchr(line, '"') != NULL) && (strrchr(line, '"') != NULL) && (strrchr(line, '"') != strchr(line, '"')))
                {
                    int count;
                    char * target;
                    
                    strncpy(value, strchr(line, '"')+1, strrchr(line, '"')-strchr(line, '"')-1);
                    value[strrchr(line, '"')-strchr(line, '"')-1] = '\0';
                    
                    strncpy(number_string, strchr(line, '[')+1, strchr(line, ']')-strchr(line, '[')-1);
                    number_string[strchr(line, ']')-strchr(line, ']')-1] = '\0';
                    number = atoi(number_string);
                    
                    symbol_pos->number = number;
                    
                    *strchr(identifier, '[') = '\0';
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = malloc(sizeof(char) * number);
                    target = (char *)symbol_pos->value;
                    for(count = 0 ; count < number ; count++)
                    {
                        strncpy(number_string, strrchr(value, ',')+1, strrchr(value, '\0')-strrchr(value, ',')-1);
                        *target = atoi(number_string);
                        target++;
                    }
                }
                else if ((strchr(line, '[') != NULL) && (strrchr(line, ']') != NULL))
                {
                    strncpy(number_string, strchr(line, '[')+1, strrchr(line, ']')-strchr(line, '[')-1);
                    number_string[strrchr(line, ']')-strchr(line, ']')-1] = '\0';
                    number = atoi(number_string);
                    
                    symbol_pos->number = number;
                    
                    *strchr(identifier, '[') = '\0';
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = NULL;
                }
                else if (strchr(line, '=') != NULL)
                {
                    symbol_pos->number = 1;
                    
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = malloc(sizeof(char));
                    sscanf(line, "%*s%*s%*s%s", number_string);
                    *(char *)symbol_pos->value = atoi(number_string);
                }
                else
                {
                    symbol_pos->number = 1;
                    
                    strcpy(symbol_pos->identifier, identifier);
                    
                    symbol_pos->value = NULL;
                }
                data_address += symbol_pos->number;
            }
            else
            {
                fprintf(fpTMP, "%s\n", line);
                code_address+=4;
            }
        }
    }
}
/*PHRASE 2-CODE SYNTHESIS*/
/*************************************************
 Function: code synthesis
 Description: phrase 2
 Input: void
 Output: none
 Return: void
 Others: scan temp file once, mainly translate from
 instructions to codes, contain three sub-procedures.
 *************************************************/
void code_synthesis(void)
{
    //PHRASE 2 HAS THREE SUB PROCEDURES
    //GENERATE HEADER
    header_generate();
    //GENERATE CODE
    code_generate();
    //GENERATE DATA
    data_generate();
}

/*************************************************
 Function: header_generate
 Description: generate the header of output file
 Input: void
 Output: none
 Return: void
 Others: generate the header of the output file,
 the header of output file mainly contains code size
 and data size.
 *************************************************/
void header_generate(void)
{
    unsigned short cs = CS_BASE_ADDRESS, ds = DS_BASE_ADDRESS, ss = SS_BASE_ADDRESS, es = ES_BASE_ADDRESS;
    fwrite(&cs, sizeof(unsigned short), 1, fpOUT);
    fwrite(&code_address, sizeof(unsigned short), 1, fpOUT);
    
    fwrite(&ds, sizeof(unsigned short), 1, fpOUT);
    fwrite(&data_address, sizeof(unsigned short), 1, fpOUT);
    
    fwrite(&ss, sizeof(unsigned short), 1, fpOUT);
    
    fwrite(&es, sizeof(unsigned short), 1, fpOUT);
}

/*************************************************
 Function: code_generate
 Description: generate the code of output file
 Input: void
 Output: none
 Return: void
 Others: translate from instructions to codes by
 calling trans_to_code function.
 *************************************************/
void code_generate(void)
{
    char line[MAX_LINE];
    char instr[8];
    char * cpPOS;
    
    int result;
    
    unsigned int target_code;
    
    
    while (fgets(line, MAX_LINE, fpTMP) != 0)
    {
        if((cpPOS = strchr(line, '#')) != NULL)
        {
            *cpPOS = '\0';
        }
        
        if ((result = sscanf(line, "%s", instr) < 1))
        {
            continue;
        }
        
        target_code = trans_to_code(line);
        fwrite(&target_code, sizeof(unsigned int), 1, fpOUT);
    }
}

/*************************************************
 Function: trans_to_code
 Description: translate from instructions to codes
 Input: char * pointing to the start of the instruction
 Output: ERROR messages
 Return: unsigned int stands the code(4 byte)
 Others: instructions can mainly divided to 8 types.
 *************************************************/
unsigned int trans_to_code(char * line)
{
    char instr[8];
    char label[34], symbol[34];
    char reg0[8], reg1[8], reg2[8];
    
    int addr;
    int arg0, arg1, arg2;
    
    int immed, port;
    
    int result;
    
    result = sscanf(line, "%s", instr);
    
    int instr_code = get_instr_code(instr);
    if (instr_code > 31)
    {
        printf("ERROR:%s is a invalid instruction!\n", line);
        exit(-1);
    }
    
    unsigned int target_code = 0;
 
    switch (instr_format[instr_code])
    {
        case '1': /*HLT RET NOP NOTC*/
            target_code = instr_code<<27;
            break;
        case '2': /*JMP CJMP OJMP CALL*/
            result = sscanf(line, "%s%s", instr, label);
            if (result < 2)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            Label * target_label = label_with_identifier(label_head, label);
            if (target_label == NULL)
            {
                printf("ERROR:undefined label!\n");
                exit(-1);
            }
            addr = target_label->address;
            target_code = (instr_code<<27)|(addr&0x00ffffff);
            break;
        case '3': /*PUSH POP*/
            result = sscanf(line, "%s%s", instr, reg0);
            if (result < 2)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            arg0 = get_reg_number(line, reg0);
            target_code = (instr_code<<27)|(arg0<<24);
            break;
        case '4': /*LOADB LOADW STOREB STOREW*/
            result = sscanf(line, "%s%s%s", instr, reg0, symbol);
            if (result < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            
            Symbol * target_symbol = symbol_with_identifier(symbol_head, symbol);
            if (target_symbol == NULL)
            {
                printf("ERROR:undefined symbol!\n");
                exit(-1);
            }
            addr = target_symbol->base_address;
            arg0 = get_reg_number(line, reg0);
            target_code = (instr_code<<27)|(arg0<<24)|(addr&0x00ffffff);
            break;
        case '5': /*LOADI ADDI SUBI*/
            result = sscanf(line, "%s%s%d", instr, reg0, &immed);
            if (result < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            arg0 = get_reg_number(line, reg0);
            target_code = (instr_code<<27)|(arg0<<24)|(immed&0x0000ffff);
            break;
        case '6': /*IN OUT*/
            result = sscanf(line, "%s%s%d", instr, reg0, &port);
            if (result < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            arg0 = get_reg_number(line, reg0);
            target_code = (instr_code<<27)|(arg0<<24)|(port&0x000000ff);
            break;
        case '7': /*ADD SUB MUL DIV AND OR NOR SAL SAR*/
            result = sscanf(line, "%s%s%s%s", instr, reg0, reg1, reg2);
            if (result < 4)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            arg0 = get_reg_number(line, reg0);
            arg1 = get_reg_number(line, reg1);
            arg2 = get_reg_number(line, reg2);
            target_code = (instr_code<<27)|(arg0<<24)|(arg1<<20)|(arg2<<16);
            break;
        case '8': /*NOTB EQU LT LTE*/
            result = sscanf(line, "%s%s%s", instr, reg0, reg1);
            if (result < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            arg0 = get_reg_number(line, reg0);
            arg1 = get_reg_number(line, reg1);
            target_code = (instr_code<<27)|(arg0<<24)|(arg1<<20);
            break;
    }
    return target_code;
}

/*************************************************
 Function: get_instr_code
 Description: return the code of instruction based
 on the instruction.
 Input: const char * points to the start of instrution
 Output: none
 Return: int stants the code of the instruction.
 Others: one example: HLT-00000
 *************************************************/
int get_instr_code(const char * instr)
{
    int count;
    for (count = 0 ; count < INSTRS_COUNT ; count++)
    {
        if(strcmp(instrs_name[count], instr) == 0)
        {
            break;
        }
    }
    return count;
}

/*************************************************
 Function: get_reg_number
 Description: get register number based on the 
 register character.
 Input:
 char * points to the start of the instruction line
 char * points to the register string
 Output: ERROR messages
 Return: int stands the code of the register number
 Others: one example: A-001
 *************************************************/
int get_reg_number(char * line, char * reg)
{
    int number;
    if (tolower(*reg) == 'z')
    {
        number = 0;
    }
    else if ((tolower(*reg) >='a') && (tolower(*reg) <= 'g'))
    {
        number = tolower(*reg)-'a'+1;
    }
    else
    {
        printf("ERROR:bad register name in %s!\n", line);
        exit(-1);
    }
    return number;
}

/*************************************************
 Function: label_with_identifier
 Description: get label node based on specific identifier
 Input:
 Label * points to the head of the label linked list
 const char * points to the start of the identifier string
 Output: none
 Return: Label * stands the specific node
 Others: none
 *************************************************/
Label * label_with_identifier(Label * head, const char * identifier)
{
    Label * node = head;
    while (node != NULL)
    {
        if (strcmp(node->identifier, identifier) == 0)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

/*************************************************
 Function: symbol_with_identifier
 Description: get symbol node based on specific identifier
 Input:
 Symbol * points to the head of the symbol linked list
 const char * points to the start of the identifier string
 Output: none
 Return: Symbol * stands the specific node
 Others: none
 *************************************************/
Symbol * symbol_with_identifier(Symbol * head, const char *identifier)
{
    Symbol * node = head->next;
    while (node != NULL)
    {
        if (strcmp(node->identifier, identifier) == 0)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

/*************************************************
 Function: data_generate
 Description: generate the data of the output file
 Input: void
 Output: none
 Return: void
 Others: generate the data of the output file
 *************************************************/
void data_generate(void)
{
    Symbol * node = symbol_head->next;
    
    unsigned char empty_byte = 0;
    
    int count;
    
    while (node != NULL)
    {
        //data with initial value -> data without initial value -> data block with initial values -> data block without initial values.
        if ((node->number == 1) && (node->value != NULL))
        {
            fwrite(node->value, node->size, 1, fpOUT);
        }
        else if ((node->number == 1) && (node->value == NULL))
        {
            for (count = 0 ; count < node->size ; count++)
            {
                fwrite(&empty_byte, 1, 1, fpOUT);
            }
        }
        else if ((node->number != 1) && (node->value != NULL))
        {
            fwrite(node->value, node->size * node->number, 1, fpOUT);
        }
        else
        {
            for (count = 0 ; count < node->size * node->number ; count++)
            {
                fwrite(&empty_byte, 1, 1, fpOUT);
            }
        }
        node = node->next;
    }
}
