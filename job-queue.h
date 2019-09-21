#ifndef JOBQUEUE
#define JOBQUEUE

#include <pthread.h>
#include "global.h"
struct job
{
    char* name;
    int execution_time;
    int priority;
    time_t arrival_time;
    enum job_state progress;
};

struct node
{
    struct job* data;
    struct node* next;
};

pthread_mutex_t job_q_mu;

void _init_job_queue();

struct node* peek();

int deconstruct_queue();

char* get_current_scheduling_policy();

void move_pointer(struct node* new_node);

int enqueue(struct node* new_node);

int find_total_waiting_time(struct node* new_node);

int submit_job(char* job_name, int job_execution_time, int job_priority);

void list_jobs();

void selection_sort(struct node**  node_arr);

int change_scheduling_policy(int new_policy);

#endif