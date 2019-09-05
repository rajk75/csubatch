#include <stdlib.h>

int __scheduling_policy = 0; //internal var, determines how the scheduling module will insert a new job

struct job
{
    char* name;
    int execution_time;
    int priority;
};

struct node
{
    struct job* data;
    struct node* next;
};

int num_jobs = 0;
struct node* __head = 0; //because this is a queue, the front of the queue will always be head.
struct node* __cur = 0;

/*
* the get current scheduling policy function returns the name of the current scheduling policy
*
*/
char* get_current_scheduling_policy()
{
    char* policy_name = (char*)(malloc(sizeof(char)*8));
    switch (__scheduling_policy)
    {
    case 0:
        policy_name = "FCFS";
        break;
    case 1:
        policy_name = "SJF";
        break;
    case 2:
        policy_name = "PRIORITY";
        break;
    default:
        policy_name = "invalid";
        break;
    }
    return policy_name;
}

/*
* move pointer function moves the __cur pointer to the node before where the node to be inserted should be, based on scheduling policy
* @return 0 or 1, 1 if failure.
*/
int move_pointer(struct node* new_node)
{
    if(__head == 0)
    {
        return 1;
    }
    //start pointer at front  of queue.
    __cur = __head;
    if (__scheduling_policy == 0) //if the scheduling policy is fcfs
    {
        //simply move to end of queue
        int move = 1;
        while(move)
        {
            if(__cur->next != 0)
            {
                __cur = __cur->next;
            }
            if(__cur->next == 0)
            {
                move = 0;
            }
        }
        return 0;
    }
    //todo, add logic for other scheduling algorithms
    else
    {
        return 1;
    }
}

/*
* the free pointers function traverses through queue and frees allocated memory
*
*/
int free_pointers()
{
    return 1;
}

/*
* the insert node function handles inserting the new node, to __cur next pointer
* @param new_node; node to be inserted
* @return 0 or 1; 1 on failure
*/
int insert_node(struct node* new_node)
{
    if(__cur == 0)
    {
        return 1;
    }
    if(__cur->next == 0) //if next is 0, then at the end of queue, therefore just set next to new node.
    {
        __cur->next = new_node;
    }
    else
    {
        new_node->next = __cur->next;
        __cur->next = new_node;
    }
    num_jobs++;
    return 0;
}

int submit_job(char* job_name, int job_execution_time, int job_priority)
{
    if(__head == 0)
    {
        __head = (struct node*)(malloc(sizeof(struct node)));
        __cur = __head;
    }
    //create a new job
    struct job* new_job = (struct job*)(malloc(sizeof(struct job)));
    new_job->name = job_name;
    new_job->execution_time = job_execution_time;
    new_job->priority = job_priority;
    //create a new node
    struct node* new_node = (struct node*)(malloc(sizeof(struct node)));
    new_node->data = new_job;
    new_node->next = 0;
    //move to end of queue function
    move_pointer(new_node);
    insert_node(new_node);
    printf("Job %s was sumbitted.\nTotal number of jobs in the queue: %d\nExpected waiting time: %d seconds\nScheduling Policy: %s\n", job_name, num_jobs, -1, get_current_scheduling_policy());
    return 1;
}

//todo: write a restructing algorithm

/*
* the change_scheduling_policy updates the schduling policy to the paramater pasted and restructures the queue
* 
* @param new_policy; the new policy to update the current scheudling policy to
* @return returns a 1 or 0, 1 on failure.
*/
int change_scheduling_policy(int new_policy)
{
    if(new_policy < 0 || new_policy > 2)
    {
        return 1;
    }
    if(__scheduling_policy == new_policy)
    {
        printf("Scheduling policy already set to %s. No jobs have been rescheduled.\n", get_current_scheduling_policy());
        return 1;
    }
    __scheduling_policy = new_policy;
    //call restructure function to reorder jobs.
    printf("Scheduling policy has been switched to %s. All the %d waiting jobs have been rescheduled.\n", get_current_scheduling_policy(), -1);
    return 0;
}