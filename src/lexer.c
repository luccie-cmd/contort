#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

Lexer lexer_init(String data){
    Lexer lexer;
    lexer.data = data;
    lexer.index_data = 0;
    return lexer;
}

#define CONTORT_COMMENT ';'

Token lexer_advance(Lexer *lexer){
    String line = string_trim(string_chopByDelim(&lexer->data, '\n'));
    while(line.count == 0){
        line = string_trim(string_chopByDelim(&lexer->data, '\n'));
    }
    String opcode = string_trim_left(string_chopByDelim(&line, ' '));
    String argument = string_trim(string_chopByDelim(&line, CONTORT_COMMENT));
    if(*opcode.data == CONTORT_COMMENT && opcode.count == 1){
        return lexer_advance(lexer);
    }

    if(string_eq(opcode, string_from_cstr("push"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.operand = string_to_int(argument), .as.inst.type = TOKEN_INST_PUSH};
    } else if(string_eq(opcode, string_from_cstr("add"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.operand = 0, .as.inst.type = TOKEN_INST_ADD};
    } else if(string_eq(opcode, string_from_cstr("hlt"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.operand = 0, .as.inst.type = TOKEN_INST_HLT};
    }

    fprintf(stderr, "`%.*s` is not a valid operand\n", (int)opcode.count, opcode.data);
    exit(1);
}

Token *lexer_lex_file(Lexer *lexer, uint64_t *len){
    Token *tokenlist = malloc(sizeof(Token));
    uint64_t tokenListLen = 0;
    uint64_t tokenListAllocLen = sizeof(Token);
    while(lexer->data.count > 0){
        Token tok = lexer_advance(lexer);
        if(tokenListAllocLen/sizeof(Token) >= tokenListLen){
            tokenListAllocLen*=(2*sizeof(Token));
            tokenlist = realloc(tokenlist, tokenListAllocLen);
        }
        tokenlist[tokenListLen++] = tok;
    }
    *len = tokenListLen;
    return tokenlist;
}
