CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET_COMP = chapter6/bin/assembler

all: ${TARGET_COMP}

$(TARGET_COMP): chapter6/assembler/assembler.c chapter6/assembler/parser.c chapter6/assembler/codegen.c
		$(CC) $(CFLAGS) -o $(TARGET_COMP) chapter6/assembler/assembler.c chapter6/assembler/parser.c chapter6/assembler/codegen.c

run: all
		./$(TARGET_COMP) chapter6/asm/Mult.asm