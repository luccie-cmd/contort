#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

Ast ast_from_tokens(Token * tokens, size_t size){
    Ast ast;
    ast.tokens = tokens;
    ast.tokens_size = size;
    ast.nodes = malloc(sizeof(Ast_Node));
    ast.nodes_size = 0;
    return ast;
}

void ast_parse_Ast(Ast *ast){
    for(size_t i = 0; i < ast->tokens_size; ++i){
        switch(ast->tokens[i].type){
            case TOKEN_TYPE_DIRECTIVE: {
                fprintf(stderr, "Parsing directives is not implemented yet!\n");
                exit(1);
            } break;
            case TOKEN_TYPE_INST: {
                ast->nodes[ast->nodes_size].type = AST_NODE_TYPE_INST;
                ast->nodes[ast->nodes_size].as.inst.type = ast->tokens[i].as.inst.type;
                ast->nodes[ast->nodes_size].as.inst.operand = ast->tokens[i].as.inst.operand;
                ast->nodes_size++;
                ast->nodes = realloc(ast->nodes, ast->nodes_size*2*sizeof(Ast_Node));
            } break;
        }
    }
}

void ast_compile(Ast *ast, char* out_file){
    FILE* f = fopen(out_file, "wb");
    for(size_t i = 0; i < ast->nodes_size; ++i){
        if(ast->nodes[i].type == AST_NODE_TYPE_DIR){
            fprintf(stderr, "Compiling directives is not implemented yet!\n");
            exit(1);
        } else if(ast->nodes[i].type == AST_NODE_TYPE_INST){
            switch(ast->nodes[i].as.inst.type){
                case TOKEN_INST_PUSH: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                    Word u64_op = ast->nodes[i].as.inst.operand;
                    fwrite(&u64_op, sizeof(u64_op), 1, f);
                } break;
                case TOKEN_INST_ADD: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_HLT: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_UNDEFINED:
                default: {
                    fprintf(stderr, "Invalid instruction type\n");
                    exit(1);
                } break;
            }
            int inst_end = 0xFF;
            fwrite(&inst_end, 1, 1, f);
        } else{
            fprintf(stderr, "Invalid node type!\n");
            exit(1);
        }
    }
    fclose(f);
}


void ast_print(Ast ast){
    printf("AST:\n");
    for(size_t i = 0; i < ast.nodes_size; ++i){
        if(ast.nodes[i].type == AST_NODE_TYPE_DIR){
            fprintf(stderr, "Printing directives is not possible yet!\n");
            exit(1);
        } else if(ast.nodes[i].type == AST_NODE_TYPE_INST){
            printf("    {type: inst, {type: %s, operand: %ld}}\n", token_inst_as_cstr(ast.nodes[i].as.inst.type), ast.nodes[i].as.inst.operand);
        }
    }
}
