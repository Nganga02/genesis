#include "../include/process.h"
#include "../include/heap.h"
#include "../include/print.h"

int processes_count;
int current_pid;
void process_init(void)
{
    processes_count = 0;
    current_pid = 0;
}
process_t *processes[15] = {0};
process_t *process_create(int *base_addr)
{

    process_t *new_process = kalloc(sizeof(new_process));
    new_process->pid = current_pid++;

    new_process->context.eax = 0;
    new_process->context.ecx = 0;
    new_process->context.edx = 0;
    new_process->context.ebx = 0;
    new_process->context.esp = 0;
    new_process->context.ebp = 0;
    new_process->context.esi = 0;
    new_process->context.edi = 0;
    new_process->context.eip = (unsigned int)base_addr;
    printi(new_process->context.eip);
    println();
    new_process->state = READY;
    new_process->base_address = base_addr;
    processes[new_process->pid] = new_process;

    processes_count++;
    return new_process;
}