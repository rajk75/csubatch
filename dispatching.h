#ifndef DISPATCHING
#define DISPATCHING
#include "global.h"
#include "csu-batch.h"

enum signal scheduling_sig;

enum signal* get_scheduling_sig();

void* dispatching_loop();

#endif