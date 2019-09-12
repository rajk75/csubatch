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

#define PROGRAM_AUTHOR "James & Raj"
#define COMMAND_INPUT_BUFFER 30 //amount of characters is allowed at command input
#define FLAG_INPUT_BUFFER 10
/*
* get_input takes a char pointer and writes input from scanf to it
* @pram dest; the destination to store input from stdin
*/
void get_input(char* dest)
{
    printf(">");
    //output = scanf("%s", dest);
    if(fgets(dest, COMMAND_INPUT_BUFFER, stdin) == NULL)
    {
        printf("fatal internal error when recieveing input.\n");
        exit(1);
    }
    //printf("DEBUG: string read in; %s\n", dest);
}

/*
* the validate command input function handles testing the input against maximum number of expected flags for and input
* @param max_num_flags the maximum number of flags in the command
* @param cmd_input the command recieved from user
* @return 1 if the number of flags in the command input exceed max num flags
*/
int validate_command_input(int max_num_flags, char* cmd_input)
{
    //test if the number of flags in the input exceed max num of flags
    int num_flags = 0;
    char* cur_char = cmd_input;
    while(*cur_char != '\0')
    {
        if(*cur_char == '-')
        {
            num_flags++;
        }
        cur_char++;
    }
    printf("DEBUG: number of flags detected %d\n", num_flags);
    return num_flags > max_num_flags;
}

/*
* parse_flags function returns a char double pointer for all of the flags parsed from the input command, 
* assumes that the input has been validated
* @param max_num_flags the maximum number of flags in the command
* @param input the command 
*/
char** parse_flags(int max_num_flags, char* input)
{  
    //char flag_arr [max_num_flags][FLAG_INPUT_BUFFER];

    //char** for pointing to multiple flag char*
    char** flags = malloc(sizeof(char)* max_num_flags);
    
    //initialize a pointer at each index of the char **
    for(int char_index = 0; char_index < max_num_flags; char_index++)
    {
        flags[char_index] = malloc(sizeof(char)*FLAG_INPUT_BUFFER);
    }

    char flag[FLAG_INPUT_BUFFER];
    char* cur_char = input;

    //loop through char** and use strcpy copy from '-' to ' '
    for(int char_index = 0; char_index < max_num_flags; char_index++)
    {
        //move cur char to '-'
        while(*cur_char != '-')
        {
            cur_char++;
        }
        //add char to array
        int i = 0;
        while(*cur_char != ' ')
        {
            cur_char++;
            flag[i] = *cur_char;
        }
        printf("DEBUG: the flag: %s", &flag);
        strcpy(flags[char_index], flag);
    }

    return flags;
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
        if(strstr(cmd_input, "quit"))
        {
            //todo: implement printing statistics,need benchmark module.
            printf("message to display when exiting csubatch\n");
            cont = 0;
        }
        else if(strstr(cmd_input, "help")) //strstr = if string contains substring "help"
        {
            printf("DEBUG: help command was detected\n");
            //todo: call help module here.
        }
        else if(strstr(cmd_input, "run"))
        {
            if(validate_command_input(3, cmd_input))
            {
                printf("ERROR: too many arguments found in input.\n");
            }
            else
            {
                //todo: need to split input into name, time, and prioity
                printf("WARNING: command spliting not implemented yet, submitting a default job.\n");
                submit_job("default-job", 10, 1);
            } 
        }
        else if(strstr(cmd_input, "list"))
        {
            //todo: implement a queue traversal, and format a table
            list_jobs();
        }
        else if(strstr(cmd_input, "sjf") )
        {
            //switch scheduling policy
            change_scheduling_policy(0);
        }
        else if(strstr(cmd_input, "fcfs"))
        {
            //switch scheduling policy
            change_scheduling_policy(1);
        }
        else if(strstr(cmd_input, "priority"))
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