#ifndef JOBQUEUE
#define JOBQUEUE

struct job;
struct node;

int deconstruct_queue();

char* get_current_scheduling_policy();

void move_pointer(struct node* new_node);

int insert_node(struct node* new_node);

int find_total_waiting_time(struct node* new_node);

int submit_job(char* job_name, int job_execution_time, int job_priority);

void list_jobs();

void selection_sort(struct node**  node_arr);

void reorder_nodes();

int change_scheduling_policy(int new_policy);

#endif