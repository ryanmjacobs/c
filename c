#!/bin/sh

c() {
    help_msg() {
        echo "Usage: $1 [file.c | compiler_options ...] [arguments]"
        echo "Execute C progams from the command line."
        echo
        echo "  Ex: c main.c"
        echo "  Ex: c main.c arg1 arg2"
        echo "  Ex: c 'main.c other.c' arg1 arg2"
        echo "  Ex: c 'main.c -lncurses' arg1 arg2"
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

    file="$(echo "$1" | cut -d' ' -f1)"
    fname="${file%\.c}"

    trap "rm -- $fname" SIGINT

    if cc -O2 -o "$fname" $1; then
        shift
        PATH=".:$PATH" "$fname" $@
    fi
    ret=$?

    trap - SIGINT
   #rm -- "$fname"
    return $ret
}

c $@
