CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET_COMP = chapter6/bin/assembler

all: ${TARGET_COMP}

$(TARGET_COMP): chapter6/assembler/assembler.c
		$(CC) $(CFLAGS) -o $(TARGET_COMP) chapter6/assembler/assembler.c

run: all
		./$(TARGET_COMP) chapter6/asm/Mult.asm