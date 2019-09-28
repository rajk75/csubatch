/*
this header file contains defitions for bechmark module
*/
#ifndef BENCHMARK
#define BENCHMARK
#include "global.h"
//functions
//void create_process_benchmark(char* benchmark_name, enum scheduling_policy policy, int num_of_jobs) ;

//void* start_background_thread();
/*
while(get_program_state()== RUNNING)
{
    //TODO; logic for keeping statistics.
}
*/

void start_test(char* benchmark_name, enum scheduling_policy policy, int num_of_jobs);

void clock_job_entry();

void clock_job_inprogress();

void clock_job_completion();

char * output_stats();

void clock_program_start();

void clock_program_end();


#endif