#ifndef GLOBAL
#define GLOBAL

enum program_state
{
    RUNNING = 0,
    EXIT = 1,
    ERROR = 2
};

enum command_flag
{
    CMD_ERROR = 10,
    CMD_INVAILD = 11,
    HELP = 12,
    RUN = 13,
    LIST = 14,
    CMD_FCFS = 15,
    CMD_SJF = 16,
    CMD_PRIORITY = 17,
    TEST = 18,
    QUIT = 19,
    DEFAULT = 101
};
enum scheduling_policy
{
    FCFS = 20,
    SJF = 21,
    PRIORITY = 22
};
#endif