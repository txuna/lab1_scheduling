#include "main.h"
#include "parse.h"


char A_process[] =  "MOV A 9;SUB A 4;ADD A A";
char B_process[] = "MOV B 10;SUB B 4;MOV C 2";
char C_process[] = "MOV D 20;MOV A 20;MOV C 7";
char D_process[] = "MOV A 5;ADD A 12;ADD A 16";
/*
각 각의 프로세스마다 Arrive Time이랑 Serive Time정함. 
Service Time의 경우 프로세스내의 AST 리스트의 길이로 가정한다. 길이 하나당 1ms
*/
int main(int argc, char** argv){
    int numberof = menu();
    AST* ast = parse(A_process);
    dump(ast);
    free_ast(ast);


    return 0;
}

//어떤 큐를 선택할 것인지
int menu(){
    int select;
    printf("Hello! This Program is Test Machine for scheduling!\n");
    printf("If you want test for scheduling, You Choice that.\n"); 
    printf("1. FIFO\n2. SJF\n3. RR\n4. MLFQ\n"); 
    printf("> "); 
    scanf("%d", &select);
    return select;
}

// 몇개의 프로세스를 활성화 시킬 것인지, 각 각의 Arrive Time은 어떻게 할것인지