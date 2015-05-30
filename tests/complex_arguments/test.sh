#!/bin/bash

pushd complex_arguments

##
# Test #2: Complex argument parsing.
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

header1 "Test #2: Complex argument parsing."

## Single File - CLI
header2 "Complex Args. - CLI"
run "$c 'complex_args.c -Wall -Werror -lm'\
         arg1 arg2 'arg three' arg\ four" "complex_args.c"

## Single File - Shebang
header2 "Complex Args. - Shebang"
run "./complex_args.c\
         arg1 arg2 'arg three' arg\ four" "./complex_args.c"
popd; echo
