#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MEMORY_SIZE 65536
#define MAX_LABELS 256
#define MAX_LABEL_LEN 32

typedef unsigned char u8;
typedef unsigned short u16;

u8 memory[MEMORY_SIZE];
u16 pc = 0;

typedef struct {
    char name[MAX_LABEL_LEN];
    u16 address;
} label;

label labels[MAX_LABELS];
int label_count = 0; 
u16 pending_labels[MAX_LABELS];
int pending_label_count = 0;

void remove_comments(char* line) {
    char* comment_start = strstr(line, "//");
    if (comment_start) *comment_start = '\0'; 
    
    char* multi_line_comment_start = strstr(line, "/*");
    while (multi_line_comment_start) {
        char* multi_line_comment_end = strstr(multi_line_comment_start, "*/");
        if (multi_line_comment_end) 
            memset(multi_line_comment_start, ' ', multi_line_comment_end - multi_line_comment_start + 2);
        else {
            *multi_line_comment_start = '\0';
            break;
        }
        multi_line_comment_start = strstr(multi_line_comment_start, "/*");
    }
}

void clean_whitespace(char* line) {
    char* src = line;
    char* dst = line;
    int in_whitespace = 0;

    while (*src) {
        if (isspace((unsigned char)*src)) {
            if (!in_whitespace) {
                *dst++ = ' '; 
                in_whitespace = 1;
            }
        } 
        else {
            *dst++ = *src;  
            in_whitespace = 0;
        }
        src++;
    }
    *dst = '\0';
}

void add_label(const char* name, u16 address){
    if (label_count >= MAX_LABELS){
        printf("Error: too many labels.\n");
        exit(1);
    }
    strcpy(labels[label_count].name, name);
    labels[label_count].address = address;
    label_count++;
}

u16 get_label_address(const char* name){
    for (int i = 0; i < label_count; i++){
        if (strcmp(labels[i].name, name) == 0) return labels[i].address;
    }
    printf("Error: Undefined label: %s\n", name);
    exit(1);
}

int is_label(const char *str){
    return isalpha(str[0]);
}

u16 parse_address(const char* token){
    if ((token[0] == '0' && token[1] == 'x') || (token[0] == '0' && token[1] == 'X')) return strtol(token, NULL, 16);
    return get_label_address(token);
}

void add_instruction(const char* instruction, const char* a, const char* b, const char* next_address){
    if (strcmp(instruction, "subleq") == 0){
        u16 address_a = parse_address(a);
        u16 address_b = parse_address(b);
        u16 address_next;

        if (is_label(next_address)){
            pending_labels[pending_label_count++] = pc + 5;
            address_next = 0;
        }
        else address_next = parse_address(next_address);

        memory[pc++] = 0x00;
        memory[pc++] = (address_a >> 8) & 0xFF;
        memory[pc++] = address_a & 0xFF;
        memory[pc++] = (address_b >> 8) & 0xFF;
        memory[pc++] = address_b & 0xFF;
        memory[pc++] = (address_next >> 8) & 0xFF;
        memory[pc++] = address_next & 0xFF;
    }

    else if (strcmp(instruction, "set") == 0){
        u16 address = strtol(a, NULL, 16);
        u16 value = strtol(b, NULL, 16);

        memory[pc++] = 0x01;
        memory[pc++] = (address >> 8) & 0xFF;
        memory[pc++] = address & 0xFF;
        memory[pc++] = value & 0xFF;
    }

    else if (strcmp(instruction, "in") == 0){
        u16 address = strtol(a, NULL, 16);

        memory[pc++] = 0x02;
        memory[pc++] = (address >> 8) & 0xFF;
        memory[pc++] = address & 0xFF;
    }

    else if (strcmp(instruction, "out") == 0){
        u16 address = strtol(a, NULL, 16);

        memory[pc++] = 0x03;
        memory[pc++] = (address >> 8) & 0xFF;
        memory[pc++] = address & 0xFF;
    }

    else if (strcmp(instruction, "hlt") == 0){
        memory[pc++] = 0x04;
    }

    else {
        printf("Error: Undefined instruction: %s\n", instruction);
        exit(1);
    }
}

void assemble(const char* input_file) {
    FILE* file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char line[256], instruction[10], a[10], b[10], next_address[10];
    while (fgets(line, sizeof(line), file)) {
        remove_comments(line);
        clean_whitespace(line);

        char* colon = strchr(line, ':');
        if (colon) {
            char label[MAX_LABEL_LEN];
            sscanf(line, "%[^:]:", label);
            add_label(label, pc);           
            continue;                       
        }

        char* token = strtok(line, " \t");
        if (token == NULL) continue;

        char* instruction = token;
        char* a = strtok(NULL, " \t");
        char* b = strtok(NULL, " \t");
        char* next_address = strtok(NULL, " \t");

        if (next_address) 
            add_instruction(instruction, a, b, next_address); 
        else if (b) 
            add_instruction(instruction, a, b, NULL);  
        else if (a) 
            add_instruction(instruction, a, NULL, NULL); 
        else 
            add_instruction(instruction, NULL, NULL, NULL);  
        
    }

    fclose(file);

    for (int i = 0; i < pending_label_count; i++) {
        u16 address = get_label_address(labels[i].name);
        memory[pending_labels[i]] = (address >> 8) & 0xFF;
        memory[pending_labels[i] + 1] = address & 0xFF;
    }
}

void save_to_file(const char* output_file){
    FILE* bin_file = fopen(output_file, "wb");
    if (bin_file == NULL){
        printf("Error: Unable to open file.\n");
        exit(1);
    }
    fwrite(memory, sizeof(u8), pc, bin_file);
    fclose(bin_file);
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Usage: %s <input.s> <output.bin>\n", argv[0]);
        return 1;
    }
    pc = 0;
    assemble(argv[1]);
    save_to_file(argv[2]);

    return 0;
}
