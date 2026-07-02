#include "../include/ata.h"

#define READ_COMMAND 0X20
#define WRITE_COMMAND 0X30

void *read_disk_chs(int sector)
{
    dev_write_byte(BASE_PORT + 6, 0x0a0);
    dev_write_byte(BASE_PORT + 2, 1);
    dev_write_byte(BASE_PORT + 3, sector);
    dev_write_byte(BASE_PORT + 4, 0);
    dev_write_byte(BASE_PORT + 5, 0);
    dev_write_byte(BASE_PORT + 7, READ_COMMAND);

    wait_drive_until_ready();

    short *buffer = kalloc(SECTOR_SIZE);

    for (int current_byte = 0; current_byte < (SECTOR_SIZE / 2); current_byte++)
    {
        buffer[current_byte] = dev_read(BASE_PORT);
    }
    return buffer;
}

void wait_drive_until_ready()
{
    volatile int status = 0;

    do
    {
        status = dev_read(BASE_PORT + 7);
    } while ((status ^ 0x80) == 128);
}

void *read_disk(int address)
{
    dev_write_byte(BASE_PORT + 6, (0x0e0 | ((address & 0x0F000000) >> 24)));
    dev_write_byte(BASE_PORT + 2, 1);
    dev_write_byte(BASE_PORT + 3, address && 0x000000ff);
    dev_write_byte(BASE_PORT + 4, (address && 0x0000ff00) >> 8);
    dev_write_byte(BASE_PORT + 5, (address && 0x000000ff) >> 16);
    dev_write_byte(BASE_PORT + 7, READ_COMMAND);

    wait_drive_until_ready();

    short *buffer = kalloc(SECTOR_SIZE);

    for (int current_byte = 0; current_byte < (SECTOR_SIZE / 2); current_byte++)
    {
        buffer[current_byte] = dev_read(BASE_PORT);
    }
    return buffer;
}

/**
 * Writing to disk using both the CHS and LBA
 */

void write_disk_chs(int sector, short *buffer)
{
    dev_write_byte(BASE_PORT + 6, 0x0a0);
    dev_write_byte(BASE_PORT + 2, 1);
    dev_write_byte(BASE_PORT + 3, sector);
    dev_write_byte(BASE_PORT + 4, 0);
    dev_write_byte(BASE_PORT + 5, 0);
    dev_write_byte(BASE_PORT + 7, WRITE_COMMAND);

    wait_drive_until_ready();

    for (int current_word = 0; current_word < (SECTOR_SIZE / 2); current_word++)
    {
        dev_write_word(BASE_PORT, buffer[current_word]);
    }

    wait_drive_until_ready();
}

void write_disk(int address, short *buffer)
{
    dev_write_byte(BASE_PORT + 6, (0x0e0 | ((address & 0x0F000000) >> 24)));
    dev_write_byte(BASE_PORT + 2, 1);
    dev_write_byte(BASE_PORT + 3, address & 0x000000FF);
    dev_write_byte(BASE_PORT + 4, (address & 0x0000FF00) >> 8);
    dev_write_byte(BASE_PORT + 5, (address & 0x00FF0000) >> 16);
    dev_write_byte(BASE_PORT + 7, WRITE_COMMAND);

    wait_drive_until_ready();

    for (int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++)
        dev_write_word(BASE_PORT, buffer[currByte]);

    wait_drive_until_ready();
}
