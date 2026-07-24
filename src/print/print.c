#include "../include/print.h"
#include "../include/utils.h"

volatile unsigned char *video;
int nextTextPosition, currentLine;

static const char *digit_to_str[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

void screen_init(void)
{
    video = (volatile unsigned char *)0xB8000;
    nextTextPosition = 0;
    currentLine = 0;
}
void print(char *string)
{
    while (*string != '\0')
    {
        kputc(*string, &nextTextPosition);
        string++;
    }
}

void kputc(char c, int *nextTextPositionPtr)
{

    video[*nextTextPositionPtr * 2] = c;
    video[*nextTextPositionPtr * 2 + 1] = 0xf;

    *nextTextPositionPtr += 1;
}

void println()
{
    nextTextPosition = ++currentLine * 80;
}

void printi(int number)
{
    if (number >= 0 && number <= 9)
    {
        print(digit_to_str[number]);
        return;
    }

    else
    {
        int remainder = number % 10;
        number = number / 10;

        printi(number);
        printi(remainder);
    }
}

void print_uint(uint32_t number)
{
    if (number >= 0 && number <= 9)
    {
        print(digit_to_str[number]);
        return;
    }

    else
    {
        uint32_t remainder = number % 10;
        number = number / 10;

        printi(number);
        printi(remainder);
    }
}

void print_oct(uint32_t number)
{
    if (number >= 0 && number <= 7)
    {
        print(digit_to_str[number]);
        return;
    }
    else
    {
        int remainder = number % 8;
        number /= 8;
        print_oct(number);
        print_oct(remainder);
    }
}

void print_hex(uint32_t number)
{   
    if (number >= 0 && number <= 15)
    {
        print(digit_to_str[number]);
        return;
    }
    else
    {
        int remainder = number % 16;
        number /= 16;
        print_hex(number);
        print_hex(remainder);
    }
}

void print_pointer(uint32_ptr_t __restrict__ ptr)
{
    print_hex(ptr);
}

int printf(char *__restrict__ string_t, ...)
{
    __va_list__ args;
    __va_start(args, string_t);

    uint16_t formated_values = 0; 

    char *s = string_t;
    while (*s != '\0')
    {

        if (*s == '%')
        {
            s++;
            switch (*s)
            {
            case 's':
                const char *tmp = __va_arg(args, char *);
                print(tmp);
                formated_values++;
                break;

            case 'd':
                int d = __va_arg(args, int);
                printi(d);
                formated_values++;
                break;

            case 'u':
                unsigned int i = __va_arg(args, double);
                print_uint(i);
                formated_values++;
                break;

            case 'c':
                char c = (char)__va_arg(args, int);
                kputc(c, &nextTextPosition);
                formated_values++;
                break;

            case 'x':
            case 'X':
                uint32_t x = (uint32_t)__va_arg(args, uint32_t);
                print("0x");
                print_hex(x);
                formated_values++;
                break;

            case 'o':
                uint32_t o = (uint32_t)__va_arg(args, uint32_t);
                print("0");
                print_oct(o);
                formated_values++;
                break;
                // applying octal

            case 'e/E':
                // applying sientific notation

            case 'p':
                uint32_ptr_t p = (uint32_ptr_t)__va_arg(args, uint32_t);
                print("0x");
                print_pointer(p);
                formated_values++;
                break;

            case '%':
                // printing the literal character
            }
            s++;
        }
        else if(*s == '\n')
        {
            println();
            s++;
        }
        else
        {
            kputc(*s, &nextTextPosition);
            s++;
        }
    }
    __va_end(args);
    return formated_values;
}


