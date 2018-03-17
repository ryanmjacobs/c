#!../c -std=c99 --
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    puts("Generating 10 random numbers...");

    srand(time(NULL));

    for (int i = 0; i < 10; i++)
        printf("%d\n", rand());

    return 0;
}

