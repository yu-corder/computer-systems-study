#include <stdio.h>
#include "codegen.h"
#include "resolve.h"

FILE *dest;
void init_generate_file(void) {
    dest = fopen("chapter6/asm/test.hack", "wb");
}

void fin_generate_file(void) {
    fclose(dest);
}

void generate_A_Number(int addr) {
    int bits[15] = {0};
    int total = addr;

    int i = 14;
    while (total != 0) {
        bits[i--] = total % 2;
        total /= 2;
    }


    fprintf(dest, "0");
    for (int i = 0; i < 15; i++) {
        fprintf(dest, "%d", bits[i]);
    }

    fprintf(dest, "\n");
    
}

void generate_A_symbol(char *name) {
    printf("str == %s\n", name);
}