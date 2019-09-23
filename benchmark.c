#include "benchmark.h"
#include "global.h"
#include "job-queue.h"
#include <pthread.h>
#include <time.h>

/* for averages */
//Number of completed jobs
int num_jc = 0;
//Sum of time between job entry and time completion
int sum_job_entry_comletion = 0;
//Sum of time between job entry time and in-progress time
int sum_job_entry_inprogress = 0;
//Sum of time from job in-progress to job completion
int sum_job_inprogres_completion = 0;
//Time of job start
int lap_job_start = 0;
//Time of job when in progress
int lab_job_inprogress = 0;
//Time of job finish
int lab_job_finish = 0;

/* for throughput */
//Time elapsed since the start of program
int program_start_time = 0;
/*data structure*/
//array of structure; job, execution time, priority; for csubatch.c to submit
struct node* benchmark_head;

void clock_job_entry()
{
    //uses time.h to set lap_job_start time(&<where to put time>)
}

void clock_job_inprogress()
{
    //uses time.h to set lab_job_inprogress, and 
    //set sum_job_entry_to_inprogress += lab_job_inprogress - lap_job_start;
}

void clock_job_completion()
{
    //uses time.h to set lab_job_completion
    //set sum_job_entry_completion += lab_job_completion - lab_job_entry
    //set sum_job_inprogress_completion += lab_job_completion - lab_job_inprogress
    //num_jobs++;
}

void start_test(char* benchmark_name, enum scheduling_policy policy, int num_of_jobs)
{
    //make sure job queue is empty, wait for queue to be empty using pthread_cond()
    //open benchmark_name file, 
    //stream in contents, creating a node and job for each
    //loop for num of jobs specified for the user creating a node for each
    //while queue not empty, continue to submit jobs 
    //calling other modules. mostly just submit_job();
}



// remove after devleopment, this main is for testing.
/*int main()
{
    start_test("examplebenchmakr", FCFS, 4);
}*/