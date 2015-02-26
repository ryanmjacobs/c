#!/bin/bash

help_msg() {
    echo "Usage: $0 \"[file.c] [compiler options]...\" [program arguments]"
    echo "Execute C progams from the command line."
    echo
    echo "  Ex: c main.c"
    echo "  Ex: c main.c arg1 arg2"
    echo "  Ex: c \"main.c other.c\" arg1 arg2"
    echo "  Ex: c \"main.c -lncurses\" arg1 arg2"
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
        exit 0
    fi
done

# will pass to compiler
comp="$1"

# fname will become argv[0]
if [ "$(echo $1 | rev | cut -d' ' -f1 | rev)" == "-s" ]; then
    # we're using the shebang
    fname="$2"
    comp+=" $2"
else
    # running from CLI
    fname="$(echo $comp | cut -d' ' -f1)"
fi

# comment out the shebangs so the compilers don't complain
for f in $comp; do
    if [ -f "$f" ]; then
        sed -i '1!b;s/^#!/\/\/#!/' "$f"
    fi
done

clean=false
cleanup() {
    [ $clean == "true" ] && return

    # uncomment the shebangs
    for f in $comp; do
        if [ -f "$f" ]; then
            sed -i '1!b;s/^\/\/#!/#!/' "$f"
        fi
    done

    # remove the tmp binary
    rm -- "$binname"

    clean=true
}
trap cleanup SIGINT

# compile and run
binname=$(mktemp /tmp/c.XXX)
if cc -O2 -o "$binname" $comp; then
    shift
    (exec -a "$fname" "$binname" $@)
    ret=$?
else
    ret=1
fi

trap - SIGINT
cleanup
exit $ret
