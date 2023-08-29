#pragma once
#include <stddef.h>
#include <mystring.h>
#include "token.h"

typedef struct{
    String data;
    size_t index_data;
} Lexer;

Lexer lexer_init(String data);
Token lexer_advance(Lexer *lexer);
Token *lexer_lex_file(Lexer *lexer, uint64_t *len);