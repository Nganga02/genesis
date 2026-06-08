#include "../include/print.h"


volatile unsigned char *video;
int nextTextPosition, currentLine;

void screen_init(void)
{
    video = (volatile unsigned char *)0xB8000;
    nextTextPosition = 0;
    currentLine = 0;
}
void print(char *string)
{
    int curr_char_location_in_vid_mem, curr_color_location_in_vid_mem;
    while (*string != '\0')
    {
        curr_char_location_in_vid_mem = nextTextPosition * 2;
        curr_color_location_in_vid_mem = curr_char_location_in_vid_mem + 1;

        video[curr_char_location_in_vid_mem] = *string;
        video[curr_color_location_in_vid_mem] = 15;

        nextTextPosition += 1;

        string++;
    }
}

void println()
{
    nextTextPosition = ++currentLine * 80;
}

void printi(int number)
{
    char *digit_to_str[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};


    if(number >= 0 && number <= 9)
    {
        print(digit_to_str[number]);
        return;
    }

    else
    {
        int remainder  = number % 10;
        number = number/10;

        printi(number);
        printi(remainder);
    }
}