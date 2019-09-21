#ifndef JOBSUBMISSION
#define JOBSUBMISSION
#include "global.h"

enum scheduling_policy _scheduling_policy;

char* get_current_scheduling_policy();

int submit_job(char* job_name, int job_execution_time, int job_priority);

int change_scheduling_policy(enum scheduling_policy new_policy);

#endif