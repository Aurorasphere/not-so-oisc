CC = gcc
CFLAGS = -Wall -g  

TARGET = bin/oiscomputer

SRCS = src/main.c src/include/instruction.c src/include/computer.c
HEADERS = src/include/instruction.h src/include/computer.h

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

