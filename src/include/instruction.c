#include "instruction.h"
#include "computer.h"

i8 memory[MEMORY_SIZE];
bool running = true;

void subleq(u16 a, u16 b, u16 next) {

    memory[b] = memory[b] - memory[a];

    if (memory[b] <= 0) {
        PC = next;
    } else {
        PC += 7;
    }
}

void set(u16 a, i8 value) {
    memory[a] = value;
    PC += 4;
}

void in(u16 a) {
    i8 input;
    scanf("%hhd", &input);
    if (input > 127) input = 127;
    memory[a] = input;
    PC += 3;
}

void out(u16 a) {
    printf("%d\n", memory[a]);
    PC += 3;
}

void hlt() {
    running = 0;
}
