#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting logic errors\033[0m\n"

run_error "i[[42]]" "Matrix and imaginary unit cannot multiply each other" $1
