#ifndef RESOLVE_H
#define RESOLVE_H

typedef struct {
    char *name;
} Symbol;

void init_symbol_table (void);
int find_symbol (char *name);
int insert_symbol (char *name);
#endif