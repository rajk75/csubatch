#include <time.h>
#include <stdlib.h>

int __scheduling_policy = 0; //internal var, determines how the scheduling module will insert a new job. 0: fcfs, 1: sjf, 2: priority

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

int num_jobs = 0;
struct node* __head = 0; //because this is a queue, the front of the queue will always be head.
struct node* __cur = 0;

//todo: write a restructing algorithm

/*
* decontruct function traverse though queue structure to free pointers
*/
int deconstruct_queue()
{
    if(__head == NULL)
    {
        return 1;
    }
    //set current pointer to head
    __cur = __head;
    //traverse and free pointers
    while(__cur != NULL)
    {
        free(__cur->data);
        struct node* old = __cur;
        __cur = __cur->next;
        free(old);
    }
    return 0;
}

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
    if(__head == NULL)
    {
        printf("fatal error, tried to move pointer before queue was populated.");
        exit(1);
    }
    //start pointer at front  of queue.
    __cur = __head;
    if(__head->data == NULL)
    {
        return 0;
    }
    if (__scheduling_policy == 0) //if the scheduling policy is fcfs
    {
        //simply move to node at the end of the queue
        while(__cur->next != NULL)
        {
            __cur = __cur->next;
        }
    }
    //todo, add logic for other scheduling algorithms
    else if (__scheduling_policy == 1)
    {
        //shortest job first, 
        while(__cur->next != NULL || new_node->data->execution_time > __cur->data->execution_time)
        {
            __cur = __cur->next;
        }
    }
    else if (__scheduling_policy == 2)
    {
        //priority
        while(__cur->next != NULL || new_node->data->priority < __cur->data->priority)
        {
            __cur = __cur->next;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

/*
* the insert node function handles inserting the new node, to __cur next pointer
* @param new_node; node to be inserted
* @return 0 or 1; 1 on failure
*/
int insert_node(struct node* new_node)
{
    if(__head == NULL || __cur == NULL)
    {
        return 1;
    }
    if(__cur->next == NULL) //if next is 0, then at the end of queue, therefore just set next to new node.
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

/* 
* find total waiting time function traverses the queue and tallies exeuction time of all submitted jobs
*/
int find_total_waiting_time()
{
    int total_time = 0;
    __cur = __head;
    while(__cur != NULL)
    {
        total_time += __cur->data->execution_time;
        __cur = __cur->next;
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
    if(__head == 0)
    {
        __head = (struct node*)(malloc(sizeof(struct node)));
        __head->data = NULL;
        __head->next = NULL;
    }
    //create a new job
    struct job* new_job = (struct job*)(malloc(sizeof(struct job)));
    //todo, add arrival time and progress to job struct.
    new_job->name = job_name;
    new_job->execution_time = job_execution_time;
    new_job->priority = job_priority;
    time(&(new_job->arrival_time));
    //if head data is null asign new job to head->data
    if(__head->data == NULL)
    {
        __head->data = new_job;
        num_jobs++;
    }
    else //else create a new node
    {   
        struct node* new_node = (struct node*)(malloc(sizeof(struct node)));
        new_node->data = new_job;
        new_node->next = NULL;
        move_pointer(new_node);
        insert_node(new_node);
    }
    int time_to_wait = 0;
    if(num_jobs > 1)
    {
        time_to_wait = find_total_waiting_time();
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
    if(__head == NULL)
    {
        return;
    }
    printf("Name\t\tCPU_time\tPri\tArrival_time\tProgress\n");
    //todo: traverse queue and print name cpu time, prioity, arrival time, and progress.
    __cur = __head;
    while(__cur != NULL)
    {
        struct job* cur_job =  __cur->data;
        struct tm* time = localtime(&(cur_job->arrival_time));
        printf("%s\t%d\t\t%d\t%d:%d:%d\tN/I\n", cur_job->name, cur_job->execution_time, cur_job->priority, time->tm_hour, time->tm_min, time->tm_sec);
        __cur = __cur->next;
    }
}

/*
* reorder nodes function contains logic to traverse queue and reorder nodes depending on new scheduling policy
*/
void reorder_nodes()
{
    struct node** node_arr = malloc(sizeof(struct node)*num_jobs);
    __cur = __head;
    //populate node array
    for(int i = 0; i < num_jobs; i++)
    {
        node_arr[i] = __cur;
        __cur++;
    }
    //sort the array based on scheduling policy
    switch(__scheduling_policy)
    {
        case 0:
            // first come first serve based on arrival time, smallest arrival time first
            printf("DEBUG: note impltemented yet");
            for(int i = 0; i < num_jobs; i++)
            {
                for(int j = i+1; j < num_jobs; j++)
                {
                    if(node_arr[j]->data->arrival_time < node_arr[i]->data->arrival_time) // TODO: write a function for this
                    {
                        struct node* temp = node_arr[i];
                        node_arr[i] = node_arr[j];
                        node_arr[j] = temp;
                    }
                }
            }
        break; 
        case 1:
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
        case 2:
            //priority 
            printf("DEBUG: note impltemented yet");
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
}

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
    reorder_nodes();
    printf("Scheduling policy has been switched to %s. All the %d waiting jobs have been rescheduled.\n", get_current_scheduling_policy(), -1);
    return 0;
}