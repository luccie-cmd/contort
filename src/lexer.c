#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

Lexer lexer_init(String data){
    Lexer lexer;
    lexer.data = data;
    lexer.index_data = 0;
    lexer.def_ops = malloc(sizeof(lexer.def_ops[0]));
    lexer.def_ops_len = 0;
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
    if(opcode.data[0] == CONTORT_COMMENT && opcode.count == 1){
        return lexer_advance(lexer);
    }
    if(opcode.data[opcode.count - 1] == ':'){
        String name = string_trim(string_chopByDelim(&opcode, ':'));
        return (Token) {.type = TOKEN_TYPE_DIRECTIVE, .as.dir.as.label.name = name, .as.dir.type = TOKEN_DIR_TYPE_LABEL};
    }

    if(string_eq(opcode, string_from_cstr("push"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.operand = string_to_int(argument), .as.inst.type = TOKEN_INST_PUSH};
    } else if(string_eq(opcode, string_from_cstr("add"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.type = TOKEN_INST_ADD};
    } else if(string_eq(opcode, string_from_cstr("hlt"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.type = TOKEN_INST_HLT};
    } else if(string_eq(opcode, string_from_cstr("equ"))){
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.type = TOKEN_INST_EQU};
    } else if(string_eq(opcode, string_from_cstr("jmp"))){
        Deferred_Operand def_op;
        def_op.name = argument;
        lexer->def_ops[lexer->def_ops_len++] = def_op;
        lexer->def_ops = realloc(lexer->def_ops, (lexer->def_ops_len+1)*sizeof(Deferred_Operand));
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.type = TOKEN_INST_JMP};
    } else if(string_eq(opcode, string_from_cstr("jmp_if"))){
        Deferred_Operand def_op;
        def_op.name = argument;
        lexer->def_ops[lexer->def_ops_len++] = def_op;
        lexer->def_ops = realloc(lexer->def_ops, (lexer->def_ops_len+1)*sizeof(Deferred_Operand));
        return (Token) {.type = TOKEN_TYPE_INST, .as.inst.type = TOKEN_INST_JMP_IF};
    } 

    fprintf(stderr, "`%.*s` is not a valid instruction\n", (int)opcode.count, opcode.data);
    exit(1);
}

Token *lexer_lex_file(Lexer *lexer, uint64_t *len){
    Token *tokenlist = malloc(sizeof(Token));
    uint64_t tokenListLen = 0;
    while(lexer->data.count > 0){
        Token tok = lexer_advance(lexer);
        if(tok.type == TOKEN_TYPE_INST && (tok.as.inst.type == TOKEN_INST_JMP || tok.as.inst.type == TOKEN_INST_JMP_IF)){
            lexer->def_ops[lexer->def_ops_len-1].index_in_ast_nodes = tokenListLen;
        }
        tokenlist[tokenListLen++] = tok;
        tokenlist = realloc(tokenlist, tokenListLen*2*sizeof(Token));
    }
    *len = tokenListLen;
    return tokenlist;
}
