#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting matrix inversion\033[0m\n"

status=0

run_test "~[[42]]" "[ 1/42 ]
[ 0.02380952... ]" $1 ; status=$((status | $?))
run_test "~[[1, 0, 2, 0];[1, 1, 0, 0];[1, 2, 1, 1];[0, 0, 1, 1]]" "[ 0 , 2 , -1 , 1 ]
[ 0 , -1 , 1 , -1 ]
[ 1/2 , -1 , 1/2 , -1/2 ]
[ -1/2 , 1 , -1/2 , 3/2 ]
[ 0 , 2 , -1 , 1 ]
[ 0 , -1 , 1 , -1 ]
[ 0.5 , -1 , 0.5 , -0.5 ]
[ -0.5 , 1 , -0.5 , 1.5 ]" $1 ; status=$((status | $?))

run_error "~42" "Matrix inversion is only possible for square matrices" $1 ; status=$((status | $?))

return $status
