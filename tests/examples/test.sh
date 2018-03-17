#!/bin/bash

header1 "Test #6: Ensure ./examples are running correctly."

# Sadly, we can't test dinoshade.c or cube.c because they require OpenGL.
# We can't test noise.sh because it requires aplay.
files=("dns.c" "hello.c" "hello.cpp" "rand.c")
returns=(1 0 0 0 1)

for i in `seq 0 $((${#files[@]} - 1))`; do
    "../examples/${files[$i]}"; ret=$?
    assert "return" "$ret -eq ${returns[$i]}"
done

echo

