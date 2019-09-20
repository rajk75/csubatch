#include <time.h>
#include <stdlib.h>
#include <stdio.h>
//inlcude header files
#include "csu-batch.h"
#include "job-queue.h"
#include "global.h"
enum scheduling_policy _scheduling_policy = FCFS; //internal var, determines how the scheduling module will insert a new job. 0: fcfs, 1: sjf, 2: priority

struct job
{
    char* name;
    int execution_time;
    int priority;
    time_t arrival_time;
    int progress;
};

struct node
{
    struct job* data;
    struct node* next;
};

//initializing
int num_jobs = 0;
struct node* _head = 0; //because this is a queue, the front of the queue will always be head.
struct node* _cur = 0;

//todo: write a restructing algorithm

/*
* decontruct function traverse though queue structure to free pointers
*/
int deconstruct_queue()
{
    if(_head == NULL)
    {
        return 1;
    }
    //set current pointer to head
    _cur = _head;
    //traverse and free pointers
    while(_cur != NULL)
    {
        free(_cur->data);
        struct node* old = _cur;
        _cur = _cur->next;
        free(old);
    }
    return 0;
}

/*
* the get current scheduling policy function returns the name of the current scheduling policy
* @return char* the name of the current policy
*/
char* get_current_scheduling_policy()
{
    char* policy_name = (char*)(malloc(sizeof(char)*8));
    switch (_scheduling_policy)
    {
    case FCFS:
        policy_name = "FCFS";
        break;
    case SJF:
        policy_name = "SJF";
        break;
    case PRIORITY:
        policy_name = "PRIORITY";
        break;
    default:
        policy_name = "invalid";
        break;
    }
    return policy_name;
}

/*
* get ndoe data returns job data based on scheduling policy
* @param
* @return
*/
int get_job_data_from_node(struct node* target)
{
    switch(_scheduling_policy)
    {
        case FCFS:
            return target->data->arrival_time;
        break;
        case SJF:
            return target->data->execution_time;
        break;
        case PRIORITY:
            return target->data->priority;
    }
    printf("fatal error: scheduling policy invaild");
    exit(1);
    return -1;
}

/*
* move pointer function moves the _cur pointer to the node before where the node to be inserted should be, based on scheduling policy
*/
void move_pointer(struct node* new_node)
{
    //TODO set error start and let csubatch handle errors
    if(_head == NULL)
    {
        printf("job queue unpopulated error! exiting...");
        exit(1);
    }
    //start pointer at front  of queue.
    if(_head->data == NULL)
    {
        printf("null job data error! exiting...");
        exit(1);
    }
    _cur = _head;
    if(_scheduling_policy != FCFS) //dangerous, assums vaild scheduling policy number.
    {
        while(_cur->next != NULL && get_job_data_from_node(_cur->next) < get_job_data_from_node(new_node))
        {
            _cur = _cur->next;
        }   
    }
    else
    {
        while(_cur->next != NULL)
        {
            _cur = _cur->next;
        }
    }
    /*
    if (_scheduling_policy == FCFS) //if the scheduling policy is fcfs
    {
        //simply move to node at the end of the queue
        while(_cur->next != NULL)
        {
            _cur = _cur->next;
        }
    }
    else if (_scheduling_policy == SJF)
    {
        //shortest job first
        while(_cur->next != NULL && _cur->next->data->execution_time < new_node->data->execution_time)
        {
            _cur = _cur->next;
        }
    }
    else if (_scheduling_policy == PRIORITY)
    {
        //priority
        while(_cur->next != NULL && new_node->data->priority < _cur->next->data->priority)
        {
            _cur = _cur->next;
        }
    }
    else
    {
        return 1;
    }
    */
}

