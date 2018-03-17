#!../../c multi_file_2.c -Wall -Werror -lm --
#include <stdio.h>

void print_pow(void);

int main(int argc, char **argv) {
    unsigned int i;

    printf("argc=%d\n", argc);

    if (argc != 4) {
        fputs("error: need 3 arguments\n", stderr);
        return 1;
    }

    for (i = 0; i < 4; i++)
        puts(argv[i]);

    print_pow();

    return 123;
}
