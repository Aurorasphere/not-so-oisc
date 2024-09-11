#include "computer.h"
#include "instruction.h"

int instruction_counter = 0;
u16 PC = 0;

void load_program(const char *filename){
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }

    fread(memory, sizeof(u8), MEMORY_SIZE, file);
    fclose(file);
}

unsigned short read_16bit(signed char *memory, unsigned int pc) {
    return (memory[pc] << 8) | memory[pc + 1];
}

void run() {
    u8 instruction = 0;
    while (running) {
        instruction = memory[PC];
        instruction_counter++;

        switch (instruction) {

            case 0: 
            {
                u16 a = read_16bit(memory, PC + 1);
                u16 b = read_16bit(memory, PC + 3);
                u16 next = read_16bit(memory, PC + 5);
                subleq(a, b, next);
            }
            break;

            case 1:
            {
                u16 addr = read_16bit(memory, PC + 1);
                i8 value = memory[PC + 3];
                set(addr, value);
            }
            break;

            case 2:  
            {
                u16 addr = read_16bit(memory, PC + 1);
                in(addr);
            }
            break;

            case 3:  
            {
                u16 addr = read_16bit(memory, PC + 1);
                out(addr);
            }
            break;

            case 4:  
                hlt();  
            break;

            default:
                printf("Unknown instruction at PC: %04x\n", PC);
                exit(1);
            break;
        }
    }
}

