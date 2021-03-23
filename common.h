#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


typedef struct _Init{
    int select; 
    int process_numberof; 
    int* arrive_time; //내부 값은 free할 필요 없음 
    char** process_list; //문자열은 free 할 피요 없음 
    AST** parse_tree_list; //내부 리스트 free 필요
}Init;

typedef struct _PCB{

}PCB;

typedef struct _RegisterInfo{
    int A; 
    int B;
    int C;
    int D;
}RegisterInfo;

typedef struct _Resource{
    RegisterInfo regs;
    int current_ip; //몇 번째 줄의 명령어를 가리키고 있는지
}Resource;

//Main
Init* menu();


//Common
char* make_new_string(char* old);
void error_handler(char* msg);

//Parse

void parse(Init* init);
AST* loop_about_semicolon(char* ins);
AST* parse_ins(char* ins);
AST* set_ast(char* op, char* left, char* right);

void set_right(char* right, AST* node);
void set_left(char* left, AST* node);
void set_op(char* op, AST* node);

void dump(AST* ast);
AST* get_end_list(AST* ast);
void free_ast(AST* ast);

//Core
// ins 한줄을 실행 및 실행마다 Resource에 저장
void execute_ins(AST* ast_ins);

//scheduling
void scheduling(Init* init);

#endif 