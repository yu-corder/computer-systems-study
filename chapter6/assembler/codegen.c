#include <stdio.h>
#include "codegen.h"
#include "resolve.h"

FILE *fp;
void init_generate_file(void) {
    fp = fopen("chapter6/asm/test.hack", "wb");
}

void fin_generate_file(void) {
    fclose(fp);
}

void generate_A_Number(int addr) {
    int bits[15] = {0};
    int total = addr;

    int i = 14;
    while (total != 0) {
        bits[i--] = total % 2;
        total /= 2;
    }


    fprintf(fp, "0");
    for (int i = 0; i < 15; i++) {
        fprintf(fp, "%d", bits[i]);
    }

    fprintf(fp, "\n");
    
}

void generate_A_symbol(char *name) {
    int addr = find_symbol(name);

    if (addr == -1) {
        addr = insert_symbol(name);
        generate_A_Number(addr);
    } else {
        generate_A_Number(addr);
    }
}

void dest_put(int *bits, int bit10, int bit11, int bit12) {
    bits[10] = bit10;
    bits[11] = bit11;
    bits[12] = bit12;
}

void dest(char *name, int *bits) {
    if (*name == 'D') {
        dest_put(bits, 0, 1, 0);
    } else if (*name == 'M') {
        dest_put(bits, 0, 0, 1);
    } else if (*name == 'A') {
        dest_put(bits, 1, 0, 0);
    } else if (*name == 'M' && name[1] == 'D') {
        dest_put(bits, 0, 1, 1);
    }

}

void no_jump(int *bits) {
    bits[13] = 0;
    bits[14] = 0;
    bits[15] = 0;
}

void comp_put
    (int *bits, int bit3, int bit4, int bit5, int bit6,
        int bit7, int bit8, int bit9)
{
    bits[3] = bit3;
    bits[4] = bit4;
    bits[5] = bit5;
    bits[6] = bit6;
    bits[7] = bit7;
    bits[8] = bit8;
    bits[9] = bit9;
}

void comp(char *name, int *bits) {
    if (name[2] == 'M') {
        comp_put(bits, 1, 1, 1, 0, 0, 0, 0);
    } else if (name[2] == 'D') {
        comp_put(bits, 0, 0, 0, 1, 1, 0, 0);
    } else if (name[2] == 'A') {
        comp_put(bits, 0, 1, 1, 0, 0, 0, 0);
    }
}

void generate_C (char *name) {
    int bits[16] = {0};
    for (int i = 0; i < 3; i++) {
        bits[i] = 1;
    }

    dest(name, bits);
    comp(name, bits);
    no_jump(bits);
    
    for (int i = 0; i < 16; i++) {
        fprintf(fp, "%d", bits[i]);
    }

    fprintf(fp, "\n");
}