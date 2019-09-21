#include <stdio.h>
#include <unistd.h>

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
    printf("[DISPATCHING THREAD] DEBUG: removing completed job in 5...\n");
    sleep(5);
    remove_job();
    printf("[DISPATCHING THREAD] DEBUG: job removed\n");
}

void* dispatching_loop()
{
    //pid_t pid;
    while(get_program_state() == RUNNING)
    {
        if(scheduling_sig == READY)
        {
            printf("[DISPATCHING TRHEAD] DEBUG: ready to excev\n");
            prototype_dispatching();
            /*
            switch(pid = fork())
            {
                case -1:
                    perror("error when creating dispatching process");
                break;
                case 0:
                    //pid = child process
                    //execv replaces the process returned by the fork, therefore no extra precuations need to be taken
                    //get head process name
                    //execv
                    exit(EXIT);
                break;
                default:
                    while(waitpid(pid, &status, WNOHANG) == 0);
                    //after execution dequeue the job and notify that a job has completed 
                break:
            }
            */
           scheduling_sig = NOTREADY;
        }
    }
    return NULL;
}