/*
* the insert node function handles inserting the new node, to _cur next pointer
* @param new_node; node to be inserted
* @return 0 or 1; 1 on failure
*/
int insert_node(struct node* new_node)
{
    if(_head == NULL || _cur == NULL)
    {
        //should be unreachable
        return 1;
    }
    int set = 0;
    if(_scheduling_policy != FCFS && get_job_data_from_node(new_node) < get_job_data_from_node(_cur))
    {
        new_node->next = _cur;
        _head = new_node;
        set = 1;
    }
    else if(_scheduling_policy == FCFS)
    {
        _cur->next = new_node;
        set = 1;
    }
    /*
    switch(_scheduling_policy)
    {
        case FCFS:
            _cur->next = new_node;
            set =1;
        break;
        case SJF:
            if(new_node->data->execution_time < _cur->data->execution_time)
            {
                new_node->next = _cur;
                _head = new_node;
                set = 1;
            }
        break;
        case PRIORITY:
            if(new_node->data->priority < _cur->data->priority)
            {
                new_node->next = _cur;
                _head = new_node;
                set = 1;
            }
        break;    
    }
    */
    if(_cur->next == NULL && !set) //if next is NULL, then at the end of queue, therefore _cur set next to new_node.
    {
        _cur->next = new_node;
    }
    if(!set)//pointer to the left of where new_node needs to be inserted
    {
        new_node->next = _cur->next;
        _cur->next = new_node;
    }
    num_jobs++;
    return 0;
}

/* 
* find total waiting time function traverses the queue and tallies exeuction time of all submitted jobs
*/
int find_total_waiting_time(struct node* new_node)
{
    int total_time = 0;
    _cur = _head;
    while(_cur != NULL && _cur != new_node)
    {
        total_time += _cur->data->execution_time;
        _cur = _cur->next;
    }
    return total_time;
}

/*
* submit_job function handles creating and pushing a new job onto the job queue structure.
* @param job_name, a char pointer that contains the job name
* @param job_execution_time, an int that conveys how long the job will take to complete
* @param job_priority, an int that which convey the job's priority
* @return 1 or 0, 1 on failure.
*/
int submit_job(char* job_name, int job_execution_time, int job_priority)
{
    if(_head == NULL)
    {
        _head = (struct node*)(malloc(sizeof(struct node)));
        _head->data = NULL;
        _head->next = NULL;
    }
    //create a new job
    struct job* new_job = (struct job*)(malloc(sizeof(struct job)));
    new_job->name = job_name;
    new_job->execution_time = job_execution_time;
    new_job->priority = job_priority;
    time(&(new_job->arrival_time));
    //if head data is null asign new job to head->data
    struct node* new_node = NULL;
    if(_head->data == NULL)
    {
        _head->data = new_job;
        num_jobs++;
    }
    else //else create a new node
    {   
        new_node = (struct node*)(malloc(sizeof(struct node)));
        new_node->data = new_job;
        new_node->next = NULL;
        move_pointer(new_node);
        insert_node(new_node);
    }
    int time_to_wait = 0;
    if(num_jobs > 1 && new_node != NULL)
    {
        time_to_wait = find_total_waiting_time(new_node);
    }
    printf("Job %s was sumbitted.\nTotal number of jobs in the queue: %d\nExpected waiting time: %d seconds\nScheduling Policy: %s\n", job_name, num_jobs, time_to_wait, get_current_scheduling_policy());
    return 0;
}

/*
* list function traveres queue and displays information about job status
*/
void list_jobs()
{
    printf("Total number of jobs in the queue: %d\n", num_jobs);
    printf("Scheduling Policy: %s\n", get_current_scheduling_policy());
    if(_head == NULL)
    {
        return;
    }
    printf("Name\tCPU_time\tPri\tArrival_time\tProgress\n");
    //todo: traverse queue and print name cpu time, prioity, arrival time, and progress.
    _cur = _head;
    while(_cur != NULL)
    {
        struct job* cur_job =  _cur->data;
        struct tm* time = localtime(&(cur_job->arrival_time));
        printf("%s\t%d\t\t%d\t%d:%d:%d\tN/I\n", cur_job->name, cur_job->execution_time, cur_job->priority, time->tm_hour, time->tm_min, time->tm_sec);
        _cur = _cur->next;
    }
}

