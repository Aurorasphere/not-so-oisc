CC = gcc
CFLAGS = -Wall -g  

COMPUTER_TARGET = bin/oiscomputer
ASSEMBLER_TARGET = bin/assembler

COMPUTER_SRCS = src/main.c src/include/instruction.c src/include/computer.c
ASSEMBLER_SRCS = src/assembler.c

COMPUTER_HEADERS = src/include/instruction.h src/include/computer.h

COMPUTER_OBJS = $(COMPUTER_SRCS:.c=.o)
ASSEMBLER_OBJS = $(ASSEMBLER_SRCS:.c=.o)

$(COMPUTER_TARGET): $(COMPUTER_OBJS)
	$(CC) $(CFLAGS) -o $(COMPUTER_TARGET) $(COMPUTER_OBJS)

$(ASSEMBLER_TARGET): $(ASSEMBLER_OBJS)
	$(CC) $(CFLAGS) -o $(ASSEMBLER_TARGET) $(ASSEMBLER_OBJS)

%.o: %.c $(COMPUTER_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(COMPUTER_OBJS) $(ASSEMBLER_OBJS) $(COMPUTER_TARGET) $(ASSEMBLER_TARGET)

