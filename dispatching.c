#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "dispatching.h"
#include "global.h"
#include "job-queue.h"

enum dispatching_state _mode = DEFAULT_MODE;

enum dispatching_state get_dispatching_state()
{
    return _mode;
}

void set_dispatching_state(enum dispatching_state new_state)
{
    _mode = new_state;
}

enum signal* get_scheduling_sig()
{
    scheduling_sig = NOTREADY;
    return &scheduling_sig;
}

/**
 * Function used for debugging dispatching functionality
 */
void prototype_dispatching()
{
    //printf("[DISPATCHING THREAD] DEBUG: removing completed job in 5...\n");
    sleep(10);
    remove_job();
    //printf("[DISPATCHING THREAD] DEBUG: job removed\n");
}

void* dispatching_loop()
{
    pid_t pid;
    while(get_program_state() == RUNNING || peek() != NULL)
    {
        //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[dispatching thread] cond wait for job queue state");
        pthread_cond_wait(&queue_not_empty_cond_t, &queue_state_t);
        if(scheduling_sig == READY)
        {
            int status;
            char*process_args[5];
            if(_mode != MCRO_BENCHMARK_MODE)
            {
                process_args[0] = "process";
                process_args[1] = "-help";
                process_args[2] = "-setup";
                process_args[3] = NULL;
            }
            else
            {
                //process_args[0] = itoa(peek()->data->execution_time,10,10); //use sprintf instead
                process_args[0] = malloc(sizeof(char) *10);
                sprintf(process_args[0], "%d", peek()->data->execution_time);
            }
            
            switch(pid = fork())
            {
                case -1:
                    perror("error when creating dispatching process");
                break;
                case 0:
                    //pid is child process
                    //execv replaces the process returned by the fork, therefore no extra precuations need to be taken
                    execv(peek()->data->name, process_args);
                    perror("dispatching loop: execv failed.");
                    exit(EXIT_FAILURE);
                break;
                default: 
                    //clock job in progress
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[dispatching thread] waiting for process");
                    while(waitpid(pid, &status, WNOHANG) == 0);
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[dispatching thread] obtaining lock for job queue");
                    pthread_mutex_lock(&job_q_mu);
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[dispatching thread] obtained lock for job queue");
                    remove_job();
                    pthread_mutex_unlock(&job_q_mu);
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[dispatching thread] released lock for job queue");
                break;
            } 
           scheduling_sig = NOTREADY;
        }
        pthread_cond_signal(&queue_not_empty_cond_t);
        pthread_mutex_unlock(&queue_state_t);
    }
    return NULL;
}