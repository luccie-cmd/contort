#pragma once
#include <stdint.h>
#include "mystring.h"

typedef union{
    uint8_t byte;
    uint16_t word;
} Word;

typedef enum{
    TOKEN_INST_UNDEFINED=0,
    TOKEN_INST_PUSH,
    TOKEN_INST_ADD,
    TOKEN_INST_SUB,
    TOKEN_INST_MUL,
    TOKEN_INST_DIV,
    TOKEN_INST_EQU,
    TOKEN_INST_JMP,
    TOKEN_INST_JMP_IF,
    TOKEN_INST_HLT,
} Token_Inst_Type;

typedef enum{
    TOKEN_TYPE_INST,
    TOKEN_TYPE_DIRECTIVE,
} Token_Type;

typedef struct{
    Token_Inst_Type type;
    Word operand;
} Token_Inst;

typedef struct{
    String name;
    uint64_t jmp_addr;
} Token_Dir_Label;

typedef struct{
    union{
        Token_Dir_Label label;
    } as;
    enum{
        TOKEN_DIR_TYPE_LABEL=0,
    } type;
} Token_Dir;

typedef struct{
    union{
        Token_Inst inst;
        Token_Dir dir;
    } as;
    Token_Type type;
} Token;

uint8_t Token_Inst_bitEquivalent(Token_Inst_Type type);
char* token_inst_as_cstr(Token_Inst_Type type);