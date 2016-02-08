#!../c -Wall -std=c89 -pedantic --
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* http://linux.die.net/man/3/hstrerror */
void _hstrerror(int err) {
    switch (err) {
        case HOST_NOT_FOUND:
            fputs("error: host is unknown", stderr);
            break;

        case NO_DATA:
            fputs("error: name is valid but has no IP address", stderr);
            break;

        case NO_RECOVERY:
            fputs("error: nonrecoverable name server error occurred", stderr);
            break;

        case TRY_AGAIN:
            fputs("error: temporary error occurred, try again later", stderr);
            break;
    }
}

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
        _hstrerror(h_errno);
    }

    return 0;
}
