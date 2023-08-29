#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "mystring.h"

int main(){
    uint64_t len;
    String file_data = string_slurp_file("./build/main.con");
    Lexer lexer = lexer_init(file_data);
    Token *toks = lexer_lex_file(&lexer, &len);
    free(toks);
    free(file_data.data);
    return 0;
}