#include "common.h"

void scheduling(Init* init){
    Resource* res = (Resource*)malloc(sizeof(Resource));
    memset(resource, 0, sizeof(Resource));
    switch(init->select){
        case 1:
        
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
void FIFO(Init* init, Resource* res){

}

void SJF(Init* init, Resource* res){

}

void RR(Init* init, Resource* res){

}

void MLFQ(Init* init, Resource* res){

}

void 