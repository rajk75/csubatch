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
#include "enums.h"

char* _input_tokens[] = {"command", "flag1", "flag2", "flag3", "flag4", "flag5", "flag6", "flag7" };
//char* _commands[] = {"help"};
//int _command_flag_no [] = {1};


char** get_tokens()
{
    return _input_tokens;
}

char* get_token(int i)
{
    return _input_tokens[i];
}

/*
* get_input takes a char pointer and writes input from scanf to it
* @pram dest; the destination to store input from stdin
*/
int get_input()
{
    char* cmd_input = (char*)(malloc(sizeof(char)*COMMAND_INPUT_BUFFER));
    printf(">");
    //output = scanf("%s", dest);
    if(fgets(cmd_input, COMMAND_INPUT_BUFFER, stdin) == NULL)
    {
        printf("fatal internal error when recieveing input.\n");
        return 2;
    }
    int vaild_num_flags = 7;
    const char delim  = ' ';
    _input_tokens[0] = strtok(cmd_input, &delim);

    for(int i = 1; i < vaild_num_flags; i++)
    {
        _input_tokens[i] = strtok(NULL, &delim);
    }
    //free(cmd_input);
    return 0;
}

/*
* tests inputted command and returns the command
*/
int parse_command()
{
    get_input();
    if(strstr(_input_tokens[0], "quit"))
    {
        return QUIT;
    }
    else if(strstr(_input_tokens[0], "help")) //strstr = if string contains substring "help"
    {
        
        return HELP;
    }
    else if(strstr(_input_tokens[0], "run"))
    {
        return RUN;
    }
    else if(strstr(_input_tokens[0], "list"))
    {
        //list_jobs();
        return  LIST;
    }
    else if(strstr(_input_tokens[0], "fcfs"))
    {
        //switch scheduling policy
        //change_scheduling_policy(0);
        return CMD_FCFS;
    }
    else if(strstr(_input_tokens[0], "sjf") )
    {
        //switch scheduling policy
        return CMD_SJF;
    }
    else if(strstr(_input_tokens[0], "priority"))
    {
        //switch scheduling policy
        return CMD_PRIORITY;
    }
    else
    {
        //invaild command message
        return CMD_INVAILD;
    }
}
    