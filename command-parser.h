/*
* header file for command_parser.h
*/
#ifndef COMMANDPARSER
#define COMMANDPARSER
#define COMMAND_INPUT_BUFFER 30 //amount of characters is allowed at command input
#define FLAG_INPUT_BUFFER 10

void _init_command_parser();
char** get_tokens();
char* get_token(int i);
int get_input();
int parse_command();
int count_num_flags(char* cmd_input);
char** parse_flags(int num_flags, char* input);
#endif