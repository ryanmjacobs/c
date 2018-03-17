#!../../c
#include <stdio.h>
#include "a.h"
#include "c/c.h"

int main(int argc, char **argv) {
    putchar(a);
    putchar(b);
    putchar(c);
    putchar(argv[1][0]);
    return 99;
}
