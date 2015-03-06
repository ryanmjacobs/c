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

# help if we have no arguments and no stdin
if [[ $# -eq 0 && -t 0 ]]; then
    help_msg 2
    exit 1
fi

# help if we get the flags
if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    help_msg 1
    exit 0
fi

# ensure our $CC variable is set
[[ -z "$CC" ]] && CC=cc
if ! type "$CC" &>/dev/null; then
    >&2 echo "error: \$CC ($CC) not found"
    exit 1
fi

# $comp  holds the files and options that will be passed to the compiler
# $fname will become the program's argv[0]
for arg in $1; do
    if [[ "$arg" == "--" ]]; then
        fname="$2"
        comp+=("$2")
        shift
    else
        comp+=("$arg")
    fi
done

# If we don't have an fname yet, pick one out of $comp
# that doesn't start with a '-'
if [[ -z "$fname" ]]; then
    for arg in $1; do
        if [[ "$arg" != -* ]]; then
            fname="$arg"
            break
        fi
    done
fi

# create a random biname
 tmpdir="$(mktemp -d -t c.XXX)"
binname="$tmpdir/bin"

# create stdin file if we need it
if [[ ! -t 0 ]]; then
    fname="stdin"
    stdin="$tmpdir/stdin.c"
    comp+=("$stdin")

    cat <&0 >$stdin # useless use of cat?
fi
0<&-

# copy source files to $tmpdir
i=0
for f in ${comp[@]}; do
    if [[ -f "$f" && "$f" != $tmpdir* ]]; then
        new_f="$tmpdir/$(echo "$f" | tr '/' '.')"
        cp "$f" "$new_f"
        comp[$i]="$new_f"
    fi
    let i++
done

# remove shebangs
for f in ${comp[@]}; do
    if [[ -f "$f" ]] && [[ "$(head -n1 "$f")" == \#\!* ]]; then
        echo "$(tail -n +2 "$f")" > "$f"
    fi
done

cleanup() {
    rm -r "$tmpdir" &>/dev/null
}
trap cleanup SIGINT

# compile and run
if "$CC" -I"$(pwd)" -O2 -o "$binname" ${comp[@]}; then
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
