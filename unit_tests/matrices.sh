#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting matrices in 1 expression\033[0m\n"

status=0

run_test "[[42]]" "[ 42 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]]" "[ 1 , 2 ]
[ 3 , 4 ]" $1 ; status=$((status ^ $?))
run_test "[[42/3]]" "[ 14 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]] + [[5, 6];[7, 8]]" "[ 6 , 8 ]
[ 10 , 12 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]] - [[5, 6];[7, 8]]" "[ -4 , -4 ]
[ -4 , -4 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]] * [[5, 6];[7, 8]]" "[ 5 , 12 ]
[ 21 , 32 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]] ^ [[5, 6];[7, 8]]" "[ 1 , 64 ]
[ 2187 , 65536 ]" $1 ; status=$((status ^ $?))
run_test "[[42]] / [[3]]" "[ 14 ]" $1 ; status=$((status ^ $?))
run_test "[[5, 2]] % [[3, 3]]" "[ 2 , 2 ]" $1 ; status=$((status ^ $?))
run_test "[[0, 1, 2];[3, 4, 5]] ** [[6];[7];[8]]" "[ 23 ]
[ 86 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]][[5, 6];[7, 8]]" "[ 19 , 22 ]
[ 43 , 50 ]" $1 ; status=$((status ^ $?))
run_test "([[1, 2];[3, 4]])[[5, 6];[7, 8]]" "[ 19 , 22 ]
[ 43 , 50 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]]**[[5, 6];[7, 8]]" "[ 19 , 22 ]
[ 43 , 50 ]" $1 ; status=$((status ^ $?))
run_test "[[1, 2];[3, 4]]*[[5, 6];[7, 8]]" "[ 5 , 12 ]
[ 21 , 32 ]" $1 ; status=$((status ^ $?))

run_test "[[0, 1, 2]] + 3" "[ 3 , 4 , 5 ]" $1 ; status=$((status ^ $?))
run_test "[[0, 1, 2]] * 3" "[ 0 , 3 , 6 ]" $1 ; status=$((status ^ $?))
run_test "[[0, 1, 2]] / 2" "[ 0 , 1/2 , 1 ]" $1 ; status=$((status ^ $?))
run_test "[[0, 1, 2]] % 2" "[ 0 , 1 , 0 ]" $1 ; status=$((status ^ $?))
run_test "[[0, 1, 2]] ^ 3" "[ 0 , 1 , 8 ]" $1 ; status=$((status ^ $?))

run_error "[[1,2];[3]]" "Invalid matrix format: inconsistent row sizes" $1 ; status=$((status ^ $?))
run_error "[[42*]]" "Invalid matrix format: Operator cannot be at the beginning or end of an expression" $1 ; status=$((status ^ $?))
run_error "[[6];[7];[8]] ** [[0, 1, 2];[3, 4, 5]]" "Matrix operator error: incompatible dimensions" $1 ; status=$((status ^ $?))
run_error "[[0, 1, 2]] ** 42" "Matrix operator (**) can only be applied to matrices" $1 ; status=$((status ^ $?))
run_error "[[1,2]] / 0" "Division by zero is impossible" $1 ; status=$((status ^ $?))

return $status
