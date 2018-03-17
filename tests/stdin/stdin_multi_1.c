#include <stdio.h>

void print_pow(void);

int main(int argc, char **argv) {
    unsigned int i;

    printf("argc=%d\n", argc);

    if (argc != 5) {
        fputs("error: need 4 arguments\n", stderr);
        return 1;
    }

    for (i = 0; i < 5; i++)
        puts(argv[i]);

    print_pow();

    return 123;
}
