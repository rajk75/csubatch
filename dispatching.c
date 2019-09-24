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
        if(scheduling_sig == READY)
        {
            //printf("[DISPATCHING TRHEAD] DEBUG: ready to excev\n");
            //prototype_dispatching();
            int status;
            char *my_args[5];
            my_args[0] = "process";
            my_args[1] = "-help";
            my_args[2] = "-setup";
            my_args[3] = NULL;
            switch(pid = fork())
            {
                case -1:
                    perror("error when creating dispatching process");
                break;
                case 0:
                    //pid is child process
                    //execv replaces the process returned by the fork, therefore no extra precuations need to be taken
                    //get head process name
                    execv("process", my_args);
                    perror("dispatching loop: execv failed.");
                    exit(EXIT_FAILURE);
                break;
                default:
                    //wait for process to finish.
                    while(waitpid(pid, &status, WNOHANG) == 0);
                    //after execution dequeue the job and notify that a job has completed
                    remove_job();
                break;
            } 
           scheduling_sig = NOTREADY;
        }
    }
    return NULL;
}