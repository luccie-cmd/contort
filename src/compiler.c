#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "mystring.h"
#include "ast.h"
#include "error.h"

bool has_in_file = false;
bool print_ast = false;
char* out_file = (char*)NULL; 
char* in_file  = (char*)NULL;

static void parse_command_line_arguments(int argc, char** argv){
    int i = 0;
    while(i < argc){
        char* argument = argv[i];
        if(strcmp(argument, "-o") == 0){
            if(i == argc){
                ERROR(ERROR_SEVERITY_FATAL, "Expected argument after `-o`!\n%s", "");
                exit(1);
            }
            i++;
            out_file = argv[i];
        } else if(strcmp(argument, "--print-ast") == 0){
            print_ast = true;
        } else{
            if(has_in_file){
                ERROR(ERROR_SEVERITY_WARNING, "Cannot parse more then 1 input file!\n%s", "");
            }
            else{
                i++;
                in_file = argv[i];
                has_in_file = true;
            }
        }
        ++i;
    }
}

int main(int argc, char *argv[]){
    parse_command_line_arguments(argc, argv);
    if(!has_in_file){
        ERROR(ERROR_SEVERITY_FATAL, "No file provided for compiler!\n%s", "");
        exit(1);
    }
    if(out_file == NULL){
        out_file = "./a.cn";
    }
    uint64_t len;
    String file_data = string_slurp_file(in_file);
    Lexer lexer = lexer_init(file_data);
    Token *toks = lexer_lex_file(&lexer, &len);
    Ast ast = ast_from_tokens(toks, (size_t)len, &lexer);
    ast_parse_Ast(&ast);
    ast_compile(&ast, out_file);
    if(print_ast) ast_print(&ast);
    free(ast.nodes);
    free(toks);
    free(lexer.def_ops);
    free(file_data.data);
    return 0;
}