#!/bin/bash

source "./test_helpers.sh"

# Path to c
c="$(cd ../; pwd)/c"

##
# Test #1: Argument parsing and linking capabilities.
#
# $1="command to run C file"
# $2="expected value of argv[0]"
##
argument_and_link() {
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

### Argument and Link Test
pushd argument_and_link_test
header1 "Test #1: Argument parsing and linking capabilities."

## Single File - CLI
header2 "Single File - CLI"
argument_and_link "$c 'single_file.c -Wall -Werror -lm'\
                         arg1 arg2 'arg three'" "single_file.c"

## Single File - Shebang
header2 "Single File - Shebang"
argument_and_link "./single_file.c\
                         arg1 arg2 'arg three'" "./single_file.c"

# Multi. File - CLI
header2 "Multi. File - CLI"
argument_and_link "$c 'multi_file_1.c multi_file_2.c -Wall -Werror -lm'\
                         arg1 arg2 'arg three'" "multi_file_1.c"

# Multi. File - CLI
header2 "Multi. File - Shebang"
argument_and_link "./multi_file_1.c\
                         arg1 arg2 'arg three'" "./multi_file_1.c"
popd
### END Argument and Link Test

# print status and quit
quit
