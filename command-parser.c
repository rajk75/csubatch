/*
Authors: James Ellerbee, Raj Kotak
Date: September 2019
Purpose: project1 for cpsc 4175.
*/

//from c lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
//from project
#include "command-parser.h"
#include "scheduling.c"

char* input_tokens[] = {"command", "flag1", "flag2", "flag3", "flag4", "flag5", "flag6", "flag7" };
char* commands[] = {"help"};
int command_flag_no [] = {1};

//int vaild_num_flags = 7;
/*
* get_input takes a char pointer and writes input from scanf to it
* @pram dest; the destination to store input from stdin
*/
void get_input()
{
    char* cmd_input = (char*)(malloc(sizeof(char)*COMMAND_INPUT_BUFFER));
    printf(">");
    //output = scanf("%s", dest);
    if(fgets(cmd_input, COMMAND_INPUT_BUFFER, stdin) == NULL)
    {
        printf("fatal internal error when recieveing input.\n");
        exit(1);
    }
    int vaild_num_flags = 7;
    const char delim  = ' ';
    input_tokens[0] = strtok(cmd_input, &delim);

    for(int i = 1; i < vaild_num_flags; i++)
    {
        input_tokens[i] = strtok(NULL, &delim);
    }
    free(cmd_input);
}

/*
* entry point to program, contains main logic for csubatch program.
*/
int main()
{
    printf("Weclome to %s's batch job scheduler Version 1.0\nType 'help' to find out more about CSUbatch commands.\n", PROGRAM_AUTHOR);
    //initalize a char pointer for command input
    
    int cont = 1;
    while(cont)
    {
        
        get_input();
        if(strstr(input_tokens[0], "quit"))
        {
            //todo: implement printing statistics,need benchmark module.
            printf("DEBUG: message to display when exiting csubatch\n");
            cont = 0;
        }
        else if(strstr(input_tokens[0], "help")) //strstr = if string contains substring "help"
        {
            printf("DEBUG: help command was detected\n");
            //todo: call help module here.
        }
        else if(strstr(input_tokens[0], "run"))
        {
            char* job_name = malloc(sizeof(char) * FLAG_INPUT_BUFFER);
            strcpy(job_name, "sample_job");
            int job_execution_time = 10;
            int job_priority = 1;

            if(input_tokens[1] != NULL)
            {
                if(strcmp(input_tokens[1], ""))
                {
                    strcpy(job_name, input_tokens[1]);
                }
            }
            if(input_tokens[2] != NULL)
            {
                if(atoi(input_tokens[2]) != 0)
                {   
                    job_execution_time = atoi(input_tokens[2]);
                }
            }
            if(input_tokens[3] != NULL)
            {
                if(atoi(input_tokens[3]) != 0)
                {
                    job_priority = atoi(input_tokens[3]);
                }
            }
            submit_job(job_name, job_execution_time, job_priority);
        }
        else if(strstr(input_tokens[0], "list"))
        {
            //todo: implement a queue traversal, and format a table
            list_jobs();
        }
        else if(strstr(input_tokens[0], "fcfs"))
        {
            //switch scheduling policy
            change_scheduling_policy(0);
        }
        else if(strstr(input_tokens[0], "sjf") )
        {
            //switch scheduling policy
            change_scheduling_policy(1);
        }
        else if(strstr(input_tokens[0], "priority"))
        {
            //switch scheduling policy
            change_scheduling_policy(2);
        }
        else
        {
            //invaild command message
            printf("ERROR: invalid command.\n"); 
        }
    }
    //functions to be called before exiting program.
    deconstruct_queue();
    //todo: write a free queue function that frees all the nodes. (implement in scheudling.c)
}