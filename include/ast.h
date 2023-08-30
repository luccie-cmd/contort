#pragma once
#include "token.h"
#include "lexer.h"
#include <stddef.h>

typedef struct{
    Token_Inst_Type type;
    Word operand;
} Ast_Node_Inst;

typedef struct{
    Word jmp_addr;
    String name;
} Ast_Dir_Label;

typedef struct{
    union{
        Ast_Dir_Label label;
    } as;
    enum{
        AST_DIR_TYPE_LABEL=0,
    } type;
} Ast_Node_Dir;

typedef enum{
    AST_NODE_TYPE_INST,
    AST_NODE_TYPE_DIR,
} Ast_Node_Type;

typedef struct{
    union{
        Ast_Node_Inst inst;
        Ast_Node_Dir dir;
    } as;
    Ast_Node_Type type;
} Ast_Node;

typedef struct{
    Token *tokens;
    size_t tokens_size;
    Ast_Node *nodes;
    size_t nodes_size;
    const Lexer *lexer;
} Ast;

Ast ast_from_tokens(Token *tokens, size_t size, const Lexer *lexer);
Word ast_GetInstsLen(const Ast *ast);
void ast_parse_Ast(Ast *ast);
void ast_compile(Ast *ast, char* out_file);
void ast_print(const Ast *ast);