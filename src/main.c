#include "include/instruction.h"
#include "include/computer.h"

int main(int argc, char *argv[]) {
    // Memory init
    if (argc != 2) {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }
    // Program load and run
    load_program(argv[1]);
    instruction_counter = 0;
    run();
    
    printf("Program finished, %d instruction executed.\n", instruction_counter);
    return 0;
}
