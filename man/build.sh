#!/bin/bash
# build.sh
# Thu Dec  4 18:55:03 PST 2025

set -eu
set -o pipefail

set -x
hash ronn
ronn --pipe --roff "./c.1.ronn" > "./c.1"
