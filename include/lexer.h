#pragma once
#include <stddef.h>
#include <mystring.h>
#include "token.h"

typedef struct{
    String name;
    size_t index_in_ast_nodes;
} Deferred_Operand;

typedef struct{
    Deferred_Operand *def_ops;
    size_t def_ops_len;
    String data;
    size_t index_data;
} Lexer;

Lexer lexer_init(String data);
Token lexer_advance(Lexer *lexer);
Token *lexer_lex_file(Lexer *lexer, uint64_t *len);