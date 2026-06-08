#ifndef H_PAGING_H
#define H_PAGING_H

#define PDE_NUM 3 /*Page Directory Entries*/
#define PTE_NUM 1024 /*Page Tables Entries*/

extern void load_page_directory();
extern void enable_paging();

extern unsigned int *page_directory;

void paging_init(void);
int create_page_entry(
    int, 
    char, 
    char, 
    char, 
    char, 
    char, 
    char, 
    char, 
    char);
#endif /*H_PAGING_H*/