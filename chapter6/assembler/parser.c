#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

int line = 0;
static void parse_A(char *p) {
    if (*p == 'R') {
        //@レジスタ
        p++;
        int addr = 0;
        if (isdigit(*p)) {
            addr = strtol(p, &p, 10);
            p++;
        } else {
            printf("Line %d: A numeric value follows the register.", line + 1);
            exit(1);
        }

        printf("addr == %d\n", addr);
        //機械語に変換関数呼び出し
    } else if (isdigit(*p)) {
        //@数値
        int addr = strtol(p, &p, 10);

        printf("addr == %d\n", addr);
        //機械語に変換関数呼び出し
    } else {
        //それ以外
        char str[32];
        int len = 0;

        while(isalnum(*p) && *p != '\n') {
            str[len++] = *p++;
        }
        str[len] = '\0';

        //機械語に変換関数呼び出し
    }
}

void parser (char *p) {
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
            parse_A(p);
            continue;
        }
        p++;
    }
    printf("line == %d\n", line);
}