//TODO fix me
void selection_sort(struct node** node_arr)
{
    for(int i = 0; i < num_jobs; i++)
    {
        for(int j = i + 1; j < num_jobs; j++)
        {
            if(get_job_data_from_node(node_arr[i]) > get_job_data_from_node(node_arr[j]))
            {
                struct node* temp = node_arr[i];
                node_arr[i] = node_arr[j];
                node_arr[j] = temp;
            }
        }
    }
   
    //unoptimized code
    /*
    int flag1, flag2;
    for(int i = 0; i < num_jobs; i++)
    {
        switch(_scheduling_policy)
        {
          case FCFS:
                flag1= node_arr[i]->data->arrival_time;
            break;
            case SJF:
                flag1 = node_arr[i]->data->execution_time;
            break;
            case PRIORITY:
                flag1 = node_arr[i]->data->priority;
            break;  
        }
        for(int j = i + 1; j < num_jobs; j++)
        {
            
            switch(_scheduling_policy)
            {
                case FCFS:
                    flag2 = node_arr[j]->data->arrival_time;
                break;
                case SJF:
                    flag2 = node_arr[j]->data->execution_time;
                break;
                case PRIORITY:
                    flag2 = node_arr[j]->data->priority;
                break;
            }
            if(flag1 > flag2)
            {
                struct node* temp = node_arr[i];
                node_arr[i] = node_arr[j];
                node_arr[j] = temp;
            }
        }
    }
    */
}

/*
* reorder nodes function contains logic to traverse queue and reorder nodes depending on new scheduling policy
*/
void reorder_nodes()
{
    if (num_jobs > 1)
    {
    struct node** node_arr = malloc(sizeof(struct node)*num_jobs);
    _cur = _head;
    //populate node array
    for(int i = 0; i < num_jobs; i++)
    {
        node_arr[i] = _cur;
        _cur = _cur->next;
    }
    //selection_sort(node_arr);
    //sort the array based on scheduling policy
    switch(_scheduling_policy)
    {
        case FCFS:
            // first come first serve based on arrival time, smallest arrival time first
            for(int i = 0; i < num_jobs; i++)
            {
                for(int j = i+1; j < num_jobs; j++)
                {
                    if(node_arr[j]->data->arrival_time < node_arr[i]->data->arrival_time) 
                    {
                        struct node* temp = node_arr[i];
                        node_arr[i] = node_arr[j];
                        node_arr[j] = temp;
                    }
                }
            }
        break; 
        case SJF:
            //shortest job first, smallest execution time first
            for(int i = 0; i < num_jobs; i++)
            {
                for(int j = i+1; j < num_jobs; j++)
                {
                    if(node_arr[j]->data->execution_time < node_arr[i]->data->execution_time)
                    {
                        struct node* temp = node_arr[i];
                        node_arr[i] = node_arr[j];
                        node_arr[j] = temp;
                    }
                }
            }
        break;
        case PRIORITY:
            //priority 
            for(int i = 0; i < num_jobs; i++)
            {
                for(int j = i+1; j < num_jobs; j++)
                {
                    if(node_arr[j]->data->priority < node_arr[i]->data->priority)
                    {
                        struct node* temp = node_arr[i];
                        node_arr[i] = node_arr[j];
                        node_arr[j] = temp;
                    }
                }
            }
        break;
    }

    _head = node_arr[0];
    _cur = _head;
    for(int i = 1; i < num_jobs; i++)
    {
        _cur->next = node_arr[i];
        _cur = _cur->next;
    }
    _cur->next = NULL;
    free(node_arr);
    }
}

/*
* the change_scheduling_policy updates the schduling policy to the paramater pasted and restructures the queue
* 
* @param new_policy; the new policy to update the current scheudling policy to
* @return returns a 1 or 0, 1 on failure.
*/
int change_scheduling_policy(int new_policy)
{
    if(_scheduling_policy == new_policy)
    {
        printf("Scheduling policy already set to %s. No jobs have been rescheduled.\n", get_current_scheduling_policy());
        return 1;
    }
    _scheduling_policy = new_policy;
    //call restructure function to reorder jobs.
    reorder_nodes();
    printf("Scheduling policy has been switched to %s. All the %d waiting jobs have been rescheduled.\n", get_current_scheduling_policy(), num_jobs);
    return 0;
}