#include <pthread.h>

#include "csu-batch.h"
#include "job-queue.h"
#include "global.h"

void* scheduling_loop(void* signal_jsub)
{
    while(get_program_state() == RUNNING)
    {
        enum signal* lsignal_jsub = (enum signal*)(signal_jsub);
        struct node* head = peek();
        if(head != NULL)
        {
            if(head->data != NULL)
            {
                if(head->data->progress == ISNOTRUNNING)
                {
                    //set job to running
                    pthread_mutex_lock(&job_q_mu);
                    head->data->progress = ISRUNNING;
                    //TODO; signal dispatcher that a job is ready to be excev'd
                    if(signal_jsub != NULL)
                    {
                        //set the signal
                        *lsignal_jsub = READY;
                    }
                    pthread_mutex_unlock(&job_q_mu);
                }
            }
        }        
    }
    return NULL;
}
