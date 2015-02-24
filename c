#!/bin/bash

help_msg() {
    echo "Usage: $0 [options] [file.c ...] -- [args]"
    echo "Execute C progams from the command line."
    echo
    echo "  Ex: c main.c"
    echo "  Ex: c main.c -- arg1 arg2"
    echo "  Ex: c main.c other.c -- arg1 arg2"
    echo "  Ex: c main.c -lncurses -- arg1 arg2"
    echo
}

# help if we have no arguments
if [ $# -eq 0 ]; then
    help_msg $FUNCNAME
    exit 2
fi

# help if we get the flags
for arg in "$@"; do
    if [ "$arg" == "--help" ] || [ "$arg" == "-h" ]; then
        help_msg $FUNCNAME
        exit 2
    fi
done

files="$(echo $@ | awk -F'--' '{print $1}')"
args="$(echo $@  | awk -F'--' '{print $2}')"

# comment out the shebangs so the compilers don't complain
for f in $files; do
    if [ -f "$f" ]; then
        sed -i '1!b;s/^#!/\/\/#!/' "$f"
    fi
done

# fname will become argv[0]
fname="$(echo "$files" | cut -d' ' -f1)"
binname=$(mktemp /tmp/c.XXX)

cleanup() {
    # uncomment the shebangs
    for f in $files; do
        if [ -f "$f" ]; then
            sed -i '1!b;s/^\/\/#!/#!/' "$f"
        fi
    done

    # remove the tmp binary
    rm -- "$binname"
}
trap cleanup SIGINT

# compile and run
if cc -O2 -o "$binname" $files; then
    (exec -a "$fname" "$binname" $args)
    ret=$?
else
    ret=1
fi

trap - SIGINT
cleanup
exit $ret
