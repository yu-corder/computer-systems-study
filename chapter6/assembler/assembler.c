#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char *read_file(const char *path) {
    FILE *fp = fopen(path, "r");

    if (!fp) {perror(path); exit(1);}

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buf = malloc(size + 1);
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);
    return buf;
}

int line = 0;
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

                //機械語に変換関数呼び出し
            } else if (isdigit(*p)) {
                //@数値
                int addr = strtol(p, &p, 10);

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
            continue;
        }
        p++;
    }
    printf("line == %d\n", line);
}

int main(int argc, char **argv) {
    int arg_count = argc - 1;
    char *src = read_file(argv[arg_count]);
    parser(src);
    return 0;
}