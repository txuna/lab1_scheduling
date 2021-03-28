#include "common.h"

//큐의 개수
const int SJF_QUEUE = 1;
const int FIFO_QUEUE = 1;
const int RR_QUEUE = 1; 
const int MLFQ_QUEUE = 5;

const int SJF_TYPE = 1;
const int FIFO_TYPE = 2;

const int RR_TYPE = 3;
const int MLFQ_TYPE = 4;

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
            SJF(init, res);
            break;

        case 3:
            RR(init, res);
            break;

        case 4:
            MLFQ(init, res);
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
    Queue* curr = queue_manager[queue_number].head;
    printf("\nQUEUE : ");
    while(curr){
        printf("-%d", curr->process_number);
        curr = curr->next;
    }
    printf("\n");
}

void insert_queue(QueueManager* queue_manager, int queue_number, int process_number, int service_time){
    Queue* new_queue = (Queue*)malloc(sizeof(Queue)); 
    new_queue->process_number = process_number; 
    new_queue->service_time = service_time;
    new_queue->next = NULL;
    if(queue_manager[queue_number].head == NULL){
        queue_manager[queue_number].head = new_queue;
        queue_manager[queue_number].tail = new_queue;
    } else{
        queue_manager[queue_number].tail->next = new_queue;
        queue_manager[queue_number].tail = new_queue;
    }
}

Queue* get_min_queue(QueueManager* queue_manager, int queue_number){
    Queue* pivot = queue_manager[queue_number].head;
    Queue* tmp = pivot;
    Queue* prev = tmp;
    if(pivot == NULL){
        return NULL;
    }
    if(pivot->next == NULL){
        queue_manager[queue_number].head = NULL;
        queue_manager[queue_number].tail = NULL;
        return pivot;
    }
    while(tmp->next){
        if(pivot->service_time >= tmp->next->service_time){
            pivot = tmp->next;
            prev = tmp;
        }
        tmp = tmp->next;
    }
    if(pivot == queue_manager[queue_number].head){
        queue_manager[queue_number].head = queue_manager[queue_number].head->next;
    }
    else if(pivot == queue_manager[queue_number].tail){
        prev->next = NULL;
        queue_manager[queue_number].tail = prev;
    }
    else{  
        prev->next = pivot->next;
  
    }
    return pivot;
    
}

//Queue리스트에서 첫번째 큐를 빼내온다. Head값 가져옴
//해당 큐는 추후 FREE 
Queue* get_queue(QueueManager* queue_manager, int queue_number){
    if(queue_manager[queue_number].head == NULL){
        return NULL;
    }
    else{
        Queue* return_queue = queue_manager[queue_number].head;
        if(return_queue->next == NULL){
            queue_manager[queue_number].head = NULL;
            queue_manager[queue_number].tail = NULL;
        }
        else{
            queue_manager[queue_number].head = queue_manager[queue_number].head->next; 
        }
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
            insert_queue(queue_manager, queue_number, i, init->serive_time[i]);
        }
    }
}

//프로세스의 PCB를 생성하고 초기화 하는 작업
void init_pcb(int process_number, Resource* res, Init* init){
    PCB pcb;
    memset(&pcb.res, 0, sizeof(PCB));
    pcb.process_number = process_number;
    init->pcb_list[process_number] = pcb;
    //memcpy(res, &pcb.res, sizeof(Resource));
}

//res로 진행중인 사항을 자신의 PCB로 백업한다. 
void backup_pcb(Init* init, int process_number, Resource* res){
    memcpy(&init->pcb_list[process_number].res, res, sizeof(Resource));
}

//backoup된 PCB를 res에 박는다. 
void set_pcb(Init* init, int process_number, Resource* res){
    memcpy(res, &init->pcb_list[process_number].res, sizeof(Resource));
}

void get_process_service_time(Init* init, int process_number, int time){
    init->serive_time[process_number] -= time;
}

//processing_parse_tree는 여기서 current_Ip만큼 땡겨서 가지고옴
AST* get_current_parse_tree(Init* init, int process_number){
    
    AST* parse_tree = init->parse_tree_list[process_number];
    int current_ip = init->pcb_list[process_number].res.current_ip;
    //printf("\nnumber :  %dparse : %d\n",process_number, current_ip);
    for(int i=0;i<current_ip;i++){
        parse_tree = parse_tree->next_instruction;
        if(parse_tree == NULL){
            return NULL;
        }
    }
    return parse_tree;
}

//프로세스가 완전히 끝나더라도 초기화된 PCB를 백업 시켜주는 과정 포함시키기
void FIFO(Init* init, Resource* res){
    fifo_sjf(init, res, FIFO_TYPE);
}


//처리중인 프로세스가 없다면 대기큐에 있는 프로세스중에서 가장 짝은 서비스타임을 가진 프로세스를 처리한다. 
//만약 처리중인 프로세스가 있다면 해당 프로세스를 완전히 처리한 후 짧은 거 선택
void SJF(Init* init, Resource* res){
    fifo_sjf(init, res, SJF_TYPE);
}

