#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting complexes in 1 expression\033[0m\n"

status=0

run_test "i" "i" $1 ; status=$((status ^ $?))
run_test "2i * 4 + 2" "2 + 8i" $1 ; status=$((status ^ $?))
run_test "42i" "42i" $1 ; status=$((status ^ $?))
run_error "42 i" "No space allowed without operator" $1 ; status=$((status ^ $?))
run_error "i i" "No space allowed without operator" $1 ; status=$((status ^ $?))
run_error "i / 0" "Division by zero is impossible" $1 ; status=$((status ^ $?))
run_error "i % 0" "Modulo operator (%) cannot be applied to complex numbers" $1 ; status=$((status ^ $?))

return $status
