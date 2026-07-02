
#ifndef H_FILESYSTEM_H
#define H_FILESYSTEM_H
#define BASE_BLOCK_ADDRESS 100
#define FILENAME_LENGTH 256

typedef struct{
    int head, tail;
}base_block_t;

typedef struct{
    char file_name[FILENAME_LENGTH];
    int next_file_address;
}metadata_t;

extern base_block_t *base_block;


void file_system_init(void);
void create_file(char *, char *);
char **list_files(void);
char *read_file(char *);


/**
 * These are metadata files
 */
metadata_t *load_metadata(int);
int get_address_by_filename(char *);
int get_previous_file_address(int);
int get_number_of_files();

#endif /*H_FILESYSTEM_H*/