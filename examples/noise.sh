#!/usr/bin/env bash
################################################################################
# I found this really cool idea online, and just wanted to post it here...
#
# Run it with: ./noise.sh
#
# Links:
#   http://www.xkcdb.com/9067
#   http://apple.stackexchange.com/q/74619
#   https://www.youtube.com/watch?v=GtQdIYUtAHg
################################################################################

# our "algorithms"
tiznut="putchar(t*(t>>12|t>>8&63&t>>4));"
jeteez="putchar(t*(t>>11|t>>8&123&t>>3));"
 xkcdb="putchar(((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6));"

# determine if we're going to use 'play' or 'aplay'
if type aplay; then
    play_cmd="aplay -c2 -t raw"
elif type play; then
    play_cmd="play -c2 -b 8 -e unsigned-integer -traw"
else
    >&2 echo "error: could not find 'play' or 'aplay'"
    exit 1
fi

# determine where c is...
if type ../c; then
    c="../c"
elif type c; then
    c="c"
else
    >&2 echo "error: you don't have c installed!"
    exit 1
fi

# exit on Ctrl-C
trap "exit 1" SIGINT

# generate and play noise
algo() {
    algo=$1
    samples=$2
    rate=$3

    cat << EOF > .tmp.c
    #include <stdio.h>
    void main(void) {
        for (int t=0; t < $samples; t++) { $algo }
    }
EOF
    c .tmp.c | $play_cmd -r $rate -
    rm .tmp.c
}

# execute each of our algorithms
algo $tiznut 52000  8000
algo $xkcdb  100000 8000
algo $tiznut 250000 48000
algo $jeteez 100000 8000
