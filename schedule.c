#include "common.h"

//큐의 개수
const int SJF_QUEUE = 1;
const int FIFO_QUEUE = 1;
const int RR_QUEUE = 1; 
const int MLFQ_QUEUE = 5;

const int true = 1;
const int false = 0;

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
//queuee manager가 관리하는 한줄의 큐를 free시킴
void free_queue_manager(QueueManager* queue_manager, int queue_size){
    for(int i=0;i<queue_size;i++){
        free_queue(queue_manager[i].head);
    }
}

void free_queue(Queue* head){
    Queue* curr = head; 
    while(curr){
        Queue* tmp = curr; 
        free(curr);
        curr = tmp->next;
    }
}

void print_queue(QueueManager* queue_manager, int queue_number){
    return;
}

void insert_queue(QueueManager* queue_manager, int queue_number, int process_number){
    Queue* new_queue = (Queue*)malloc(sizeof(Queue)); 
    new_queue->process_number = process_number; 
    new_queue->next = NULL;
    if(queue_manager[queue_number].head == NULL){
        queue_manager[queue_number].head = new_queue;
        queue_manager[queue_number].tail = new_queue;
    } else{
        queue_manager[queue_number].tail->next = new_queue;
        queue_manager[queue_number].tail = new_queue;
    }
}

//Queue리스트에서 첫번째 큐를 빼내온다. Head값 가져옴
//해당 큐는 추후 FREE 
Queue* get_queue(QueueManager* queue_manager, int queue_number){
    if(queue_manager[queue_number].head == NULL){
        return NULL;
    }
    else{
        Queue* return_queue = queue_manager[queue_number].head;
        queue_manager[queue_number].head = queue_manager[queue_number].head->next; 
        return return_queue;
    }
}

QueueManager* init_queue_manager(int queue_size){
    QueueManager* queue_manager = (QueueManager*)malloc(sizeof(QueueManager) * queue_size);
    for(int i=0;i<queue_size;i++){
        queue_manager[i].head = NULL;
        queue_manager[i].tail = NULL;
        queue_manager[i].priority = i;
    }
    return queue_manager; 
}

//먼저 들어온 큐에 대해 리로스 선점 실행처리를 하면서 큐를 돌릴까 
//Arrive Time이 가장 빠른것 부터 처리 
void check_arrive_time(Init* init, QueueManager* queue_manager, int queue_number, int cpu_time){
    for(int i=0;i<init->process_numberof;i++){
        if(cpu_time == init->arrive_time[i]){
            insert_queue(queue_manager, queue_number, i);
        }
    }
}

//프로세스가 완전히 끝나더라도 초기화된 PCB를 백업 시켜주는 과정 포함시키기
void FIFO(Init* init, Resource* res){
    int queue_number = 0;
    QueueManager* queue_manager = init_queue_manager(FIFO_QUEUE);
    int cpu_time = 0;
    int running = false;
    int current_process_number = 0;
    AST* processing_parse_tree = NULL;
    for(int finished_number_of_process = 0;finished_number_of_process<init->process_numberof;){
        //CPU Time과 각 프로세스간의 Arrive 타임 체크 동일한 시간대라면 큐를 삽입한다. 
        check_arrive_time(init, queue_manager, queue_number, cpu_time);
        //Running상태가 아니라면 큐에서 첫 번째 큐를 가지고온다. 
        //그리고 running true로 설정하고 프로세싱 -> 프로세싱이 끝나면 running false
        if(running == false){
            Queue* current_queue = get_queue(queue_manager, queue_number);
            //큐가 비어있지 않다면
            if(current_queue != NULL){
                printf("-SWITCHING-");
                /*
                    Context Switching이 발생하는 부분 
                    새로운  프로세스의 PCB를 초기화하고 res에 덮는다. 
                */
                current_process_number = current_queue->process_number;
                PCB pcb;
                memset(&pcb.res, 0, sizeof(PCB));
                pcb.process_number = current_process_number;
                init->pcb_list[current_process_number] = pcb;
                memcpy(res, &pcb.res, sizeof(Resource));
                free(current_queue);
                running = true;
                //여기서 AST 넘김
                processing_parse_tree = init->parse_tree_list[current_process_number];
            }else{
                printf("-SLEEP-");
            }
        } 
        else if(running == true){
            /*
                선점된 프로세스의 종료이다. 
                나중을 위해 프로세스가 종료되더라도 해당 res를 해당 프로세스의 PCB에 넣는다. 
            */
            if(processing_parse_tree == NULL){
                running = false;
                finished_number_of_process +=1;
                memcpy(&init->pcb_list[current_process_number].res, res, sizeof(Resource));
                //show_resource(&init->pcb_list[current_process_number].res);
            }
            else{
                execute_ins(processing_parse_tree, res);
                processing_parse_tree = processing_parse_tree->next_instruction;
                printf("-%d-", current_process_number);
            }
        }
        cpu_time+=1;
    }
    free(queue_manager);   
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
    printf("\n===============REG=================\n");
    printf("A regs : %d\n", res->regs.A);
    printf("B regs : %d\n", res->regs.B);
    printf("C regs : %d\n", res->regs.C);
    printf("D regs : %d\n", res->regs.D);
}