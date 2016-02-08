#!/usr/bin/c -Wall -std=c89 -pedantic --
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    struct hostent *h;
    struct in_addr**ia;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <host>\n", argv[0]);
        return 1;
    }

    h = gethostbyname(argv[1]);

    if (h) {
        ia = (struct in_addr**)h->h_addr_list;
        while (*ia) printf("%s\n", inet_ntoa(**(ia++)));
    } else {
        printf("%s\n", hstrerror(h_errno));
    }

    return 0;
}
