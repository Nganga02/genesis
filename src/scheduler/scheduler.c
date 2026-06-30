#include "../include/scheduler.h"
#include "../include/print.h"

int next_sch_pid;
int curr_sch_pid;
process_t *next_process;
void scheduler_init(void)
{
    next_sch_pid = 0;
    curr_sch_pid = 0;
}

process_t *get_next_process(void)
{
    next_process = processes[next_sch_pid];

    curr_sch_pid = next_sch_pid;
    next_sch_pid++;
    next_sch_pid = next_sch_pid % processes_count;

    return next_process;
}

void scheduler(int eip, int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
    print(" EAX = ");
    printi(eax);
    print(" ");
    print("EIP = ");
    printi(eip);
    println();

    print("current schedule pid = ");
    printi(curr_sch_pid);
    println();
    process_t *current_process = processes[curr_sch_pid];

    next_process = get_next_process();

    if (current_process->state == RUNNING)
    {
        
        current_process->context.eax = eax;
       
        current_process->context.ecx = ecx;
        
        current_process->context.edx = edx;
        
        current_process->context.ebx = ebx;
       
        current_process->context.esp = esp;
        
        current_process->context.ebp = ebp;
        
        current_process->context.esi = esi;
       
        current_process->context.edi = edi;
        
        current_process->context.eip = eip;
       
    }

    current_process->state = READY;

    load_context();
    


    next_process->state = RUNNING;

}

void run_next_process(void)
{
    print("next process eip = ");
    printi(next_process->context.eip);
    println();
    asm(
        "   sti; \
            jmp *%0" ::
            "r"(next_process->context.eip)
    );
}