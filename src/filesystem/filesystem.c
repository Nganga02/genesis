#include "../include/filesystem.h"
#include "../include/ata.h"
#include "../include/string.h"
#include "../include/utils.h"

base_block_t *base_block;
void file_system_init()
{
    base_block = read_disk(BASE_BLOCK_ADDRESS);
}

static void update_base_block(int new_head, int new_tail)
{
    base_block->head = new_head;
    base_block->tail = new_tail;

    write_disk(BASE_BLOCK_ADDRESS, base_block);
}

/**
 * In creating a file, we are figuring out if the base block contains no files
 * and if so then append a file adjacent to the base block address.
 *
 * if the head is zero? add one to the value of tail the next adjacent block
 *
 * if the value is != 0 we use tail
 *
 *      given tail initial position is x
 * :: before tail --> x
 *
 * after inserting out file
 *
 *  files address will be at point
 */
void create_file(char *file_name, char *buffer)
{
    int metadata_lba = (base_block->head == 0) ? base_block->head + 1 : base_block->tail + 2;

    int file_lba = metadata_lba + 1;

    metadata_t *metadata = kalloc(sizeof(*metadata));

    metadata->next_file_address = 0;

    int current_index = 0;

    size_t i;

    for (i = 0; i < FILENAME_LENGTH; i++)
    {
        if (file_name[i] == '\0')
            break;
    }

    if (i == FILENAME_LENGTH)
    {
        // No null terminator found within the allowed length.
        // Reject the filename.
        return;
    }

    /* Safe to copy: we know the whole string fits. */
    memcpy(metadata->file_name, file_name, i + 1);
    

    write_disk(metadata_lba, metadata);

    write_disk(file_lba, buffer);

    if (base_block->head == 0)
    {
        update_base_block(metadata_lba, metadata_lba);
    }
    else
    {
        metadata_t *tail_metadata = load_metadata(base_block->tail);

        tail_metadata->next_file_address = metadata_lba; // Updating the tail metadata address to point to the newly added tail

        write_disk(base_block->tail, tail_metadata);
        update_base_block(base_block->head, metadata_lba);
    }
}



metadata_t *load_metadata(int address)
{
    metadata_t *metadata = read_disk(address);

    return metadata;
}

char **list_files(void)
{
    if(base_block->head == 0)
        return -1;

    char **list = kalloc(get_number_of_files() * sizeof(char*));
    metadata_t *current_file = load_metadata(base_block->head);
    int index = 0;

    while(1)
    {
        list[index] = current_file->file_name;
        int i;

        if((i = current_file->next_file_address)== 0)break;

        current_file = load_metadata(i);
        index++;
    }
    return list;

}

int get_number_of_files()
{
    if(base_block->head == 0)
        return 0;
    int number_of_files = 0;

    metadata_t *current_file = load_metadata(base_block->head);

    while(1)
    {
        if(current_file->next_file_address == 0) break;
        current_file = load_metadata(current_file->next_file_address);
        number_of_files++;
    }

    return number_of_files;
}