void fifo_sjf(Init* init, Resource* res, int type){
    int queue_number = 0;
    QueueManager* queue_manager = init_queue_manager(FIFO_QUEUE);
    int cpu_time = 0;
    int running = false;
    int current_process_number = 0;
    AST* processing_parse_tree = NULL;
    int is_switching = false;

    for(int finished_number_of_process = 0;finished_number_of_process<init->process_numberof;){
        //CPU Time과 각 프로세스간의 Arrive 타임 체크 동일한 시간대라면 큐를 삽입한다. 
        if(!is_switching){
            check_arrive_time(init, queue_manager, queue_number, cpu_time);
        }
        is_switching = false;
        //Running상태가 아니라면 큐에서 첫 번째 큐를 가지고온다. 
        //그리고 running true로 설정하고 프로세싱 -> 프로세싱이 끝나면 running false
        if(running == false){
            //get_queue 대신 가장 작은 큐를 가지고 온다.
            Queue* current_queue = NULL;
            if(type == FIFO_TYPE){
                current_queue = get_queue(queue_manager, queue_number);
            }else if(type == SJF_TYPE){
                current_queue = get_min_queue(queue_manager, queue_number);
            }
            
            //큐가 비어있지 않다면
            if(current_queue != NULL){
                printf("-SWITCHING-");
                is_switching = true;
                /*
                    Context Switching이 발생하는 부분 
                    새로운  프로세스의 PCB를 초기화하고 res에 덮는다. 
                */
                current_process_number = current_queue->process_number;
                init_pcb(current_process_number, res, init);
                set_pcb(init, current_process_number, res);
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
                backup_pcb(init, current_process_number, res);
                show_resource(&init->pcb_list[current_process_number].res, current_process_number);
            }
            else{
                execute_ins(processing_parse_tree, res);
                processing_parse_tree = processing_parse_tree->next_instruction;
                printf("-%d-", current_process_number);
            }
        }
        if(!is_switching){
            cpu_time+=1;   
        }
       
    }
    free(queue_manager);   
}

void MLFQ_RR(Init* init, Resource* res, int type_queue_number){
    int time_slice = 1;
    int queue_number = 0;
    QueueManager* queue_manager = init_queue_manager(MLFQ_QUEUE);
    int cpu_time = 0;
    int running = false;
    int current_process_number = 0;
    AST* processing_parse_tree = NULL;
    int current_time_slice = 0;
    int is_switching = false;
    int process_end = false;

    for(int finished_number_of_process = 0;finished_number_of_process<init->process_numberof;){
        if(!is_switching){
            check_arrive_time(init, queue_manager, 0, cpu_time);
        }else{
            is_switching = false;
        }
        if(running == false){
            //항상 모든 우선순위 큐를 돌리면서 우선순위가 높은 큐에 값이 있는지 찾는다. 
            for(queue_number=0;queue_number<type_queue_number;queue_number++){
                Queue* current_queue = get_queue(queue_manager, queue_number);
                if(current_queue == NULL){
                    continue; // 해당 큐는 비어져있으니 다음 우선순위의 큐를 찾는다.
                }else{
                    //printf("-SWITCHING-");
                    is_switching = true;
                    current_process_number = current_queue->process_number;
                    //printf("get queue : %d and cpu time : %d\n", current_process_number, cpu_time);
                    if(init->status[current_process_number] == READY){
                        init_pcb(current_process_number, res, init);
                    }
                    set_pcb(init, current_process_number, res);
                    free(current_queue);
                    running = true;
                    processing_parse_tree = get_current_parse_tree(init, current_process_number);
                    break;
                }
            }

            
        } 
        else if(running == true){
            execute_ins(processing_parse_tree, res);
            printf("-%d-", current_process_number);
            current_time_slice += 1;
            processing_parse_tree = processing_parse_tree->next_instruction;

            if(current_time_slice >= time_slice || processing_parse_tree == NULL){
                 backup_pcb(init, current_process_number, res);
                 if(processing_parse_tree == NULL){
                     //프로세스가 완전히 끝남
                    finished_number_of_process +=1;
                    init->status[current_process_number] = TERMINATE;
                    show_resource(&init->pcb_list[current_process_number].res, current_process_number);
                 }else{
                    //시간 할당량만을 다 썼기에 다음 순위의 큐에 insert 만약 최하위 큐라면 다시 해당 큐로 
                    get_process_service_time(init, current_process_number, current_time_slice);
                    //printf("insert : %d and cpu time : %d\n", current_process_number, cpu_time);
                    if(queue_number == type_queue_number-1){
                        insert_queue(queue_manager, queue_number, current_process_number, init->serive_time[current_process_number]);
                    }else{
                        insert_queue(queue_manager, queue_number+1, current_process_number, init->serive_time[current_process_number]);
                    }
                    init->status[current_process_number] = WAIT;
                 }
                //is_switching = true;
                running = false;
                current_time_slice = 0;
                //continue;//여기서도 Time Slice가 끝났는지 확인해야함 안그럼 CPU TIME 한번 날먹됨. 아니면 continue로 CPU TIME ++ ㄴㄴ 
            }
        }
        if(!is_switching){
            cpu_time+=1;
            //printf("\n  CPU PLUS %d\n", cpu_time);   
        }
    }
}

void RR(Init* init, Resource* res){
    MLFQ_RR(init, res, RR_QUEUE);
}
/*
새로들어오는 작업들은 우선순위가 가장 높은 큐에 넣는다. 
우선순위가 높은 큐(0-->1-->2) 부터 스케쥴링을 처리한다. 
우선순위가 가장 높은 큐를 처리하고나서 해당 큐가 비워져 있다면 다음 우선순위큐를 처리한다. 처리하다가 우선순위가 높은 큐가 있다면 해당 큐를 다시 처리한다. 


*/
void MLFQ(Init* init, Resource* res){
    MLFQ_RR(init, res, MLFQ_QUEUE);
}



void show_resource(Resource* res, int process_number){
    printf("\nProcess Number : %d  Register\n", process_number);
    printf("A regs : %d\n", res->regs.A);
    printf("B regs : %d\n", res->regs.B);
    printf("C regs : %d\n", res->regs.C);
    printf("D regs : %d\n", res->regs.D);
}