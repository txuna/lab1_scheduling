#ifndef __PARSE_H_
#define __PARSE_H_

#include "common.h"

#define MOV 0 
#define SUB 1
#define ADD 2

#define REG 1
#define VALUE 2

typedef struct _Node{
    int type; // REG or VALUE
    union{
        int value;
        char reg;
    };
}Node;

typedef struct _AST{
    unsigned int op_type;
    Node left;
    Node right;
    struct _AST* next_instruction;
}AST;

void parse(char* ins);
AST* loop_about_semicolon(char* ins);
AST* parse_ins(char* ins);
AST* set_ast(char* op, char* left, char* right);

void set_right(char* right, AST* node);
void set_left(char* left, AST* node);
void set_op(char* op, AST* node);

void dump(AST* ast);
AST* get_end_list(AST* ast);
#endif