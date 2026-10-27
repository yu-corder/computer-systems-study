#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

    if (addr == -1) {
        addr = find_symbol(name);
    }
    
    if (addr == -1) {
        addr = insert_symbol(name);   
    }

    generate_A_Number(addr);
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


static void dest_put(int *bits, const char *binary) {
    for (int i = 0; i < 3; i++) {
        bits[i + 10] = binary[i] - '0';
    }
}

static int dest(char *p, int *bits) {
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

typedef struct {
    const char *name;
    const char *binary;
} JumpEntry;

static const JumpEntry jump_table[] = {
    {"JGT", "001"},
    {"JEQ", "010"},
    {"JGE", "011"},
    {"JLT", "100"},
    {"JNE", "101"},
    {"JLE", "110"},
    {"JMP", "111"},
    {"NOJ", "000"},
};

static void jump_put(int *bits, const char *binary) {
    for (int i = 0; i < 3; i++) {
        bits[i + 13] = binary[i] - '0';
    }
}

static int jump(char *p, int *bits) {
    for (size_t i = 0; i < sizeof(jump_table) / sizeof(jump_table[0]); i++) {
        const char *name = jump_table[i].name;
        int len = strlen(name);

        if (strncmp(p, name, len) == 0 &&
            (p[len] == '\n' || p[len] == '\0')) {
                jump_put(bits, jump_table[i].binary);
                return len;
        }
    }

    return 1;
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

static void comp_put (int *bits, const char *binary)
{
    for (int i = 0; i < 7; i++) {
        bits[i + 3] = binary[i] - '0';
    }
}

static int comp(char *p, int *bits) {
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

    char *line_end = strchr(p, '\n');
    if (line_end == NULL) {
        line_end = p + strlen(p);
    }

    size_t line_len = line_end - p;

    char *equal = memchr(p, '=', line_len);
    char *semicolon = memchr(p, ';', line_len);

    if (equal != NULL) {
        tmp_len = dest(p, bits);
        p += tmp_len + 1;
        len += tmp_len + 1;
    }

    tmp_len = comp(p, bits);
    len += tmp_len;

    if (semicolon != NULL) {
        tmp_len = jump(semicolon + 1, bits);
        len += tmp_len + 1;
    }

    if (!one_path) {
        for (int i = 0; i < 16; i++) {
            fprintf(fp, "%d", bits[i]);
        }

        fprintf(fp, "\n");
    }

    ram_addr++;
    return len;
}