#!/bin/bash

c="$PWD/../c"

shopt -s expand_aliases
alias pushd='pushd &>/dev/null'
alias popd='popd &>/dev/null'

# styles
       nc="\033[0m"
     bold="\033[1m"
  italics="\033[3m"
underline="\033[4m"

# colors
      red="\033[31m"
    green="\033[32m"
   yellow="\033[33m"
     cyan="\033[36m"

# counters
PASSES=0
 FAILS=0

header1() { echo -e "${italics}${bold}${yellow}$@${nc}"; }
header2() { echo -e "${italics}${yellow}$@${nc}"; }

assert() {
    echo -en "${cyan}$1${nc} - "

    if `eval "[[ $2 ]]"`; then
        echo -e "${green}pass: $2${nc}" 1>&2
        let PASSES++
    else
        echo -e "${red}fail: $2${nc}" 1>&2
        let FAILS++
    fi
}

# print status and quit
quit() {
    END=$(date +%s.%N)
    printf "${yellow}Duration: %0.1fs${nc}\n"\
           $(echo - | awk "{print $END - $START}")

    if [ $FAILS -gt 0 ]; then
        fail_color=$red
        pass_color=$cyan
    else
        fail_color=$cyan
        pass_color=$green
    fi

    echo
    echo -en "${cyan}$(($PASSES + $FAILS)) tests, ${nc}"
    echo -en "${pass_color}$PASSES passed, ${nc}"
    echo -en "${fail_color}$FAILS failed${nc}"
    echo

    exit $FAILS
}

# clear out the cache
export C_CACHE_PATH="$(mktemp -d -t c.XXX)"
rm -rf "$C_CACHE_PATH"

START=$(date +%s.%N)
