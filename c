#!/usr/bin/env bash

help_msg() {
    >&$1 echo "Usage: $0 [file.c ... | compiler_options ...] [program arguments]"
    >&$1 echo "Execute C progams from the command line."
    >&$1 echo
    >&$1 echo "  Ex: c main.c"
    >&$1 echo "  Ex: c main.c arg1 arg2"
    >&$1 echo "  Ex: c \"main.c other.c\" arg1 arg2"
    >&$1 echo "  Ex: c \"main.c -lncurses\" arg1 arg2"
    >&$1 echo
}

sed_i() {
    sed="gsed"
    if ! type "$sed" &>/dev/null &>/dev/null; then
        sed="sed"
    fi
    "$sed" -i "$1" "$2"
}

# help if we have no arguments and no stdin
if [ $# -eq 0 ] && [ -t 0 ]; then
    help_msg 2
    exit 1
fi

# help if we get the flags
if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    help_msg 1
    exit 0
fi

# get our CC variable set
[ -z "$CC" ] && CC=cc
if ! type "$CC" &>/dev/null &>/dev/null; then
    >&2 echo "error: \$CC ($CC) not found"
    exit 1
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

# create a random biname
binname=$(mktemp -t c.XXX)

# create stdin file if we need it
if [ ! -t 0 ]; then
    fname="stdin"
    stdin="$binname.stdin.c"
    comp+=" $stdin"

    cat <&0 >$stdin # useless use of cat?
fi

# comment out the shebangs so the compilers don't complain
for f in $comp; do
    if [ -f "$f" ]; then
        sed_i '1!b;s/^#!/\/\/#!/' "$f"
    fi
done

clean=false
cleanup() {
    [ $clean == "true" ] && return

    # uncomment the shebangs
    for f in $comp; do
        if [ -f "$f" ]; then
            sed_i '1!b;s/^\/\/#!/#!/' "$f"
        fi
    done

    # remove the tmp files
    rm "$binname"
    [ -t 0 ] || rm "$stdin"

    clean=true
}
trap cleanup SIGINT

# compile and run
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
