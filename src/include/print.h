#ifndef H_PRINT_H
#define H_PRINT_H
extern volatile unsigned char *video;
extern int nextTextPosition;
extern int currentLine;


#define _newline_           '\n'
#define _tab_               '\t'
#define _carriage_return_   '\r'




typedef __builtin_va_list __va_list__;

#define __va_start(argptr, last)    __builtin_va_start(argptr, last)
#define __va_end(argptr)            __builtin_va_end(argptr)
#define __va_arg(argptr, type)      __builtin_va_arg(argptr, type)
#define __va_copy(argptr, src)      __builtin_va_copy(argptr, src)

void screen_init(void);
void print(char *);
void println();
void printi(int);
int printf(char *, ...);


#endif /*H_PRINT_H*/