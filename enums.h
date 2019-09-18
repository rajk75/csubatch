#ifndef ENUMS
#define ENUMS

enum program_state
{
    RUNNING = 0,
    EXIT = 1,
    ERROR = 2
};

enum command_flag
{
    CMD_ERROR = 3,
    CMD_INVAILD = 12,
    HELP = 4,
    RUN = 5,
    LIST = 6,
    CMD_FCFS = 7,
    CMD_SJF = 8,
    CMD_PRIORITY = 9,
    TEST = 10,
    QUIT = 11,
    DEFAULT = 100
};
enum scheduling_policy
{
    FCFS = 30,
    SJF = 31,
    PRIORITY = 32
};
#endif