#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "dispatching.h"
#include "global.h"
#include "job-queue.h"



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
        //use pthread_cond to suspend the thread.
        pthread_mutex_lock(&queue_state_t);
        pthread_cond_wait(&queue_not_empty_cond_t, &queue_state_t);
        if(scheduling_sig == READY)
        {
            int status;
            char *process_args[5];
            process_args[0] = "process";
            process_args[1] = "-help";
            process_args[2] = "-setup";
            process_args[3] = NULL;
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
                    //wait for process to finish.
                    while(waitpid(pid, &status, WNOHANG) == 0);
                    //after execution dequeue the job and notify that a job has completed
                    pthread_mutex_lock(&job_q_mu);
                    remove_job();
                    pthread_mutex_unlock(&job_q_mu);
                break;
            } 
           scheduling_sig = NOTREADY;
        }
        pthread_mutex_unlock(&queue_state_t);
    }
    return NULL;
}