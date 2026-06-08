#ifndef H_SCHEDULER_H
#define H_SCHEDULER_H

#include "process.h"


extern int next_sch_pid, curr_sch_pid;

extern process_t *next_process;


void scheduler_init(void);
process_t *generate_next_process(void);
void scheduler(int, int, int, int, int, int, int, int, int);
void run_next_process(void);

#endif /*H_SCHEDULER_H*/
