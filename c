#!/usr/bin/env bash

# max cachesize in kilobytes
[[ -z "$C_CACHE_SIZE" ]] && C_CACHE_SIZE=$((5*1024))

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

cleanup() {
    # remove $tmpdir, which holds source code
    rm -rf "$tmpdir" "$tmproot/stdin.c"

    # remove cache files until we are under $cachesize
    while [[ "$(du -kc "$tmproot" | tail -1 | cut -f1)" -gt "$C_CACHE_SIZE" ]]; do
        [[ -z "$(ls -A "$tmproot")" ]] && break
        rm -rf "$(find "$tmproot" -type f | tail -n1)"
    done
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

# get cache location
if [[ -n "$C_CACHE_PATH" ]]; then
    tmproot="$C_CACHE_PATH"
else
    [[ -z "$TMPDIR" ]] && TMPDIR="/tmp"
    tmproot="$TMPDIR/c.cache"
fi
mkdir -p "$tmproot"

# create stdin file if we need it
if [[ ! -t 0 ]]; then
    fname="stdin"
    stdin="$tmproot/stdin.c"
    comp+=("$stdin")

    cat <&0 >$stdin # useless use of cat?
fi
0<&-

shasum="sha1sum"
if ! type "$shasum" &>/dev/null; then
    shasum="shasum"
fi

# determine if we are C or C++, then use appropriate flags
for f in ${comp[@]}; do
    if [[ -f "$f" && "$f" =~ \.(cc|c\+\+|cpp|cxx)$ ]]; then
        comp+=($CXXFLAGS "-lstdc++")
        type "$CXX" &>/dev/null && CC="$CXX"
        break
    else
        comp+=($CFLAGS)
        break
    fi
done
comp+=($CPPFLAGS)

# create calculated biname
cachename="$("$shasum" <<< "$CC")"
for f in ${comp[@]}; do
    # first, append sha1sums of all files and options into one long string
    if [[ -f "$f" ]]; then
        cachename+="$("$shasum" "$f" | cut -d' ' -f1)"
    else
        cachename+="$("$shasum" <<< "$f" | cut -d' ' -f1)"
    fi
done

# now sha1sum this so that it fits into a filename
sha="$("$shasum" <<< $cachename | cut -d' ' -f1)"
tmpdir="$tmproot/dir.$sha"
binname="$tmproot/$sha"

# run binary
run() {
    trap cleanup SIGINT

    shift
    (exec -a "$fname" "$binname" "$@")
    ret=$?

    trap - SIGINT
    cleanup
    exit $ret
}

# run cached file if it exists
if [[ -f "$binname" ]]; then
    run "$@"
else
    mkdir -p "$tmpdir"
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

# remove shebangs
for f in ${comp[@]}; do
    if [[ -f "$f" ]] && [[ "$(head -n1 "$f")" == \#\!* ]]; then
        echo "$(tail -n +2 "$f")" > "$f"
    fi
done

# compile and run
if "$CC" -O2 -o "$binname" ${comp[@]} $includes; then
    run "$@"
else
    exit 1
fi
