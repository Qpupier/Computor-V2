#!/bin/sh

. ./unit_tests/utils.sh

status=0

echo "\033[34;1mTesting complexes in a single expression\033[0m\n"

run_test "i" "i" $1 ; status=$((status | $?))
run_test "i + 1" "1 + i" $1 ; status=$((status | $?))
run_test "2i * 4 + 2" "2 + 8i" $1 ; status=$((status | $?))
run_test "42i" "42i" $1 ; status=$((status | $?))
run_error "42 i" "No space allowed without operator" $1 ; status=$((status | $?))
run_error "i i" "No space allowed without operator" $1 ; status=$((status | $?))
run_error "i / 0" "Division by zero is impossible" $1 ; status=$((status | $?))
run_error "i % 0" "Modulo operator (%) cannot be applied to complex numbers" $1 ; status=$((status | $?))
run_error "i[[42]]" "Impossible operation between matrix and complex number" $1 ; status=$((status | $?))

echo "\n\033[34;1mTesting complexes in equations\033[0m\n"

run_test "i = 0" "False" $1 ; status=$((status | $?))
run_test "i^2 = -1" "True" $1 ; status=$((status | $?))
run_test "x = i^4" "x = 1" $1 ; status=$((status | $?))

return $status
