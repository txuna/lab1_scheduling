#include "common.h"

char* make_new_string(char* old){
    char* new_str = (char*)malloc(strlen(old) + 1);
    memset(new_str, 0, strlen(old) + 1);
    strncpy(new_str, old, strlen(old));
    return new_str;

}

void error_handler(char* msg){
    printf("%s\n", msg);
    exit(1);
}