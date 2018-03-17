#!/bin/bash

header1 "Test #6: Ensure ./examples are running correctly."

# Sadly, we can't test dinoshade.c or cube.c because they require OpenGL.
# And, we can't test noise.sh because it requires aplay.
files=("dns.c" "hello.c" "hello.cpp" "rand.c")
returns=(1 0 0 0 1)

for shebang in "" "$c"; do
    [ -z "$shebang" ] &&\
        header2 "using shebang" ||\
        header2 "calling directly"

    for i in `seq 0 $((${#files[@]} - 1))`; do
        cmd="$shebang ../examples/${files[$i]}"

        # set CFLAGS for non-shebang
        unset cf
        [ -n "$shebang" ] && cf="-DRUN -std=c99"

        # run cmd
        echo $cmd
        CFLAGS=$cf eval $cmd

        assert "return" "$? -eq ${returns[$i]}"
    done
    echo
done

echo
