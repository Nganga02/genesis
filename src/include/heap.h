#ifndef H_HEAP_H
#define H_HEAP_H
extern unsigned int heap_base;


#define ALIGNMENT 8
#define ALIGN(size) (((size)+(ALIGNMENT-1)) & ~(ALIGNMENT-1))

typedef int size_t;
/**
 * Memory block data structure
 */
typedef struct block_t{
    size_t size;
    int is_free;
    struct block_t *next;
    struct block_t *prev;
    char *data[1]; //pointer to one byte of memory
}block_t;

typedef struct{
    block_t *free_list;
    block_t *used_list;
    size_t total_size;
    size_t used_size;
}heap_t;
void heap_init();
void *kalloc(int);

#endif /*H_HEAP_H*/