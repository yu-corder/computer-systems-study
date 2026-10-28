#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "resolve.h"

static Symbol *symbol_table;
static int symbol_table_capacity;
static int symbol_table_count = 0;

static int symbol_address = 16;


static Label *label_table;
static int label_table_capacity;
static int label_table_count = 0;

typedef struct {
    char name[8];
    int addr;
} EntryLabel;

static EntryLabel predefined_table[] = {
    {"SP", 0},
    {"LCL", 1},
    {"ARG", 2},
    {"THIS", 3},
    {"THAT", 4},
    {"R0", 0},
    {"R1", 1},
    {"R2", 2},
    {"R3", 3},
    {"R4", 4},
    {"R5", 5},
    {"R6", 6},
    {"R7", 7},
    {"R8", 8},
    {"R9", 9},
    {"R10", 10},
    {"R11", 11},
    {"R12", 12},
    {"R13", 13},
    {"R14", 14},
    {"R15", 15},
    {"SCREEN", 16384},
    {"KBD", 24576},
};

static void insert_predefined_symbols(void) {
    int size = sizeof(predefined_table) / sizeof(predefined_table[0]);
    for (int i = 0; i < size; i++) {
        int current_index = symbol_table_count++;
        size_t len = strlen(predefined_table[i].name);

        char *copy = malloc(len + 1);
        if (copy == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        memcpy(copy, predefined_table[i].name, len + 1);
        symbol_table[current_index].name = copy;
        symbol_table[current_index].addr = predefined_table[i].addr;
    }
}

void init_symbol_table (void) {
    symbol_table_capacity = 32;
    symbol_table = malloc(sizeof(Symbol) * 32);
    if (symbol_table == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    symbol_address = 16;
    insert_predefined_symbols();
}

static void make_bigger(void) {
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
            return symbol_table[i].addr;
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

    if (symbol_address >= 16384) {
        fprintf(stderr, "Error: Symbol address space exhausted\n");
        exit(EXIT_FAILURE);
    }

    symbol_table[current_index].name = copy;
    symbol_table[current_index].addr = symbol_address++;

    return symbol_table[current_index].addr;
}

void init_label_table(void) {
    label_table_capacity = 16;
    label_table = malloc(sizeof(Label) * 16);
    if (label_table == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
}

static void make_label_bigger(void) {
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


void free_table(void) {
    for (int i = 0; i < label_table_count; i++) {
        free(label_table[i].name);
    }

    for (int i = 0; i < symbol_table_count; i++) {
        free(symbol_table[i].name);
    }

    free(label_table);
    free(symbol_table);
    label_table = NULL;
    symbol_table = NULL;
}

