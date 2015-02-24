#!/bin/bash

help_msg() {
    echo "Usage: $0 [options] [file.c ...] -- main.c [args]"
    echo "Execute C progams from the command line."
    echo
    echo "  Ex: c main.c"
    echo "  Ex: c -- main.c arg1 arg2"
    echo "  Ex: c other.c -- main.c arg1 arg2"
    echo "  Ex: c -lncurses -- main.c arg1 arg2"
    echo
}

# help if we have no arguments
if [ $# -eq 0 ]; then
    help_msg $FUNCNAME
    exit 2
fi

# help if we got the flags
for arg in "$@"; do
    if [ "$arg" == "--help" ] || [ "$arg" == "-h" ]; then
        help_msg $FUNCNAME
        exit 2
    fi
done

if [ $# -eq 1 ]; then
    comp="$@"
    args=""
    fname="$@"
else
    comp="$(echo $@ | awk -F'--' '{print $1}')"
    args="$(echo $@ | awk -F'--' '{print $2}')"

    # fname will become argv[0]
    fname="$(echo $args | cut -d' ' -f1)"
     args="$(echo $args | cut -d' ' -f2-)"
     comp="$(echo $comp $fname)"

    # in case we have no arguments
    [ "$args" == "$fname" ] && args=""
fi

# comment out the shebangs so the compilers don't complain
for f in $comp; do
    if [ -f "$f" ]; then
        sed -i '1!b;s/^#!/\/\/#!/' "$f"
    fi
done

cleanup() {
    # uncomment the shebangs
    for f in $comp; do
        if [ -f "$f" ]; then
            sed -i '1!b;s/^\/\/#!/#!/' "$f"
        fi
    done

    # remove the tmp binary
    rm -- "$binname"
}
trap cleanup SIGINT

# compile and run
binname=$(mktemp /tmp/c.XXX)
if cc -O2 -o "$binname" $comp; then
    (exec -a "$fname" "$binname" $args)
    ret=$?
else
    ret=1
fi

trap - SIGINT
cleanup
exit $ret
