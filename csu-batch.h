#ifndef CSUBATCH
#define CSUBATCH
#define PROGRAM_AUTHOR "James & Raj"
#define VERSION_NUM "1.0"
#include "global.h"
#include <pthread.h>

enum program_state get_program_state();
pthread_mutex_t pipe_mu;
pthread_mutex_t queue_t;
pthread_mutex_t queue_state_t;
pthread_cond_t queue_not_empty_cond_t;
#endif