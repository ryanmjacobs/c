#!/bin/bash

pushd header
header1 "Test #7: Run a program uses header files."

# direct call
out="$($c 'main.c -Wall -Werror' d)"
assert "return" "$? -eq 99"
assert "output" "'$out' == 'abcd'"

# shebang
out="$(./main.c d)"
assert "return" "$? -eq 99"
assert "output" "'$out' == 'abcd'"

popd; echo
