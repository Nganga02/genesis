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

    if (str_length(file_name) < FILENAME_LENGTH)
    {
        str_copy(file_name,
                 metadata->file_name,
                 sizeof(metadata->file_name));
    }

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
    if (base_block->head == 0)
        return -1;

    char **list = kalloc(get_number_of_files() * sizeof(char *));
    metadata_t *current_file = load_metadata(base_block->head);
    int index = 0;

    while (1)
    {
        list[index] = current_file->file_name;
        int i;

        if ((i = current_file->next_file_address) == 0)
            break;

        current_file = load_metadata(i);
        index++;
    }
    return list;
}

int get_number_of_files()
{
    if (base_block->head == 0)
        return 0;
    int number_of_files = 0;

    metadata_t *current_file = load_metadata(base_block->head);

    while (1)
    {
        if (current_file->next_file_address == 0)
            break;
        number_of_files++;
        current_file = load_metadata(current_file->next_file_address);
    }

    return number_of_files;
}

char *read_file(char *filename)
{

    int address = get_address_by_filename(filename);

    if (address == 0)
        return 0;

    char *buffer = read_disk(address + 1); // the data is stored at the next level from the metadata block.
    return buffer;
}

int get_address_by_filename(char *filename)
{
    int address = 0;
    if (base_block->head == 0)
        return 0;

    metadata_t *current_file = load_metadata(base_block->head);

    int index = 0;

    while (1)
    {

        if (str_compare(filename, current_file->file_name) == 0)
        {
            return index == 0 ? base_block->head : address;
        }
        if (current_file->next_file_address == 0)
            break;
        address = current_file->next_file_address;
        current_file = load_metadata(current_file->next_file_address);
        index++;
    }

    return address;
}

/**
 * First we make it exist then we fine tune it
 */
int get_previous_file_address(int address)
{

    metadata_t *previous_file = load_metadata(base_block->head);
    int previous_address = base_block->head;
    if (base_block->head == 0)
        return 0;
    while (1)
    {
        if (previous_file->next_file_address == address)
            return previous_address;

        previous_address = previous_file->next_file_address;
        previous_file = load_metadata(previous_file->next_file_address);
    }
}

void delete_file(char *filename)
{
}
