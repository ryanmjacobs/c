#!/usr/bin/env bash

# max cachesize in kilobytes (default=5MB)
[[ -z "$C_CACHE_SIZE" ]] && C_CACHE_SIZE=$((5*1024))
if ! [[ "$C_CACHE_SIZE" =~ ^[0-9]*$ ]]; then
    C_CACHE_SIZE=$((5*1024))
    >&2 echo 'warning: $C_CACHE_SIZE should be a positive integer'
fi

help_msg() {
    >&$1 echo "Usage: $(basename "$0") <file.c ... | compiler_options ...> [program_arguments]"
    >&$1 echo 'Execute C programs from the command line.'
    >&$1 echo
    >&$1 echo '  Ex: c main.c'
    >&$1 echo '  Ex: c main.c arg1 arg2'
    >&$1 echo "  Ex: c 'main.c other.c' arg1 arg2"
    >&$1 echo "  Ex: c 'main.c -lncurses' arg1 arg2"
    >&$1 echo
}

cleanup() {
    # remove temporary source directory
    rm -rf "$tmpdir"

    # current cachesize
    size="$(du --block-size=1024 -s "$tmproot" | tail -n1 | cut -f1)"

    # remove cache files until we are under the maximum cache size (C_CACHE_SIZE)
    while [[ "$size" -gt "$C_CACHE_SIZE" ]]; do
        [[ -z "$(ls -A "$tmproot")" ]] && break
        rm -rf "$(find "$tmproot" | tail -n1)"
    done
}

# Handle --help, -h, and zero args
[[ "$1" == "--help" || "$1" == "-h" ]] && { help_msg 1; exit 0; }
[[ "$#" -lt 1 ]] && { help_msg 2; exit 1; }

# ensure our $CC and $CXX variables are set
[[ -z "$CC" ]]  && CC=cc
[[ -z "$CXX" ]] && CXX=c++
if ! hash "$CC" &>/dev/null; then
    >&2 echo "error: \$CC ($CC) not found"
    exit 1
fi

# $comp holds the files and options that will be passed to the compiler
# $fname will become the program's argv[0]
for arg in $1; do
    if [[ "$arg" == "--" ]]; then
        fname="$2"
        comp=("$2" "${comp[@]}")
        shift
    else
        comp+=("$arg")
    fi
done

# if we don't have an fname yet, pick one out of $comp
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

# decide on a hash function by using the first one we find
potential_hashes=(md5sum sha256sum sha1sum shasum)
hash_func=:
for hf in "${potential_hashes[@]}"; do
    hash "$hf" &>/dev/null && { hash_func="$hf"; break; }
done

# disable caching if we don't locate a hashing function
[ "$hash_func" == : ] && C_CACHE_SIZE=0

# determine if we are C or C++, then use appropriate flags
is_cpp=false
for f in "$fname" "${comp[@]}"; do
    # only examine files
    [[ ! -f "$f" ]] && continue

    # if one file has a C++ extension, then the whole set is C++
    if [[ "$f" =~ \.(cc|c\+\+|cpp|cxx)$ ]]; then
        is_cpp=true

        if hash "$CXX" &>/dev/null; then
            # found $CXX, we will use that
            CC="$CXX"
            comp+=($CXXFLAGS)
        else
            # couldn't find $CXX, so we make do with $CC and -lstdc++
            comp+=($CFLAGS "-lstdc++")
        fi

        break
    fi
done

# add $CFLAGS if and only if we are not C++
if [[ "$is_cpp" == false ]]; then
    comp+=($CFLAGS)
fi

# add preprocessor flags
comp+=($CPPFLAGS)

# hash all of our data
prehash="$CC ${comp[@]}" # compiler + flags and files
for f in "${comp[@]}"; do
    [ -f "$f" ] && prehash+="$f $(cpp "$f" 2>&1)"
done

# hash everything into one unique identifier, for caching purposes
id="c$("$hash_func" <<< "$prehash" | cut -d' ' -f1)"
tmpdir="$tmproot/$id.src"
binname="$tmproot/$id.bin"

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
includes="-I'$PWD'"
for f in ${comp[@]}; do
    [[ -f "$f" ]] && includes+=" -I'$(dirname "$f")'"
done

# copy source files to $tmpdir
i=0
for f in ${comp[@]}; do
    if [[ -f "$f" && "$f" != $tmpdir* ]]; then
        mkdir -p "$tmpdir/$(dirname "$f")"
        cp "$f" "$tmpdir/$f"

        # assume language is C, if no extension is given
        base="$(basename "$f")"
        ext="${base##*.}"
        if [[ "$ext" == "$base" ]]; then
            comp[$i]="-x c $tmpdir/$f -x none"
        else
            comp[$i]="$tmpdir/$f"
        fi
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
if eval "$CC" -O2 -o "$binname" ${comp[@]} $includes; then
    run "$@"
else
    cleanup
    exit 1
fi
