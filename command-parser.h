/*
* header file for command_parser.h
*/
#ifndef COMMAND_INCL_GUARD
#define PROGRAM_AUTHOR "James & Raj"
#define COMMAND_INPUT_BUFFER 30 //amount of characters is allowed at command input
#define FLAG_INPUT_BUFFER 10
//begin function declarations
void get_input(char* dest);
int count_num_flags(char* cmd_input);
char** parse_flags(int num_flags, char* input);
#endif
#define COMMAND_INCL_GUARD