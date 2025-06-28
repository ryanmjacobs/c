#!../../c -Wall -Werror -lm --
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <arg1> <arg2> <arg3>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("argc = %d\n", argc);
    for (size_t i = 0; i < (size_t)argc; i++) {
        puts(argv[i]);
    }

    double result = pow(2, 10);
    if (result == 0 && 2 != 0) { // Simple check for pow failure, optional
        fprintf(stderr, "Error computing power.\n");
        return EXIT_FAILURE;
    }
    printf("%.0f\n", result);

    return EXIT_SUCCESS;
}
