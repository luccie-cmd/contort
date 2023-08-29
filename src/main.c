#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "mystring.h"

int main(){
    uint64_t len;
    String file_data = string_slurp_file("./build/main.con");
    Lexer lexer = lexer_init(file_data);
    Token *toks = lexer_lex_file(&lexer, &len);
    for(uint64_t i = 0; i < len; ++i){
        printf("%d\n", toks[i].as.inst.type);
    }
    free(toks);
    free(file_data.data);
    return 0;
}