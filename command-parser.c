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
#include "scheduling.c"

const char* PROGRAM_AUTHOR = "James & Raj";
const int COMMAND_INPUT_BUFFER = 10; //amount of characters is allowed at command input

/*
* get_input takes a char pointer and writes input from scanf to it
* @pram dest; the destination to store input from stdin
*/
void get_input(char* dest)
{
    int output = 0;
    printf(">");
    output = scanf("%s", dest);
    if(output < 0)
    {
        printf("fatal internal error when recieveing input from function 'scanf'.\n");
        exit(1);
    }
}

/*
* entry point to program, contains main logic for csubatch program.
*/
int main()
{
    printf("Weclome to %s's batch job scheduler Version 1.0\nType 'help' to find out more about CSUbatch commands.\n", PROGRAM_AUTHOR);
    //initalize a char pointer for command input
    char* cmd_input = (char*)(malloc(sizeof(char)*COMMAND_INPUT_BUFFER));
    int cont = 1;
    while(cont)
    {
        get_input(cmd_input);
        if(strcmp(cmd_input, "quit") == 0)
        {
            //todo: implement printing statistics.
            printf("message to display when exiting csubatch\n");
            cont = 0;
        }
        else if(strstr(cmd_input, "help")) //if string contains substring "help"
        {
            printf("DEBUG: help command was detected\n");
            //todo: call help module here.
        }
        else if(strstr(cmd_input, "run"))
        {
            //todo: need to split input into name, time, and prioity
            printf("WARNING: command spliting not implemented yet, submitting a default job.\n");
            submit_job("default-job", 10, 1);
        }
        else if(strstr(cmd_input, "list"))
        {
            //todo: implement a queue traversal, and format a table
            list_jobs();
        }
        else if(strcmp(cmd_input, "sjf") == 0)
        {
            //switch scheduling policy
            change_scheduling_policy(0);
        }
        else if(strcmp(cmd_input, "fcfs") == 0)
        {
            //switch scheduling policy
            change_scheduling_policy(1);
        }
        else if(strcmp(cmd_input, "priority") == 0)
        {
            //switch scheduling policy
            change_scheduling_policy(2);
        }
        else
        {
            //invaild command message
            printf("invalid command.\n"); 
        }
    }
    //functions to be called before exiting program.
    free(cmd_input);
    deconstruct_queue();
    //todo: write a free queue function that frees all the nodes. (implement in scheudling.c)
}