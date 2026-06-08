#ifndef H_PROCESS_H
#define H_PROCESS_H
typedef enum process_state
{
    READY,
    RUNNING
}process_state_t;

typedef struct process_context
{
    int eax, ebx, ecx, edx, esp, ebp, esi, edi, eip;
}process_context_t;


typedef struct process
{
    int pid;
    process_context_t context;
    process_state_t state;
    int *base_address;
}process_t;


extern process_t *processes[15];

extern int processes_count, current_pid;

void process_init(void);
process_t *process_create(int *base_addr);

#endif /*H_PROCESS_H*/