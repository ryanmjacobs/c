#!../c -std=c99 -Wall -Werror -DRUN --
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    puts("Generating 10 random numbers...");

    srand(time(NULL));

#ifdef RUN
    for (int i = 0; i < 10; i++)
        printf("%d\n", rand());
#else
    printf("error: compile with flag -DRUN\n");
#endif

    return 0;
}
