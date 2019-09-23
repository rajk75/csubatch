#include <time.h>
#include <stdlib.h>
#include <stdio.h>
//inlcude header files
#include "job-queue.h"
#include "global.h"

//initializing
enum scheduling_policy _scheduling_policy = FCFS;

int num_jobs = 0;
struct node* _head = NULL; //because this is a queue, the front of the queue will always be head.
struct node* _cur = NULL;

/**
 * the init job queue intitalizes defintions
 */
void _init_job_queue()
{
    pthread_mutex_init(&job_q_mu, NULL);
}

/**
 * The peek function returns the pointer to head
 * @return struct node*
 */
struct node* peek()
{
    return _head;
}

/**
* decontruct function traverse though queue structure to free pointers
*/
int deconstruct_queue()
{
    pthread_mutex_destroy(&job_q_mu);
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

/**
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

/**
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
    if(_scheduling_policy != FCFS) //dangerous, assumes vaild scheduling policy number.
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
}

/**
* the enequeue function handles inserting the new node, to _cur next pointer
* @param new_node; node to be inserted
* @return 0 or 1; 1 on failure
*/
int enqueue(struct node* new_node)
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

struct node* dequeue()
{
    struct node* old_head = _head;
    if(_head->next == NULL)
    {
        _head = NULL;
    }
    else
    {
        _head = _head->next;
    }
    old_head->next = NULL;
    num_jobs--;
    return old_head;
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

/**
* submit_job function handles creating and pushing a new job onto the job queue structure.
* @param job_name, a char pointer that contains the job name
* @param job_execution_time, an int that conveys how long the job will take to complete
* @param job_priority, an int that which convey the job's priority
* @return 1 or 0, 1 on failure.
*/
int submit_job(char* job_name, int job_execution_time, int job_priority)
{
    pthread_mutex_lock(&job_q_mu);
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
    new_job->progress = ISNOTRUNNING;
    //if head data is null asign new job to head->data
    struct node* new_node = NULL;
    if(_head->data == NULL)
    {
        //TODO; this should be in insert node
        _head->data = new_job;
        num_jobs++;
    }
    else //else create a new node
    {   
        new_node = (struct node*)(malloc(sizeof(struct node)));
        new_node->data = new_job;
        new_node->next = NULL;
        move_pointer(new_node);
        //pthread_mutex_lock(&job_q_mu);
        enqueue(new_node);
        //TODO; notify time of job entry
        //pthread_mutex_unlock(&job_q_mu);
    }
    int time_to_wait = 0;
    if(num_jobs > 1 && new_node != NULL)
    {
        time_to_wait = find_total_waiting_time(new_node);
    }
    printf("Job %s was sumbitted.\nTotal number of jobs in the queue: %d\nExpected waiting time: %d seconds\nScheduling Policy: %s\n", job_name, num_jobs, time_to_wait, get_current_scheduling_policy());
    pthread_mutex_unlock(&job_q_mu);    
    return 0;
}

void remove_job()
{
    struct node* old_head = dequeue();
    //struct job* old_job = old_head->data;
    //TODO; calculations to do on job finish. call function from benchmark
    free(old_head);
}

const char* progress_to_string(struct job* target)
{
    return target->progress == ISRUNNING ? "run" : " ";
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
    //TODO; fix formatting for printing
    printf("Name\tCPU_time\tPri\tArrival_time\tProgress\n");
    _cur = _head;
    while(_cur != NULL)
    {
        struct job* cur_job =  _cur->data;
        struct tm* time = localtime(&(cur_job->arrival_time));
        printf("%s\t%d\t\t%d\t%d:%d:%d\t%s\n", cur_job->name, cur_job->execution_time, cur_job->priority, time->tm_hour, time->tm_min, time->tm_sec, progress_to_string(cur_job));
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
            if(get_job_data_from_node(node_arr[i]) > get_job_data_from_node(node_arr[j]) && node_arr[i]->data->progress != ISRUNNING)
            {
                struct node* temp = node_arr[i];
                node_arr[i] = node_arr[j];
                node_arr[j] = temp;
            }
        }
    }
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
    selection_sort(node_arr);

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