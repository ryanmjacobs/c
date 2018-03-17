#include <stdio.h>

int main(int argc, char **argv) {
    if (argc == 2)
        fputs(argv[1], stdout);

    char buf[128];
    while (fgets(buf, 128, stdin)) {
        fputs(buf, stdout);
    }

    return 77;
}
