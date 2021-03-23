#include "common.h"

/*
각 각의 연산자에 맞게 연산 수행
Right는 무조건 레지스터고 
Left는 레지스터 또는 값임
*/
void execute_ins(AST* ast_ins, Resource* res){

    // 명령어의 오른쪽 값을 받아옴 
    //int right_value = get_value(ast_ins->right, res);
    if(ast_ins->op_type == MOV){
        int right_value = get_value(ast_ins->right, res);
        set_value(ast_ins->left, right_value, res);
    }

    else if(ast_ins->op_type == SUB){
        int right_value = get_value(ast_ins->right, res);
        int left_value = get_value(ast_ins->left, res);
        int result = left_value - right_value;
        set_value(ast_ins->left, result, res);
    }
    
    else if(ast_ins->op_type == ADD){
        int right_value = get_value(ast_ins->right, res);
        int left_value = get_value(ast_ins->left, res);
        int result = right_value + left_value;
        set_value(ast_ins->left, result, res);
    }
    return;
}

//type이 value라면 value값 그대로 return, 만약 register라면 register에 있는 값 return
int get_value(Node node, Resource* res){
    if(node.type == REG){
        switch(node.reg){
            case 'A':
                return res->regs.A;

            case 'B':
                return res->regs.B;

            case 'C':
                return res->regs.C;

            case 'D':
                return res->regs.D;
        }
    }
    else if(node.type == VALUE){
        return node.value;
    }
}

void set_value(Node node, int value, Resource* res){
    //printf("test2: %d\n", node.type);
    if(node.type == REG){
        switch(node.reg){
            case 'A':
                res->regs.A = value;
                break; 

            case 'B':
                res->regs.B = value;
                break; 

            case 'C':
                res->regs.C = value;
                break; 

            case 'D':
                res->regs.D = value;
                break;
        }
        
    }
    return;
}