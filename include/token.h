#pragma once
#include <stdint.h>

typedef uint64_t Word;

typedef enum{
    TOKEN_INST_UNDEFINED=0,
    TOKEN_INST_PUSH,
    TOKEN_INST_ADD,
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
    char* uwu; // TODO: Directives (labels)
} Token_Dir;

typedef struct{
    union{
        Token_Inst inst;
        Token_Dir dir;
    } as;
    Token_Type type;
} Token;