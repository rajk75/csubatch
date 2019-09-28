#ifndef DISPATCHING
#define DISPATCHING
#include "global.h"
#include "csu-batch.h"

enum signal scheduling_sig;

enum signal* get_scheduling_sig();

enum dispatching_state get_dispatching_state();

void set_dispatching_state(enum dispatching_state new_state);

void* dispatching_loop();

#endif