# Makefile for listtasks.c on AmigaOS 3.1

# Compiler
CC = gcc

# Source file
SRC = listtasks.c

# Output executable
TARGET = listtasks

# Compiler flags
CFLAGS = -Wall -O2

# Linker flags
LDFLAGS = -lamiga

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean build files
.PHONY: clean
clean:
	rm -f $(TARGET)
