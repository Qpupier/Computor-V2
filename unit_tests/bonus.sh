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

echo "\n\033[34;1mTesting vectors\033[0m\n"

run_test "[1, 2, 3]" "[1, 2, 3]" $1 ; status=$((status | $?))
run_test "[1, 2, 3] + [4, 5, 6]" "[5, 7, 9]" $1 ; status=$((status | $?))
run_test "[1, 2, 3][[4];[5];[6]]" "[ 32 ]" $1 ; status=$((status | $?))
run_test "[1, 2, 3] ** [[4];[5];[6]]" "[ 32 ]" $1 ; status=$((status | $?))
run_test "[[4];[5];[6]] ** [1, 2, 3]" "[ 4 , 8 , 12 ]
[ 5 , 10 , 15 ]
[ 6 , 12 , 18 ]" $1 ; status=$((status | $?))

run_error "[1, 2, 3] * [[4];[5];[6]]" "Matrix operation: incompatible dimensions" $1 ; status=$((status | $?))

echo "\n\033[34;1mTesting functions\033[0m\n"

run_test "e^2" "7.38905610..." $1 ; status=$((status | $?))
run_test "exp(2)" "7.38905610..." $1 ; status=$((status | $?))
run_test "||3i - 4||" "5" $1 ; status=$((status | $?))
run_test "norm(3i - 4)" "5" $1 ; status=$((status | $?))
run_test "|-42|" "42" $1 ; status=$((status | $?))
run_test "abs(-42)" "42" $1 ; status=$((status | $?))
run_test "cos(2pi)" "1.00000000..." $1 ; status=$((status | $?))
run_test "sin(3pi/2)" "-1.00000000..." $1 ; status=$((status | $?))
run_test "tan(0)" "0" $1 ; status=$((status | $?))
run_test "sqrt(9)" "3" $1 ; status=$((status | $?))
run_test "sqrt(2)" "1.41421356..." $1 ; status=$((status | $?))
run_test "sqrt(-1)" "i" $1 ; status=$((status | $?))
run_test "rad(180)" "3.14159265..." $1 ; status=$((status | $?))
run_test "deg(pi/2)" "90.00000000..." $1 ; status=$((status | $?))
run_test "5!" "120" $1 ; status=$((status | $?))
run_test "fact(5)" "120" $1 ; status=$((status | $?))

return $status
