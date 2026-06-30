#include "../include/heap.h"

unsigned int heap_base;

static heap_t heap = {0};

void heap_init(void)
{
    heap_base = 0X100000;
}




void *kalloc(int bytes)
{
    unsigned int new_object_address = heap_base;
    heap_base += bytes;
    return (void *)new_object_address;
}



/**
 * Flow of memory mapping
 * 
 * 1. point es:di at the destination buffer for the list
 * 2. clear ebx 
 * 3. setting edx to the magic number 0x534d4150
 * 4. setting eax to 0xe820
 * 5. set ecx to 24 
 * 6. call int15h
 * 
 * 
 * 
 * If the call is successful,
 * 1. eax will contain 0x534D4150 
 * and carry will be clear
 * 2. ebx has some none zero value and should be reserved for 
 * the next call to the function 
 * 3. cl will contain the no of bytes actually stored at es:di
 * 
 * 
 * For the subsequent calls
 * 1. increment di by the list entry size,
 * 2. reset eax to e820 and ecx to 24
 * 
 * 
 */