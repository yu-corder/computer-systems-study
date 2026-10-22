#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "codegen.h"
#include "resolve.h"
#include "parser.h"

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
    int addr = find_label(name);
    if (addr != -1) {
        generate_A_Number(addr);
        return;
    }

    addr = find_symbol(name);

    if (addr == -1) {
        addr = insert_symbol(name);
        generate_A_Number(addr);
    } else {
        generate_A_Number(addr);
    }
}

typedef struct {
    const char *name;
    const char *binary;
} DestEntry;

static const DestEntry dest_table[] = {
    {"AMD=", "111"},

    {"MD=", "011"},
    {"AD=", "110"},
    {"AM=", "101"},

    {"D=", "010"},
    {"M=", "001"},
    {"A=", "100"},
};


void dest_put(int *bits, const char *binary) {
    for (int i = 0; i < 3; i++) {
        bits[i + 10] = binary[i] - '0';
    }
}

int dest(char *p, int *bits) {
    for (size_t i = 0; i < sizeof(dest_table) / sizeof(dest_table[0]); i++) {
        const char *name = dest_table[i].name;
        int len = strlen(name);

        if (strncmp(p, name, len) == 0) {
                dest_put(bits, dest_table[i].binary);
                return len - 1;
        }
    }

    return 0;
}

void jump_put(int *bits, int bit13, int bit14, int bit15) {
    bits[13] = bit13;
    bits[14] = bit14;
    bits[15] = bit15;
}

int no_jump(int *bits) {
    jump_put(bits, 0, 0, 0);
    return 1;
}

int jump(char *p, int *bits) {
    int len = 3;
    if (*p == 'J' && p[1] == 'G' && p[2] == 'T') {
        jump_put(bits, 0, 0, 1);
    } else if (*p == 'J' && p[1] == 'E' && p[2] == 'Q') {
        jump_put(bits, 0, 1, 0);
    } else if (*p == 'J' && p[1] == 'G' && p[2] == 'E') {
        jump_put(bits, 0, 1, 1);
    } else if (*p == 'J' && p[1] == 'L' && p[2] == 'T') {
        jump_put(bits, 1, 0, 0);
    } else if (*p == 'J' && p[1] == 'N' && p[2] == 'E') {
        jump_put(bits, 1, 0, 1);
    } else if (*p == 'J' && p[1] == 'L' && p[2] == 'E') {
        jump_put(bits, 1, 1, 0);
    } else if (*p == 'J' && p[1] == 'M' && p[2] == 'P') {
        jump_put(bits, 1, 1, 1);
    } else {
        jump_put(bits, 0, 0, 0);
        len = 1;
    }

    return len;
}


typedef struct {
    const char *name;
    const char *binary;
} CompEntry;

static const CompEntry comp_table[] = {
    {"0", "0101010"},
    {"1", "0111111"},
    {"D", "0001100"},
    {"A", "0110000"},
    {"M", "1110000"},

    {"-1", "0111010"},
    {"-D", "0001111"},
    {"-A", "0110011"},
    {"-M", "1110011"},

    {"!M", "1110001"},
    {"!D", "0001101"},
    {"!A", "0110001"},

    {"M+1", "1110111"},
    {"M-1", "1110010"},
    {"D+1", "0011111"},
    {"D-1", "0001110"},
    {"A+1", "0110111"},
    {"A-1", "0110010"},

    {"D+A", "0000010"},
    {"D+M", "1000010"},
    {"D-M", "1010011"},
    {"M-D", "1000111"},
    {"A-D", "0000111"},
    {"D-A", "0010011"},

    {"D&M", "1000000"},
    {"D|M", "1010101"},
    {"D&A", "0000000"},
    {"D|A", "0010101"},
};

void comp_put (int *bits, const char *binary)
{
    for (int i = 0; i < 7; i++) {
        bits[i + 3] = binary[i] - '0';
    }
}

int comp(char *p, int *bits) {
    for (size_t i = 0; i < sizeof(comp_table) / sizeof(comp_table[0]); i++) {
        const char *name = comp_table[i].name;
        int len = strlen(name);

        if (strncmp(p, name, len) == 0 &&
            (p[len] == ';' || p[len] == '\n' || p[len] == '\0')) {
                comp_put(bits, comp_table[i].binary);
                return len;
        }
    }

    return 0;
}

int generate_C (char *p) {
    int bits[16] = {0};
    for (int i = 0; i < 3; i++) {
        bits[i] = 1;
    }

    int len = 0;
    int tmp_len = 0;

    tmp_len = dest(p, bits);
    len += tmp_len;
    p += tmp_len;
    
    char *tmp_p = p;
    int jump_len = 0;
    if (*p == '=') {
        tmp_len = no_jump(bits);
    } else if (p[1] == ';' || p[2] == ';') {
        while (*tmp_p != ';') {
            tmp_p += 1;
        }
        tmp_p += 1;
        jump_len = jump(tmp_p, bits);
        tmp_len = 0;
    }

    len += tmp_len;
    p += tmp_len;

    tmp_len = comp(p, bits);
    len += tmp_len;
    len += jump_len;
    if (!one_path) {
        for (int i = 0; i < 16; i++) {
            fprintf(fp, "%d", bits[i]);
        }

        fprintf(fp, "\n");
    }

    ram_addr++;
    return len;
}