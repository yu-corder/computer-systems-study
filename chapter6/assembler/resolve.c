#include <stdlib.h>
#include <stdio.h>

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
    int addr = 1;
    printf("name = %s\n", name);
    return addr;
}