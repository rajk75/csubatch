#include <pthread.h>

#include "csu-batch.h"
#include "job-queue.h"
#include "global.h"

void* scheduling_loop(void* signal_jsub)
{
    
    //pthread_mutex_lock(&queue_t);
    while(get_program_state() == RUNNING || peek()!= NULL)
    {
        //pthread_mutex_lock(&queue_state_t);
        pthread_cond_wait(&queue_not_empty_cond_t, &queue_state_t);
        enum signal* lsignal_jsub = (enum signal*)(signal_jsub);
        struct node* head = peek();    
        if(head != NULL)
        {
            if(head->data != NULL)
            {
                if(head->data->progress == ISNOTRUNNING)
                {
                    pthread_mutex_lock(&job_q_mu);
                    head->data->progress = ISRUNNING;
                    if(signal_jsub != NULL)
                    {
                        *lsignal_jsub = READY;
                    }
                    pthread_mutex_unlock(&job_q_mu);
                }
            }
        }  
        pthread_cond_signal(&queue_not_empty_cond_t);
        pthread_mutex_unlock(&queue_state_t);   
    }
    return NULL;
}
