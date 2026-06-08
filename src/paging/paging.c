#include "../include/paging.h"
#include "../include/heap.h"

unsigned int *page_directory;

int create_page_entry(
    int base_address,
    char present,
    char writable,
    char priviledge_level,
    char cache_enabled,
    char write_through_cache,
    char accessed,
    char page_size,
    char dirty)
{
    int entry = 0;
    entry |= present;
    entry |= writable << 1;
    entry |= priviledge_level << 2;
    entry |= write_through_cache << 3;
    entry |= cache_enabled << 4;
    entry |= accessed << 5;
    entry |= dirty << 6;
    entry |= page_size << 7;

    return base_address | entry;
}

void paging_init(void)
{
    unsigned int current_page_frame = 0;
    page_directory = kalloc(4 * 1024);

    for (int curr_pde = 0; curr_pde < PDE_NUM; curr_pde++)
    {
        unsigned int *page_table = kalloc(4 * PTE_NUM);

        for (int curr_pte = 0; curr_pte < PTE_NUM; curr_pte++, current_page_frame++)
        {
            page_table[curr_pte] = create_page_entry(
                current_page_frame * 4096,
                1, 0, 0, 1, 1, 0, 0, 0);
        }

        page_directory[curr_pde] = create_page_entry(
            page_table,
            1, 0, 0, 1, 1, 0, 0, 0
        );
    }

    load_page_directory();
    enable_paging();
}
