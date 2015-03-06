#!/usr/bin/env bash

# Size of the binary cache store
cachesize=10

# Make sure this does not have a trailing slash...
tmproot=/tmp/cachec

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

# Removes all but the last $cachesize compiled files from the cache
cleanup() {
    if [[ $(ls -1 "$tmproot" | wc -l) -gt $cachesize ]]; then
        # List the objects in $tmproot, ordered by date, tail reads line $cachesize+1 onwards, then remove said directories
        ((cachesize++))
        ls -t1 "$tmproot" | tail -n +$cachesize | while read rem; do rm -rf "$tmproot/$rem"; done
    fi
}

# Runs our binary
run() {
    shift
    (exec -a "$fname" "$binname" "$@")
    return $?
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

# Where we store our cache
tmpdir="$tmproot"
mkdir -p $tmpdir

# create stdin file if we need it
if [[ ! -t 0 ]]; then
    fname="stdin"
    stdin="$tmpdir/stdin.c"
    comp+=("$stdin")

    cat <&0 >$stdin # useless use of cat?
fi
0<&-

# create a calculated biname
for f in ${comp[@]}; do
    # First, append sha1sums of all files and options into one long string
    if [[ -f "$f" ]]; then
        cachename="$cachename$(sha1sum $f|cut -d' ' -f1)"
    else
        cachename="$cachename$(sha1sum <<< $f|cut -d' ' -f1)"
    fi
done
# Now sha1sum this so that it fits into a filename
tmpdir="$tmpdir/$(sha1sum <<< $cachename|cut -d' ' -f1)"

binname="$tmpdir/bin"

# Run cached file if it exists
if [[ -f "$binname" ]]; then
    trap cleanup SIGINT
    run "$@"
    ret=$?
    # cleanup and exit
    trap - SIGINT
    cleanup
    exit $ret
else
    mkdir -p $tmpdir
fi

# assemble our includes, based on the original file locations
includes="-I$(pwd)"
for f in ${comp[@]}; do
    [[ -f "$f" ]] && includes+=" -I$(dirname "$f")"
done

# copy source files to $tmpdir
i=0
for f in ${comp[@]}; do
    if [[ -f "$f" && "$f" != $tmpdir* ]]; then
        mkdir -p "$tmpdir/$(dirname "$f")"
        cp "$f" "$tmpdir/$f"
        comp[$i]="$tmpdir/$f"
    fi
    let i++
done

for f in ${comp[@]}; do
    # for C++ files
    [[ "$f" =~ \.(cc|c\+\+|cpp|cxx)$ ]] && cpp=true

    # remove shebangs
    if [[ -f "$f" ]] && [[ "$(head -n1 "$f")" == \#\!* ]]; then
        echo "$(tail -n +2 "$f")" > "$f"
    fi
done

# link stdc++ if necessary
[[ "$cpp" == true ]] && comp+=("-lstdc++")

trap cleanup SIGINT

# compile and run
if "$CC" -O2 $CFLAGS -o "$binname" ${comp[@]} $includes; then
    run "$@"
    ret=$?
else
    ret=1
fi

# cleanup and exit
trap - SIGINT
cleanup
exit $ret
