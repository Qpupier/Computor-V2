#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting numbers in 1 expression\033[0m\n"

status=0

run_test "1+1" "2" $1 ; status=$((status | $?))
run_test " 1    	+	1" "2" $1 ; status=$((status | $?))
run_test "1+2" "3" $1 ; status=$((status | $?))
run_test "2+1" "3" $1 ; status=$((status | $?))
run_test "0*1" "0" $1 ; status=$((status | $?))
run_test "2((3 * 4 % 5)6 - (-3^2))" "42" $1 ; status=$((status | $?))
run_test "4.2 / 3" "7/5
1.4" $1 ; status=$((status | $?))
run_test "2/6" "1/3
0.333333" $1 ; status=$((status | $?))
run_test "42( 3 * 4 )" "504" $1 ; status=$((status | $?))
run_test "2 + 4 *2 - 5 %4 + 2 * (4 + 5)" "27" $1 ; status=$((status | $?))
run_test "-1" "-1" $1 ; status=$((status | $?))
run_test "(-1)" "-1" $1 ; status=$((status | $?))
run_test "-(1+2)" "-3" $1 ; status=$((status | $?))
run_test "(((((42)))))" "42" $1 ; status=$((status | $?))
run_test "0" "0" $1 ; status=$((status | $?))
run_test "0003" "3" $1 ; status=$((status | $?))
run_test "1000000*0" "0" $1 ; status=$((status | $?))
run_test "1 - (-1)" "2" $1 ; status=$((status | $?))
run_error "42 42" "No space allowed without operator" $1 ; status=$((status | $?))
run_error "1/0" "Division by zero is impossible" $1 ; status=$((status | $?))
run_error "1 / (3 * 4 + (-12))" "Division by zero is impossible" $1 ; status=$((status | $?))

run_test "1 + i" "1 + i" $1 ; status=$((status | $?))

run_test "3 + [[1,2];[3,4]]" "[ 4 , 5 ]
[ 6 , 7 ]" $1 ; status=$((status | $?))

return $status
