#ifndef H_PRINT_H
#define H_PRINT_H
extern volatile unsigned char *video;
extern int nextTextPosition;
extern int currentLine;

void screen_init(void);
void print(char *);
void println();
void printi(int);

#endif /*H_PRINT_H*/