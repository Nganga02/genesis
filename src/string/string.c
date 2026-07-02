#include "../include/string.h"
#include "../include/utils.h"

static char *t, *s;
static size_t index;

int str_length(char *string)
{
    index = 0;

    while (*t != '\0')
    {
        ++index;
        t++;
    }
    return index;
}

void str_copy(char *source, char *destination, int sizeof_destination)
{
    index = 0;

    if (str_length(source) <= sizeof_destination)
    {
        while (*source != '\0')
        {
            destination[index] = source[index];
            index++;
        }
        destination[index] = '\0';
    }
}

/**
 * How pattern matching is done:
 * Given two strings eg: Ian(string_1) and Lea(string_1)
 * We want ot identify which index the strings don't match
 *
 **/

int str_compare(char *string_1, char *string_2)
{
    int offset;

    // is either the string is empty or the string lengths are not equal return -1
    if ((*string_1 == '\0' || *string_2 == '\0') || (str_length(string_1) != str_length(string_2)))
    {
        return -1;
    }

    while (*t != '\0' || *s != '\0')
    {
        s = string_1, t = string_2;

        if (*(s++) != *(t++))
        {
            offset++;
        }
    }
    return offset;
}
