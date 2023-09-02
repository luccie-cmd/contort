#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "token.h"

static uint8_t *readFile(const char* path, size_t *len){
    FILE *f = fopen(path, "rb");
    if(!f){
        printf("Could not open `%s` no such file or directory\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *buffer = malloc(size);
    fread(buffer, 1, size, f);

    fclose(f);
    *len = size;
    return buffer;
}

static uint64_t reverse_bytes(uint64_t bytes){
    uint64_t result = 0;
    while(bytes!=0){
        result = (result << 8) | (bytes & 0xFF);
        bytes >>= 8;
    }
    return result;
}

typedef struct{
    Token_Inst_Type type;
    Word operand;
} Inst;

#define MAKE_INST(type, argument) ((Inst) {.type = type, .operand = argument});

typedef struct{
    Inst *program;
    Word program_size;
    uint64_t ip;

    Word *stack;
    uint64_t stack_size;

    int halt;
} Executor;

static void executor_add(Executor *exe, Inst inst){
    exe->program[exe->program_size++] = inst;
    exe->program = realloc(exe->program, (exe->program_size+1)*sizeof(Inst));
}

static Executor parse_file(uint8_t *buffer, size_t len){
    Executor executor = {0};
    executor.program = malloc(sizeof(Inst));
    executor.stack = malloc(sizeof(Word));
    size_t i = 0;
    while(i < len){
        Token_Inst_Type type = buffer[i++];
        switch(type){
            case TOKEN_INST_PUSH: {
                Word result = 0;
                while(buffer[i] != 0xFF){
                    uint8_t buffer_t = buffer[i++];
                    result = (result << 8) | buffer_t;
                }
                result = reverse_bytes(result);
                Inst inst = MAKE_INST(type, result);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_JMP: {
                Word result = 0;
                while(buffer[i] != 0xFF){
                    uint8_t buffer_t = buffer[i++];
                    result = (result << 8) | buffer_t;
                }
                result = reverse_bytes(result);
                Inst inst = MAKE_INST(type, result);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_JMP_IF: {
                Word result = 0;
                while(buffer[i] != 0xFF){
                    uint8_t buffer_t = buffer[i++];
                    result = (result << 8) | buffer_t;
                }
                result = reverse_bytes(result);
                Inst inst = MAKE_INST(type, result);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_ADD: {
                Inst inst = MAKE_INST(type, 0);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_SUB: {
                Inst inst = MAKE_INST(type, 0);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_MUL: {
                Inst inst = MAKE_INST(type, 0);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_DIV: {
                Inst inst = MAKE_INST(type, 0);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_EQU: {
                Inst inst = MAKE_INST(type, 0);
                executor_add(&executor, inst);
            } break;
            case TOKEN_INST_HLT: {
                Inst inst = MAKE_INST(type, 0);
                executor_add(&executor, inst);
            } break;

            case TOKEN_INST_UNDEFINED:
            default: {
                printf("No value with %x is parsed yet\n", type);
                exit(1);
            }
        }
        i++;
    }
    return executor;
}

static void executor_run_inst(Executor *exe){
    Inst inst = exe->program[exe->ip];
    switch(inst.type){
        case TOKEN_INST_PUSH: {
            exe->stack[exe->stack_size++] = inst.operand;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
            exe->ip++;
        } break;
        case TOKEN_INST_ADD: {
            exe->stack[exe->stack_size-2] = exe->stack[exe->stack_size-2] + exe->stack[exe->stack_size-1];
            exe->stack_size-=1;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
            exe->ip++;
        } break;
        case TOKEN_INST_SUB: {
            exe->stack[exe->stack_size-2] = exe->stack[exe->stack_size-2] - exe->stack[exe->stack_size-1];
            exe->stack_size-=1;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
            exe->ip++;
        } break;
        case TOKEN_INST_MUL: {
            exe->stack[exe->stack_size-2] = exe->stack[exe->stack_size-2] * exe->stack[exe->stack_size-1];
            exe->stack_size-=1;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
            exe->ip++;
        } break;
        case TOKEN_INST_DIV: {
            exe->stack[exe->stack_size-2] = exe->stack[exe->stack_size-2] / exe->stack[exe->stack_size-1];
            exe->stack_size-=1;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
            exe->ip++;
        } break;
        case TOKEN_INST_EQU: {
            exe->stack[exe->stack_size-2] = exe->stack[exe->stack_size-1] == exe->stack[exe->stack_size-2];
            exe->stack_size-=1;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
            exe->ip++;
        } break;
        case TOKEN_INST_HLT: {
            exe->halt = 1;
            exe->ip++;
        } break;
        case TOKEN_INST_JMP_IF: {
            if(exe->stack[exe->stack_size-1]){
                exe->ip = inst.operand;
            } else{
                exe->ip++;
            }
            exe->stack_size-=1;
            exe->stack = realloc(exe->stack, (exe->stack_size+1)*sizeof(uint64_t));
        } break;
        case TOKEN_INST_JMP: {
            exe->ip = inst.operand;
        } break;
        case TOKEN_INST_UNDEFINED:
        default: {
            printf("%s is not handled yet\n", token_inst_as_cstr(inst.type));
            exit(1);
        } break;
    }
}

static void executor_print_stack(const Executor *exe){
    printf("\nStack:\n");
    for(uint64_t i = 0; i < exe->stack_size; ++i){
        printf("    %ld\n", exe->stack[i]);
    }
    printf("\n");
}

static void executor_run(Executor *exe){
    while(!exe->halt){
        executor_run_inst(exe);
    }
}

int main(){
    size_t len = 0;
    uint8_t* file_buffer = readFile("./a.cn", &len);
    Executor exe = parse_file(file_buffer, len);
    executor_run(&exe);
    executor_print_stack(&exe);

    free(file_buffer);
    free(exe.program);
    free(exe.stack);
    return 0;
}