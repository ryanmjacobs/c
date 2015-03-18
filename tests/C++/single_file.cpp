#!../../c -Wall -Werror -lm --
#include <math.h>
#include <stdio.h>

int main(int argc, char **argv) {
    unsigned int i;

    printf("argc=%d\n", argc);

    if (argc != 4) {
        fputs("error: need 3 arguments\n", stderr);
        return 1;
    }

    for (i = 0; i < 4; i++)
        puts(argv[i]);

    printf("%.0f\n", pow(2, 10));

    return 123;
}
