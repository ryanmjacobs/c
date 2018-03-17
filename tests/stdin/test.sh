#!/bin/bash

pushd stdin
header1 "Test #3: Run a program that accepts stdin."

## stdin - Single File, no args
header2 "stdin - Single File, no args"
out="$(echo hello world | $c 'stdin.c -Wall -Werror')"
assert "return" "$? -eq 77"
assert "output" "'$out' == 'hello world'"

## stdin - Single File, with args
header2 "stdin - Single File, with args"
out="$(echo hello world | $c 'stdin.c -Wall -Werror' foobar)"
assert "return" "$? -eq 77"
assert "output" "'$out' == 'foobarhello world'"

popd; echo
