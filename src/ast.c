#include "ast.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>

Ast ast_from_tokens(Token * tokens, size_t size, Lexer *lexer){
    Ast ast;
    ast.tokens = tokens;
    ast.tokens_size = size;
    ast.nodes = malloc(sizeof(Ast_Node));
    ast.nodes_size = 0;
    ast.lexer = lexer;
    return ast;
}

void ast_parse_Ast(Ast *ast){
    String *names = malloc(sizeof(String));
    size_t namesi = 0;
    for(uint64_t k = 0; k < ast->tokens_size; ++k){
        if(ast->tokens[k].type == TOKEN_TYPE_DIRECTIVE && ast->tokens[k].as.dir.type == TOKEN_DIR_TYPE_LABEL){
            names[namesi++] = ast->tokens[k].as.dir.as.label.name;
            names = realloc(names, (namesi+1)*sizeof(String));
        }
    }
    for(size_t i = 0; i < ast->tokens_size; ++i){
        switch(ast->tokens[i].type){
            case TOKEN_TYPE_DIRECTIVE: {
                switch(ast->tokens[i].as.dir.type){
                    case AST_DIR_TYPE_LABEL: {
                        ast->nodes[ast->nodes_size].type = AST_NODE_TYPE_DIR;
                        ast->nodes[ast->nodes_size].as.dir.type = AST_DIR_TYPE_LABEL;
                        ast->nodes[ast->nodes_size].as.dir.as.label.name = ast->tokens[i].as.dir.as.label.name;
                        ast->nodes[ast->nodes_size].as.dir.as.label.jmp_addr.byte = (uint8_t)ast_GetInstsLen(ast);
                        ast->nodes_size++;
                        ast->nodes = realloc(ast->nodes, (ast->nodes_size+1)*sizeof(Ast_Node));
                    } break;
                }
            } break;
            case TOKEN_TYPE_INST: {
                ast->nodes[ast->nodes_size].type = AST_NODE_TYPE_INST;
                ast->nodes[ast->nodes_size].as.inst.type = ast->tokens[i].as.inst.type;
                ast->nodes[ast->nodes_size].as.inst.operand = ast->tokens[i].as.inst.operand;
                ast->nodes_size++;
                ast->nodes = realloc(ast->nodes, (ast->nodes_size+1)*sizeof(Ast_Node));
            } break;
        }
    }
    for(size_t i = 0; i < namesi; ++i){
        for(size_t j = 0; j < ast->lexer->def_ops_len; ++j){
            if(string_eq(names[i], ast->lexer->def_ops[j].name)){
                for(size_t l = 0; l < ast->nodes_size; ++l){
                    if(string_eq(names[i], ast->nodes[l].as.dir.as.label.name)){
                        ast->nodes[ast->lexer->def_ops[j].index_in_ast_nodes].as.inst.operand = ast->nodes[l].as.dir.as.label.jmp_addr;
                        break;
                    }
                }
            }
        }
    }
    free(names);
}

void ast_compile(Ast *ast, char* out_file){
    FILE* f = fopen(out_file, "wb");
    for(size_t i = 0; i < ast->nodes_size; ++i){
        if(ast->nodes[i].type == AST_NODE_TYPE_INST){
            switch(ast->nodes[i].as.inst.type){
                case TOKEN_INST_PUSH: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                    if(ast->nodes[i].as.inst.operand.byte <= 255){
                        uint8_t bytes_to_read = 1;
                        fwrite(&bytes_to_read, bytes_to_read, 1, f);
                    } else if(ast->nodes[i].as.inst.operand.word <= 65535){
                        uint8_t bytes_to_read = 2;
                        fwrite(&bytes_to_read, bytes_to_read, 1, f);
                    }
                    // Word u64_op = ast->nodes[i].as.inst.operand;
                    // fwrite(&u64_op, sizeof(u64_op), 1, f);
                } break;
                case TOKEN_INST_JMP: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                    Word u64_op = ast->nodes[i].as.inst.operand;
                    fwrite(&u64_op, sizeof(u64_op), 1, f);
                } break;
                case TOKEN_INST_JMP_IF: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                    Word u64_op = ast->nodes[i].as.inst.operand;
                    fwrite(&u64_op, sizeof(u64_op), 1, f);
                } break;
                case TOKEN_INST_ADD: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_SUB: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_MUL: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_DIV: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_HLT: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_EQU: {
                    uint8_t bitEqu = Token_Inst_bitEquivalent(ast->nodes[i].as.inst.type);
                    fwrite(&bitEqu, 1, 1, f);
                } break;
                case TOKEN_INST_UNDEFINED:
                default: {
                    ERROR(ERROR_SEVERITY_FATAL, "Invalid instruction type\n%s", "");
                    exit(1);
                } break;
            }
            int inst_end = 0xFF;
            fwrite(&inst_end, 1, 1, f);
        } else{
            if(ast->nodes[i].type != AST_NODE_TYPE_DIR){
                ERROR(ERROR_SEVERITY_FATAL, "Invalid node type!\n%s", "");
                exit(1);
            }
        }
    }
    fclose(f);
}


void ast_print(const Ast *ast){
    printf("AST:\n");
    for(size_t i = 0; i < ast->nodes_size; ++i){
        if(ast->nodes[i].type == AST_NODE_TYPE_DIR){
            ERROR(ERROR_SEVERITY_INFO, "    {type: dir, {label: {name: %.*s, jmp addr: %d}}\n", (int)ast->nodes[i].as.dir.as.label.name.count, ast->nodes[i].as.dir.as.label.name.data, ast->nodes[i].as.dir.as.label.jmp_addr.byte);
        } else if(ast->nodes[i].type == AST_NODE_TYPE_INST){
            ERROR(ERROR_SEVERITY_INFO, "    {type: inst, {type: %s, operand: %d}}\n", token_inst_as_cstr(ast->nodes[i].as.inst.type), ast->nodes[i].as.inst.operand.byte);
        }
    }
}

uint64_t ast_GetInstsLen(const Ast *ast){
    uint64_t insts = 0;
    for(size_t i = 0; i < ast->nodes_size; ++i){
        if(ast->nodes[i].type == AST_NODE_TYPE_INST){
            insts++;
        }
    }
    return insts;
}