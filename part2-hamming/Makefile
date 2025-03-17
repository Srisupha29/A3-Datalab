# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# List of object files
OBJS = main.o hamming16.o

# Target executable name
TARGET = hamming16

all: $(TARGET) clean

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c hamming16.h
	$(CC) $(CFLAGS) -c main.c

hamming16.o: hamming16.c hamming16.h
	$(CC) $(CFLAGS) -c hamming16.c

clean:
	rm -f $(OBJS)