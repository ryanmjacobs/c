#!/bin/bash

# Ensure files are loaded relative to test.sh
cd "$(dirname "$BASH_SOURCE")"

# Load helpers
source "./test_helpers.sh"

# Run tests
source ./argument_and_link_test/test.sh
source ./complex_arguments/test.sh
source ./stdin/test.sh
source ./C++/test.sh
source ./shell/test.sh
source ./examples/test.sh
source ./header/test.sh

quit
