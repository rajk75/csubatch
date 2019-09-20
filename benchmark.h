/*
this header file contains defitions for bechmark module
*/
#ifndef BENCHMARK
#define BENCHMARK
#include "global.h"
//functions
void create_process_benchmark(char* benchmark_name, enum scheduling_policy policy, int num_of_jobs) ;

void* start_background_thread();
/*
while(get_program_state()== RUNNING)
{
    //TODO; logic for keeping statistics.
}
*/
#endif