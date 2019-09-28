#include "benchmark.h"
#include "global.h"
#include "job-queue.h"
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduling.h"
#include "dispatching.h"
#include <pthread.h>


/* for averages */
//Number of completed jobs
int num_jc = 0;
//Sum of time between job entry and time completion
time_t sum_job_entry_completion = 0;
//Sum of time between job entry time and in-progress time
time_t sum_job_entry_inprogress = 0;
//Sum of time from job in-progress to job completion
time_t sum_job_inprogres_completion = 0;
//Time of job start
time_t lap_job_start = 0;
//Time of job when in progress
time_t lap_job_inprogress = 0;
//Time of job finish
time_t lap_job_finish = 0;



struct jobInfo{
    char * jobName;
    int executionTime;
};

char job1[] = {"process"};
char job2[] = {"process"};
char job3[] = {"process"};
char job4[] = {"process"};
char job5[] = {"process"};

// int executionTime1 = 1;
// int executionTime2 = 1;
// int executionTime3 = 1;
// int executionTime4 = 1;
// int executionTime5 = 1;



// &(array[0].jobName) = job1;
// array[0].executionTime = 1;
// &(array[1].jobName) = job2;
// array[1].executionTime = 1;
// &(array[2].jobName) = job3;
// array[2].executionTime = 1;
// &(array[3].jobName) = job4;
// array[3].executionTime = 1;
// &(array[4].jobName) = job5;
// array[4].executionTime = 1;

// array[2].exec



/* for throughput */
//Time elapsed since the start of program
time_t program_start_time = 0;
time_t program_end_time = 0;



/*data structure*/
//array of structure; job, execution time, priority; for csubatch.c to submit
//struct node* benchmark_head;

void clock_program_start(){
    time(&program_start_time);
}

void clock_program_end(){
    time(&program_end_time);
}

void clock_job_entry()
{
    //uses time.h to set lap_job_start time(&<where to put time>)

    time(&lap_job_start);

}

void clock_job_inprogress()
{
    //uses time.h to set lap_job_inprogress, and 
    //set sum_job_entry_to_inprogress += lap_job_inprogress - lap_job_start;
    time(&lap_job_inprogress);
    sum_job_entry_inprogress += lap_job_inprogress - lap_job_start;
}

void clock_job_completion()
{
    time(&lap_job_finish);
    sum_job_entry_completion += lap_job_finish - lap_job_start;
    sum_job_inprogres_completion += lap_job_finish - lap_job_inprogress;
    num_jc++;
    //uses time.h to set lap_job_completion
    //set sum_job_entry_completion += lap_job_completion - lap_job_entry
    //set sum_job_inprogress_completion += lap_job_completion - lap_job_inprogress
    //num_jobs++;
}

void start_test(char* benchmark_name, enum scheduling_policy policy, int num_of_jobs)
{

    int l_num_jc =  num_jc;

    time_t l_sum_job_entry_completion = sum_job_entry_completion;

    time_t l_sum_job_entry_inprogress = sum_job_entry_inprogress;

    time_t l_sum_job_inprogres_completion = sum_job_inprogres_completion;

    time_t l_lap_job_start = lap_job_start;

    time_t l_lap_job_inprogress = lap_job_inprogress;

    time_t l_lap_job_finish = lap_job_finish;

    time_t l_program_start_time = program_start_time;

    time_t l_program_end_time = program_end_time;
    
    struct jobInfo array[5];
    (array[0].jobName) = job1;
    array[0].executionTime = 1;
    (array[1].jobName) = job2;
    array[1].executionTime = 1;
    (array[2].jobName) = job3;
    array[2].executionTime = 1;
    (array[3].jobName) = job4;
    array[3].executionTime = 1;
    (array[4].jobName) = job5;
    array[4].executionTime = 1;

    //int sum_benchmark_entry_completion;

    while(peek() != NULL);

    for(int i = 0; i < num_of_jobs; i++){
        char * job_name = (char *)(malloc(sizeof(char) * 20));
        strncpy(job_name,array[i].jobName,20);
        submit_job(job_name,array[i].executionTime,policy);
    }


    l_num_jc = num_jc - l_num_jc;
    l_sum_job_entry_completion = sum_job_entry_completion - l_sum_job_entry_completion;
    l_sum_job_entry_inprogress = sum_job_entry_inprogress - l_sum_job_entry_inprogress;
    l_sum_job_inprogres_completion = sum_job_inprogres_completion - l_sum_job_inprogres_completion;
    l_lap_job_start = lap_job_start - l_lap_job_start;
    l_lap_job_inprogress = lap_job_inprogress - l_lap_job_inprogress;
    l_lap_job_finish = lap_job_finish - l_lap_job_finish;
    l_program_start_time = program_start_time - l_program_start_time;
    l_program_end_time = program_end_time - l_program_end_time;

    printf("Total number of jobs submitted: %d\n",l_num_jc);
    printf("Average turn around time:    %ld seconds\n",(l_sum_job_entry_completion/num_jc));
    printf("Throughput:                  %ld seconds\n",(l_num_jc/(l_program_end_time-l_program_start_time)));

    
    


    //make sure job queue is empty, wait for queue to be empty using pthread_cond()
    //open benchmark_name file, 
    //stream in contents, creating a node and job for each
    //loop for num of jobs specified for the user creating a node for each
    //while queue not empty, continue to submit jobs 
    //calling other modules. mostly just submit_job();
    
    //before printing, sub global vars from local
    //calculate averages
    //print

}

char * output_stats(){
    char * str = malloc(sizeof(char) * 100);

    sprintf(str,"Total number of jobs submitted: %d\n",num_jc);
    sprintf(str,"Average turn around time:    %ld seconds\n",(sum_job_entry_completion/num_jc));
    sprintf(str,"Throughput:                  %ld seconds\n",(num_jc/(program_end_time-program_start_time)));



    return str;
}




// remove after devleopment, this main is for testing.
/*
int main()
{
    pthread_cond_init(&queue_not_empty_cond_t, NULL);
    pthread_mutex_init(&pipe_mu, NULL);
    pthread_mutex_init(&queue_t, NULL);
    pthread_mutex_init(&queue_state_t, NULL);
    pthread_t scheduling_t;
    pthread_t dispatching_t;
    _init_job_queue();
    //if(pthread_create(&scheduling_t, NULL, &scheduling_loop, &scheduling_sig))
    if(pthread_create(&scheduling_t, NULL, &scheduling_loop, get_scheduling_sig())) //use this if statement when dispatching module is setup
    {
        fprintf(stderr, "Error creating scheduling thread.");
        return 1;
    }
    if(pthread_create(&dispatching_t, NULL, &dispatching_loop, NULL))
    {
        fprintf(stderr, "Error creating dispatching thread");
        return 1;
    }
    start_test("examplebenchmakr", FCFS, 4);
    pthread_cond_signal(&queue_not_empty_cond_t);
    pthread_join(dispatching_t, NULL);
    pthread_join(scheduling_t, NULL);
    //printf("DEBUG: message to display when exiting csubatch\n");
    deconstruct_queue();
    pthread_mutex_destroy(&pipe_mu);
    pthread_mutex_destroy(&queue_t);
    pthread_cond_destroy(&queue_not_empty_cond_t);
    return 0;'
    
}*/