#include "./include/print.h"
#include "./include/paging.h"
#include "./include/scheduler.h"
#include "./include/heap.h"
#include "./include/filesystem.h"
#include "./include/bootinfo.h"


void processA();
void processB();
void processC();
void processD();

void kernel_main(struct boot_info_t *boot_info)
{
    println();
    printi(boot_info->range_set[0].range_start);
    printi(boot_info->count);
    process_t *p1, *p2, *p3, *p4;

    
    heap_init();
    paging_init();
    screen_init();
    process_init();
    scheduler_init();
    file_system_init();
    // println();
    // print("\nboot info address: ");
    // printi((boot_info->range_set)[0].range_start);
    // println();
    printf("\nBoot info address %d\n", (boot_info->range_set)[0].range_start);
    printf("Welcome to 539kernel! \nWe are in protected mode\n%x\n", 539);
    printf("Range count: %d\n", boot_info->count);
   


    p1 = process_create(&processA);
  
    p2 = process_create(&processB);
 
    p3 = process_create(&processC); 
   
    p4 = process_create(&processD); 
   

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
