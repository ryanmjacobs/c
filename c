#!/bin/bash

help_msg() {
    echo "Usage: $0 [file.c ... | compiler_options ...] [program arguments]"
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
if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    help_msg $FUNCNAME
    exit 0
fi

# get our CC variable set
[ -z "$CC" ] && CC=cc
if ! type "$CC" &>/dev/null &>/dev/null; then
    echo "error: \$CC ($CC) not found"
    exit 3
fi

# $comp  holds the files and options that will be passed to the compiler
# $fname will become the program's argv[0]
for arg in $1; do
    if [ "$arg" == "--" ]; then
        fname="$2"
        comp+=" $2"
        shift
    else
        comp+=" $arg"
    fi
done

# If we don't have an fname yet, pick one out of $comp
# that doesn't start with a '-'
if [ -z "$fname" ]; then
    for arg in $1; do
        if [[ "$arg" != -* ]]; then
            fname="$arg"
            break
        fi
    done
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
    (exec -a "$fname" "$binname" "$@")
    ret=$?
else
    ret=1
fi

# cleanup and exit
trap - SIGINT
cleanup
exit $ret
