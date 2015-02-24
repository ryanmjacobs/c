#!/bin/sh

c() {
    help_msg() {
        echo "Usage: $1 \"[file.c] [compiler options]...\" [program arguments]"
        echo "Execute C progams from the command line."
        echo
        echo "  Ex: c main.c"
        echo "  Ex: c main.c arg1 arg2"
        echo "  Ex: c \"main.c other.c\" arg1 arg2"
        echo "  Ex: c \"main.c -lncurses\" arg1 arg2"
        echo
    }

    if [ $# -eq 0 ]; then
        help_msg $FUNCNAME
        return 2
    fi

    for arg in "$@"; do
        if [ "$arg" == "--help" ] || [ "$arg" == "-h" ]; then
            help_msg $FUNCNAME
            return 2
        fi
    done

    fname="$(echo "$1" | cut -d' ' -f1)"
    binname=$(mktemp /tmp/c.XXX)

    trap "rm -- $binname" SIGINT

    if cc -O2 -o "$binname" $1; then
        shift
        (exec -a "$fname" "$binname" $@)
        ret=$?
    else
        ret=1
    fi

    trap - SIGINT
    rm -- "$binname"
    return $ret
}

c $@
