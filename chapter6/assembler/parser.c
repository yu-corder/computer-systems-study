#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "parser.h"
#include "codegen.h"
#include "resolve.h"
bool one_path = true;
int line = 0;
int ram_addr = 0;
static char *read_name(char *p, int *len) {
    char *start = p;

    while (isalnum((unsigned char) *p)) {
        p++;
    }

    size_t length = (size_t)(p - start);
    char *str = malloc(length + 1);

    if (str == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    memcpy(str, start, length);
    str[length] = '\0';
    *len = (int) length;

    return str;
}

static char *parse_A(char *p) {
    if (*p == 'R') {
        //@レジスタ
        p++;
        int addr = 0;
        if (isdigit(*p)) {
            addr = strtol(p, &p, 10);
            if (addr < 0 || addr > 15) {
                printf("Line %d: Expected a numeric value after register.", line + 1);
                exit(1);
            }

            if (!one_path) {
                generate_A_Number(addr);
            }
        } else {
            int len = 0;
            char *str = read_name(p, &len);
            p += len;

            if (!one_path) {
                generate_A_symbol(str);
            }

            free(str);
        }
    } else if (isdigit(*p)) {
        //@数値
        int addr = strtol(p, &p, 10);

        if (addr < 0 || addr > 32767) {
            printf("Line %d: Expected a numeric value after register.", line + 1);
            exit(1);
        }

        if (!one_path) {
            generate_A_Number(addr);
        }
    } else {
        //それ以外
        int len = 0;
        char *str = read_name(p, &len);
        p += len;

        if (!one_path) {
            generate_A_symbol(str);
        }

        free(str);
    }
    ram_addr++;
    return p;
}

static char *parse_label(char *p) {
    char str[32];
    int len = 0;
    while(isalnum(*p) && *p != ')') {
        str[len++] = *p++;
    }

    str[len] = '\0';

    if (*p == ')') {
        p++;
    }

    if (!one_path) return p;

    insert_label(str, ram_addr);
    return p;
}

void parser (char *p) {
    line = 0;
    while (*p) {
        if (*p == '\n') {
            p++;
            line++;
            continue;
        }

        if (isspace(*p)) { p++; continue;}

        if (*p == '/' && (p[1] == '/')) {
            while (*p != '\n') {
                p++;
            }
            continue;
        }

        if (*p == '@') {
            p++;
            p = parse_A(p);
            continue;
        }

        if ((*p == 'D' || *p== 'M' || *p == 'A'  || *p == '0' || *p == '1') && (p[1] == '=' || p[1] == ';')) {
            p += generate_C(p);
            continue;
        }

        if (*p == '(') {
            p++;
            p = parse_label(p);
            continue;
        }

        p++;
    }
    printf("line == %d\n", line);
}
