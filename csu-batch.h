#ifndef CSUBATCH
#define CSUBATCH
#define PROGRAM_AUTHOR "James & Raj"
#define VERSION_NUM "1.0"
#include "global.h"
#include <pthread.h>

enum program_state get_program_state();
pthread_mutex_t pipe_mu;
#endif