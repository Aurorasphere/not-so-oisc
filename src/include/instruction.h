#include <stdio.h>
#include <stdbool.h>

#define MEMORY_SIZE 65536

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char i8;

extern i8 memory[MEMORY_SIZE];
extern bool running;

void subleq(u16 a, u16 b, u16 next);
void set(u16 a, i8 value);
void in(u16 a);
void out(u16 a);
void hlt();
