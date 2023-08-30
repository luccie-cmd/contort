#include "token.h"
#include <stddef.h>

uint8_t Token_Inst_bitEquivalent(Token_Inst_Type type){
    switch(type){
        case TOKEN_INST_PUSH: return TOKEN_INST_PUSH;
        case TOKEN_INST_ADD: return TOKEN_INST_ADD;
        case TOKEN_INST_HLT: return TOKEN_INST_HLT;
        case TOKEN_INST_UNDEFINED:
        default: return 0;
    }
}

char * token_inst_as_cstr(Token_Inst_Type type){
    switch(type){
        case TOKEN_INST_PUSH: return "push";
        case TOKEN_INST_ADD: return "add";
        case TOKEN_INST_HLT: return "hlt";
        case TOKEN_INST_UNDEFINED:
        default: return NULL;
    }
}
