#!/bin/bash

pushd stdin

##
# Test #5: Compile files from stdin.
#
# $1="command to run C file"
# $2="expected value of argv[0]"
##
run() {
    out=$(eval "$1")
    ret=$?
    n=1
    while read -r line; do
        case $n in
            1) assert "argc   " "'$line' == 'argc=5'";;
            2) assert "argv[0]" "'$line' == '$2'";;
            3) assert "argv[1]" "'$line' == 'arg1'";;
            4) assert "argv[2]" "'$line' == 'arg2'";;
            5) assert "argv[3]" "'$line' == 'arg three'";;
            6) assert "argv[4]" "'$line' == 'arg four'";;
            7) assert "pow()  " "'$line' -eq 1024";;
        esac
        let n++
    done <<< "$out"
    assert "return " "$ret -eq 123"
}

header1 "Test #3: Compile files from stdin."

## stdin - Single File
header2 "stdin - Single File"
run "$c '-Wall -Werror -lm'\
         arg1 arg2 'arg three' arg\ four < stdin.c" "stdin"

## stdin - Multi. File
header2 "stdin - Multi. File"
run "$c '-Wall -Werror -lm stdin_multi_2.c'\
         arg1 arg2 'arg three' arg\ four < stdin_multi_1.c" "stdin"
popd; echo

