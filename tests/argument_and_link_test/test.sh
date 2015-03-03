#!/bin/bash

pushd argument_and_link_test

##
# Test #1: Argument parsing and linking capabilities.
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
            1) assert "argc   " "'$line' == 'argc=4'";;
            2) assert "argv[0]" "'$line' == '$2'";;
            3) assert "argv[1]" "'$line' == 'arg1'";;
            4) assert "argv[2]" "'$line' == 'arg2'";;
            5) assert "argv[3]" "'$line' == 'arg three'";;
            6) assert "pow()  " "'$line' -eq 1024";;
        esac
        let n++
    done <<< "$out"
    assert "return " "$ret -eq 123"
}

header1 "Test #1: Argument parsing and linking capabilities."

## Single File - CLI
header2 "Single File - CLI"
run "$c 'single_file.c -Wall -Werror -lm'\
         arg1 arg2 'arg three'" "single_file.c"

## Single File - Shebang
header2 "Single File - Shebang"
run "./single_file.c\
         arg1 arg2 'arg three'" "./single_file.c"

# Multi. File - CLI
header2 "Multi. File - CLI"
run "$c 'multi_file_1.c multi_file_2.c -Wall -Werror -lm'\
         arg1 arg2 'arg three'" "multi_file_1.c"

# Multi. File - CLI
header2 "Multi. File - Shebang"
run "./multi_file_1.c\
         arg1 arg2 'arg three'" "./multi_file_1.c"
popd; echo
