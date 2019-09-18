/*
Author: James Ellerbee
Purpose: Main for csu batch
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//header
#include "csu-batch.h"
//other c files
#include "command-parser.h"
#include "job-queue.h"
#include "enums.h"

enum program_state _state = RUNNING;
enum command_flag _command = DEFAULT;

void call_create_job()
{
    char* job_name = malloc(sizeof(char) * FLAG_INPUT_BUFFER);
    strcpy(job_name, "sample_job");
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

int main()
{
    printf("Weclome to %s's batch job scheduler Version %s\nType 'help' to find out more about CSUbatch commands.\n", PROGRAM_AUTHOR, VERSION_NUM);
    //TODO: pthread_create
    while(_state != EXIT)
    {
        if(_state == ERROR)
        {
            printf("program in erroneous state, exiting...");
            exit(1);
        }
        _command  = parse_command();
        switch(_command)
        {
            case CMD_ERROR:
                printf("COMMAND ERROR.\n"); 
            break;
            case HELP:
                printf("help module in devleopment\n");
                //todo: interface with help module here.
            break;
            case RUN:
                call_create_job();
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
                printf("benchmark module in development");
            break;
            case QUIT:
                _state = EXIT;
            break;
            default:
                printf("ERROR: invalid command.\n");
            break;
        }
    }
    //functions to be called before exiting program.
    printf("DEBUG: message to display when exiting csubatch\n");
    deconstruct_queue();
    //TODO: write a free queue function that frees all the nodes. (implement in scheudling.c)
}