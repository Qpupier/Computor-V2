#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting matrices in 1 expression\033[0m\n"

status=0

run_test "[[42]]" "[ 42 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]]" "[ 1 , 2 ]
[ 3 , 4 ]" $1 ; status=$((status ^ $?))
run_test "[[42/3]]" "[ 14 ]" $1 ; status=$((status ^ $?))
run_error "[[1,2];[3]]" "Invalid matrix format: inconsistent row sizes" $1 ; status=$((status ^ $?))
run_error "[[42*]]" "Invalid matrix format: Operator cannot be at the beginning or end of an expression" $1 ; status=$((status ^ $?))

return $status
