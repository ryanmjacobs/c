#!/bin/bash

header1 "Test #5: Correct script warnings/errors/returns."

## Shell - $C_CACHE_SIZE
header2 "Shell - \$C_CACHE_SIZE warnings"

# cache=-10
line=$(eval "C_CACHE_SIZE=-10 $c" 2>&1); ret=$?
fline="$(echo "$line" | head -n1)"
assert "\$C_CACHE_SIZE=-10"\
     "'$fline' =~ warning:\\ \\\$C_CACHE_SIZE\\ should\\ be\\ a\\ positive\\ integer.*"
assert "return" "$ret -eq 1"

# cache=0
line=$(eval "C_CACHE_SIZE=0 $c" 2>&1); ret=$?
fline="$(echo "$line" | head -n1)"
assert "\$C_CACHE_SIZE=0" "'$fline' =~ Usage:.*"
assert "return" "$ret -eq 1"

# cache=10
line=$(eval "C_CACHE_SIZE=10 $c" 2>&1); ret=$?
fline="$(echo "$line" | head -n1)"
assert "\$C_CACHE_SIZE=10" "'$fline' =~ Usage:.*"
assert "return" "$ret -eq 1"

## Shell - help flags
header2 "Shell - help flags"

# gives usage without any args
line=$(eval "$c" 2>&1); ret=$?
fline="$(echo "$line" | head -n1)"
assert "c" "'$fline' =~ Usage:.*"
assert "return" "$ret -eq 1"

# gives usage with --help
line=$(eval "$c --help" 2>&1); ret=$?
fline="$(echo "$line" | head -n1)"
assert "c --help" "'$fline' =~ Usage:.*"
assert "return" "$ret -eq 0"

# gives usage with -h
line=$(eval "$c -h" 2>&1); ret=$?
fline="$(echo "$line" | head -n1)"
assert "c -h" "'$fline' =~ Usage:.*"
assert "return" "$ret -eq 0"

echo
