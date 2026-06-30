#include "./include/print.h"
#include "./include/paging.h"
#include "./include/scheduler.h"

void processA();
void processB();
void processC();
void processD();

void kernel_main(void)
{
    process_t *p1, *p2, *p3, *p4;

    heap_init();
    screen_init();
    process_init();
    scheduler_init();
    print("Welcome to 539kernel!");
    println();
    print("We are now in Protected-mode");
    println();
    printi(539);
    println();

    process_create(&processA);
    print("process count");
    printi(processes_count);
    println();
    print(" A process.eip = ");
    printi(&processA);
    println();
    p2 = process_create(&processB);
    print("process count");
    printi(processes_count);
    println();
    print(" A process.eip = ");
    printi(&processB);
    println();
    p3 = process_create(&processC); 
    print("process count");
    printi(processes_count);
    println();
    print(" A process.eip = ");
    printi(&processC);
    println();

    while (1)
        ;
}

void interrupt_handler(int interrupt_number)
{
    println();
    print("Interrupt number   ");
    printi(interrupt_number);
}

void processA()
{

    print("Process A,");

    while (1)
        asm("mov $5390, %eax");
}

void processB()
{

    print("Process B,");

    while (1)
        asm("mov $5391, %eax");
}

void processC()
{

    print("Process C,");

    while (1)
        asm("mov $5392, %eax");
}

void processD()
{
    print("Process D,");

    while (1)
        asm("mov $5393, %eax");
}
