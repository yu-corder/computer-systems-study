#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "resolve.h"

Symbol *symbol_table;
int symbol_table_capacity;
int symbol_table_count = 0;

static char *predefined_table[] = {
    "SP",
    "LCL",
    "ARG",
    "THIS",
    "THAT",
    "R5",
    "R6",
    "R7",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15"
};

void insert_predefined_symbols(void) {
    int size = sizeof(predefined_table) / sizeof(predefined_table[0]);
    for (int i = 0; i < size; i++) {
        insert_symbol(predefined_table[i]);
    }
}

void init_symbol_table (void) {
    symbol_table_capacity = 16;
    symbol_table = malloc(sizeof(Symbol) * 16);
    insert_predefined_symbols();
}

void make_bigger(void) {
    int new_capacity = symbol_table_capacity * 2;
    Symbol *new_symbol_table = realloc(symbol_table, sizeof(Symbol) * new_capacity);

    if (new_symbol_table == NULL) {
        printf("Memory expansion failed.");
        exit(1);
    }

    symbol_table_capacity = new_capacity;
    symbol_table = new_symbol_table;
}

int find_symbol (char *name) {
    for (int i = 0; i < symbol_table_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int insert_symbol (char *name) {
    int current_index = symbol_table_count++;

    int len = 0;
    while (*name != '\0') {
        name++;
        len++;
    }
    name -= len;

    symbol_table[current_index].name = malloc(len + 1);
    len = 0;
    while (*name != '\0') {
        symbol_table[current_index].name[len++] = *name++;
    }
    symbol_table[current_index].name[len] = '\0';

    return current_index;
}