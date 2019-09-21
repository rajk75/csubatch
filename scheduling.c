#include <pthread.h>

#include "csu-batch.h"
#include "job-queue.h"
#include "global.h"

void* scheduling_loop(void* singal_jsub)
{
    while(get_program_state() == RUNNING)
    {
        //set head to run
        struct node* head = peek();
        if(head->data->progress == ISNOTRUNNING)
        {
            //TODO; signal dispatcher that a job is ready to be excev'd
            if(singal_jsub != NULL)
            {
                //do the signal
            }
            //set job to running
            head->data->progress = ISRUNNING;
        }
    }
    return NULL;
}
