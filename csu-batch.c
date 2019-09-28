/*
Author: James Ellerbee
Purpose: Main for csu batch
*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//headers
#include "csu-batch.h"
#include "command-parser.h"
#include "job-queue.h"
#include "global.h"
#include "help2.h"
#include "scheduling.h"
#include "dispatching.h"

enum program_state _state = RUNNING; //should only be read from other sources
enum command_flag _command = DEFAULT;
/*
* this function return the state of the main process
* @param
* @return
*/
enum program_state get_program_state()
{
    return _state;
}

/*
* call create job function handles job submission
* @param
* @return
*/
void call_create_job()
{
    char* job_name = malloc(sizeof(char) * FLAG_INPUT_BUFFER);
    strcpy(job_name, "process");
    int job_execution_time = 10;
    int job_priority = 1;
    if(get_token(1) != NULL)
    {
        if(strcmp(get_token(1), ""))
        {
            strcpy(job_name, get_token(1));
        }
    }
    if(get_token(2) != NULL)
    {
        if(atoi(get_token(2)) != 0)
        {   
            job_execution_time = atoi(get_token(2));
        }
    }
    if(get_token(3) != NULL)
    {
        if(atoi(get_token(3)) != 0)
        {
            job_priority = atoi(get_token(3));
        }
    }
    submit_job(job_name, job_execution_time, job_priority);
}

void call_help_module()
{
    help(get_token(1));
}

int main(int argc, char** argv)
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
    if(argc > 0)
    {
        if(_state == ERROR)
        {
            fprintf(stderr,"program in erroneous state, exiting...");
            return 1;
        }
       //if child process, use set command instead and use data pthread_mutex_lock(&job_q_mu);m jobs array in benchmark
        pthread_mutex_lock(&pipe_mu);
        _command  = parse_command();
        pthread_mutex_unlock(&pipe_mu); 
        switch(_command)
        {
            case DONOTHING: break;
            case CMD_ERROR:
                printf("COMMAND ERROR.\n");
            break;
            case HELP:
                printf("help module in devleopment\n");
                //todo: interface with help module here.
                call_help_module();
            break;
            case RUN:
                if(access(get_token(1),F_OK) != -1)
                {
                    pthread_mutex_lock(&job_q_mu);
                    call_create_job();    
                    pthread_mutex_unlock(&job_q_mu);                
                }
                else
                {
                    printf("Process with name '%s' does not exist, please try again.\n", get_token(1));
                }
            break;
            case LIST:
                list_jobs();
            break;
            case CMD_FCFS:
                change_scheduling_policy(FCFS);
            break;
            case CMD_SJF:
                change_scheduling_policy(SJF);
            break;
            case CMD_PRIORITY:
                change_scheduling_policy(PRIORITY);
            break;
            case TEST:
                printf("Benchmark module in development");
                //call function that forks the process
            break;
            case QUIT:
                _state = EXIT;
            break;
            default:
                printf("ERROR: invalid command. (%s)\n", get_token(0));
            break;
        }
    }
    pthread_cond_signal(&queue_not_empty_cond_t);
    pthread_join(dispatching_t, NULL);
    pthread_join(scheduling_t, NULL);
    //TODO; join benchmark thread to main, printing statistics
    printf("DEBUG: message to display when exiting csubatch\n");
    //outputstats
    deconstruct_queue();
    pthread_mutex_destroy(&pipe_mu);
    pthread_mutex_destroy(&queue_t);
    pthread_cond_destroy(&queue_not_empty_cond_t);
    return 0;
}
