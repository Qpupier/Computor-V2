#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting logic errors\033[0m\n"

status=0

run_error "42 / % 42" "Two operators cannot be adjacent" $1 ; status=$((status ^ $?))
run_error "i[[42]]" "Matrix and imaginary unit cannot multiply each other" $1 ; status=$((status ^ $?))

return $status
