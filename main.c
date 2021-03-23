#include "common.h"


/*
각 각의 프로세스마다 Arrive Time이랑 Serive Time정함. 
Service Time의 경우 프로세스내의 AST 리스트의 길이로 가정한다. 길이 하나당 1ms
*/
int main(int argc, char** argv){

    Init* init = menu();
    // init의 멤버인 AST** 설정
    parse(init);
    //Queue에 맞는 프로세싱 및 service time만큼의 run
    for(int i = 0; i < init->process_numberof;i++){
        printf("=================================\n");
        dump(init->parse_tree_list[i]);
        free_ast(init->parse_tree_list[i]);
    }
    scheduling(init);

    free(init->process_list);
    free(init->arrive_time);
    free(init);
    return 0;
}

//어떤 큐를 선택할 것인지
Init* menu(){

    char* processes[] = {
        "MOV A 9;SUB A 4;ADD A A",
        "MOV B 10;SUB B 4;MOV C 2",
        "MOV D 20;MOV A 20;MOV C 7",
        "MOV A 5;ADD A 12;ADD A 16",
    };

    Init* init = (Init*)malloc(sizeof(Init));
    printf("Hello! This Program is Test Machine for scheduling!\n");
    printf("If you want test for scheduling, You Choice that.\n"); 
    printf("1. FIFO\n2. SJF\n3. RR\n4. MLFQ\n"); 
    printf("> "); 
    scanf("%d", &init->select);

    printf("Now, You choice number of process.\n");
    printf("> ");
    scanf("%d", &init->process_numberof);

    init->arrive_time = (int*)malloc(init->process_numberof * sizeof(int));
    printf("Ok, Please input each arrive time.\n");
    for(int i = 0; i < init->process_numberof; i++){
        printf("%d's arrive time : ", i+1);
        scanf("%d", &init->arrive_time[i]);
    }
    init->process_list = (char**)malloc(init->process_numberof * sizeof(char**));
    for(int i = 0; i < init->process_numberof; i++){
        init->process_list[i] = processes[i];
    }

    init->parse_tree_list = (AST**)malloc(init->process_numberof * sizeof(AST**));
    return init;
}

// 몇개의 프로세스를 활성화 시킬 것인지, 각 각의 Arrive Time은 어떻게 할것인지