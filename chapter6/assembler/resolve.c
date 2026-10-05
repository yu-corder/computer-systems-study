#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "resolve.h"

Symbol *symbol_table;
int symbol_table_capacity;
int symbol_table_count = 0;

void init_symbol_table (void) {
    symbol_table_capacity = 8;
    symbol_table = malloc(sizeof(Symbol) * 8);
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