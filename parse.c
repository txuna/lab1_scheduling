#include "common.h"

/*
명령문을 읽어들이고 한줄 단위로 쪼갬 -> 한줄 씩 파싱하면서 띄워쓰기 단위로 끊고 구조체 구성
*/
void parse(Init* init){
    for(int i=0; i<init->process_numberof;i++){
        AST* command_list = loop_about_semicolon(init->process_list[i]);
        init->parse_tree_list[i] = command_list;
    }
    
    //dump(command_list);
    //free_ast(command_list);
}

void dump(AST* ast){
    AST* dump_ast = ast;
    while(dump_ast){
        printf("OP_TYPE : %d\nLEFT_TYPE : %d\nRIGHT_TYPE : %d\n\n", dump_ast->op_type, dump_ast->left.type, dump_ast->right.type);
        dump_ast = dump_ast->next_instruction;
    }
}

void free_ast(AST* ast){
    AST* curr = ast; 
    AST* next = curr;
    if(curr == NULL){
        return;
    }
    while(curr){
        free(curr);
        next = curr->next_instruction;
        curr = next;
    }
    return;
}

AST* get_end_list(AST* ast){
    AST* end_ast = ast;
    if(end_ast == NULL){
        return NULL;
    }
    while(end_ast->next_instruction != NULL){
        end_ast = end_ast->next_instruction;
    }
    return end_ast;
}

/*
parse로 받아온 ins를 세미클론 단위로 쪼개서 parse_ins에 넘김 
*/
AST* loop_about_semicolon(char* ins){
    char* new_ins = make_new_string(ins);
    AST* command_list = NULL;
    char* next_ptr = NULL;
    char* ptr = strtok_r(new_ins, ";", &next_ptr);
    
    while(ptr != NULL){
        AST* node = parse_ins(ptr);
        if(command_list == NULL){
            command_list = node;
        }else{
            AST* end_ast = get_end_list(command_list);
            end_ast->next_instruction = node;
            //command_list->next_instruction = node;
        }
        ptr = strtok_r(NULL, ";", &next_ptr);
    }
    free(new_ins);
    return command_list;
}
/*
하나의 instruction을 3개의 조합으로 분할 시킴
*/
AST* parse_ins(char* ins){
    char* next_ptr = NULL;
    char* command = make_new_string(ins);
    char* ptr = NULL;

    ptr = strtok_r(command, " ", &next_ptr); 
    char* op = make_new_string(ptr);

    ptr = strtok_r(NULL, " ", &next_ptr);
    char* left = make_new_string(ptr);

    ptr = strtok_r(NULL, " ", &next_ptr);
    char* right = make_new_string(ptr);


    //printf("INS : %s %s %s\n", op, right, left);
    AST* node =  set_ast(op, left, right);

    free(op);
    free(left);
    free(right);
    free(command);

    return node;
}

/*
각 명령어별로 mov, sub, add 판단하고 right, left 판단하고 노드 생성
*/
AST* set_ast(char* op, char* left, char* right){
    AST* node = (AST*)malloc(sizeof(AST));
    memset(node, 0, sizeof(AST));
    set_op(op, node);
    set_left(left, node);
    set_right(right, node);
    node->next_instruction = NULL;
    return node;
}

void set_op(char* op, AST* node){
    if(!strcmp(op, "MOV")){
        node->op_type = MOV;
    }else if(!strcmp(op, "SUB")){
        node->op_type = SUB;
    }else if(!strcmp(op, "ADD")){
        node->op_type = ADD;
    }else{
        error_handler("Incorrect OP\n");
    };
    return;
}

// 레지스터만 옮 수 있음
void set_left(char* left, AST* node){
    if(!strcmp(left, "A")){
        node->left.reg = 'A';
    }else if(!strcmp(left, "B")){
        node->left.reg = 'B';
    }else if(!strcmp(left, "C")){
        node->left.reg = 'C';
    }else if(!strcmp(left, "D")){
        node->left.reg = 'D';
    }else{
        error_handler("Incorrect LEFT Value\n");
    }
    node->left.type = REG;
    return;
}

//레지스터 및 값이 올 수 있음
void set_right(char* right, AST* node){
    if(!strcmp(right, "A")){
        node->right.reg = 'A';
    }else if(!strcmp(right, "B")){
        node->right.reg = 'B';
    }else if(!strcmp(right, "C")){
        node->right.reg = 'C';
    }else if(!strcmp(right, "D")){
        node->right.reg = 'D';
    }else{
        node->right.type = VALUE;
        node->right.value = atoi(right);
        return;
    }
    node->right.type = REG;
    return;
}
