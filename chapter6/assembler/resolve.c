#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "resolve.h"

Symbol *symbol_table;
int symbol_table_capacity;
int symbol_table_count = 0;


Label *label_table;
int label_table_capacity;
int label_table_count = 0;

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
    if (symbol_table_count >= symbol_table_capacity) {
        make_bigger();
    }

    size_t len = strlen(name);

    char *copy = malloc(len + 1);
    if (copy == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    memcpy(copy, name, len + 1);

    int current_index = symbol_table_count++;
    symbol_table[current_index].name = copy;

    return current_index;
}

void init_label_table(void) {
    label_table_capacity = 16;
    label_table = malloc(sizeof(Label) * 16);
}

void make_label_bigger(void) {
    int new_capacity = label_table_capacity * 2;
    Label *new_label_table = realloc(label_table, sizeof(Label) * new_capacity);

    if (new_label_table == NULL) {
        printf("Memory expansion failed.");
        exit(1);
    }

    label_table_capacity = new_capacity;
    label_table = new_label_table;
}

void insert_label (char *name, int len, int ram_addr) {
    if (label_table_count >= label_table_capacity) {
        make_label_bigger();
    }
    int current_index = label_table_count++;

    label_table[current_index].name = malloc(len + 1);
    strcpy(label_table[current_index].name, name);
    label_table[current_index].ram_addr = ram_addr;
}

int find_label (char *name) {
    for (int i = 0; i < label_table_count; i++) {
        if (strcmp(label_table[i].name, name) == 0) {
            return label_table[i].ram_addr;
        }
    }
    return -1;
}

