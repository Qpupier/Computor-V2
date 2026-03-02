#!/bin/sh

. ./unit_tests/utils.sh

echo "Testing logic errors\n"

run_error "i[[42]]" "Matrix and imaginary unit cannot multiply each other"
