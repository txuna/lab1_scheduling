#include "common.h"

void scheduling(Init* init){
    Resource* res = (Resource*)malloc(sizeof(Resource));
    memset(res, 0, sizeof(Resource));
    switch(init->select){
        case 1:
            FIFO(init, res);
            break; //
        
        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        default:
            error_handler("Hmm... other scheduling number isn't exists!");
            break;
    }

    free(res);
}

//먼저 들어온 큐에 대해 리로스 선점
//Arrive Time이 가장 빠른것 부터 처리 
void FIFO(Init* init, Resource* res){
    //test
    AST* tmp_ast = init->parse_tree_list[0];
    while(tmp_ast){
        execute_ins(tmp_ast, res);
        show_resource(res);
        tmp_ast = tmp_ast->next_instruction;
    }
}

//처리중인 프로세스가 없다면 대기큐에 있는 프로세스중에서 가장 짝은 서비스타임을 가진 프로세스를 처리한다. 
//만약 처리중인 프로세스가 있다면 해당 프로세스를 완전히 처리한 후 짧은 거 선택
void SJF(Init* init, Resource* res){

}

void RR(Init* init, Resource* res){

}

void MLFQ(Init* init, Resource* res){

}

void show_resource(Resource* res){
    printf("===============REG=================\n");
    printf("A regs : %d\n", res->regs.A);
    printf("B regs : %d\n", res->regs.B);
    printf("C regs : %d\n", res->regs.C);
    printf("D regs : %d\n", res->regs.D);
}