#ifndef H_ATA_H
#define H_ATA_H
#define BASE_PORT 0X1F0
#define SECTOR_SIZE 512


void wait_drive_until_ready();


/**
 * The function reads the disk and returns the memory address
 * containing the data
 */
void *read_disk(int);
void write_disk(int, short *);

void *read_disk_chs(int);
void write_disk_chs(int, short *);


#endif /*H_ATA_H*/
    