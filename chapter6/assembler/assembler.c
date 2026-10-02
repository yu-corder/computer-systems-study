#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"
#include "codegen.h"

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

int main(int argc, char **argv) {
    int arg_count = argc - 1;

    char *src = read_file(argv[arg_count]);
    
    init_generate_file();
    parser(src);
    fin_generate_file();
    return 0;
}