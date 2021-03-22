#include "main.h"
#include "parse.h"

/*
숫자는 두자리를 지원하지 않는다. 
*/
char A_process[] =  "MOV A 9;SUB A 4;ADD A A";

int main(int argc, char** argv){
    parse(A_process);
    return 0;
}