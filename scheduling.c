#include <pthread.h>
#include <stdio.h>
#include "csu-batch.h"
#include "job-queue.h"
#include "global.h"
#include "dispatching.h"

void* scheduling_loop(void* signal_jsub)
{
    
    //pthread_mutex_lock(&queue_t);
    while(get_program_state() == RUNNING || peek()!= NULL)
    {
        //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[scheduling thread] cond wait for job queue state");
        pthread_cond_wait(&queue_not_empty_cond_t, &queue_state_t);
        //pthread_mutex_lock(&queue_state_t);
        enum signal* lsignal_jsub = (enum signal*)(signal_jsub);
        struct node* head = peek();    
        if(head != NULL)
        {
            if(head->data != NULL)
            {
                if(head->data->progress == ISNOTRUNNING)
                {
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[scheduling thread] obtaining lock for job queue");
                    pthread_mutex_lock(&job_q_mu);
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[scheduling thread] obtained lock for job queue");
                    head->data->progress = ISRUNNING;
                    //if(signal_jsub != NULL)
                    {
                        *lsignal_jsub = READY;
                    }
                    pthread_mutex_unlock(&job_q_mu);
                    //if(get_dispatching_state() == MCRO_BENCHMARK_MODE) puts("[scheduling thread] released lock for job queue");
                }
            }
        }  
        pthread_cond_signal(&queue_not_empty_cond_t);
        pthread_mutex_unlock(&queue_state_t);  
    }
    return NULL;
}
