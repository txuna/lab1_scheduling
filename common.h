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



typedef struct _RegisterInfo{
    int A; 
    int B;
    int C;
    int D;
}RegisterInfo;

// 명령어를 실행할 때 의 결과를 저장, 하나만 사용 이거를 전역으로 쓸까 고민중
typedef struct _Resource{
    RegisterInfo regs;
    int current_ip; //몇 번째 줄의 명령어를 가리키고 있는지
}Resource;

//스케쥴링 될 때 해당 프로세스의 리소스 자원
typedef struct _PCB{
    Resource res;
    int process_number;
}PCB;

typedef struct _Queue{
    int process_number;
    struct _Queue* next;
}Queue;

//해당 큐의 우선순위와 큐의 끝과 처음을 가리킨다. 
typedef struct _QueueManager{
    Queue* head; 
    Queue* tail;
    int priority; //해당 큐에 대한 우선순위 
}QueueManager;

//큐들의 배열이다. 
typedef struct _QueueList{
    Queue* queue_list; //Multi Level Feedback Queue에 대한 대비 이 스케쥴링이 아닐경우 단일로 사용
}QueueList;

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
void execute_ins(AST* ast_ins, Resource* res);

//scheduling
void scheduling(Init* init);

void FIFO(Init* init, Resource* res);
void SJF(Init* init, Resource* res);
void RR(Init* init, Resource* res);
void MLFQ(Init* init, Resource* res);

#endif 