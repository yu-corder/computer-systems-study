#ifndef RESOLVE_H
#define RESOLVE_H

typedef struct {
    char *name;
} Symbol;

typedef struct {
    char *name;
    int ram_addr;
} Label;

void init_symbol_table (void);
int find_symbol (char *name);
int insert_symbol (char *name);

void init_label_table(void);
void insert_label (char *name, int len, int ram_addr);
int find_label (char *name);
#endif