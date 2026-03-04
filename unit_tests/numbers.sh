#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting numbers in 1 expression\033[0m\n"

status=0

run_test "1+1" "2" $1 ; status=$((status ^ $?))
run_test " 1    	+	1" "2" $1 ; status=$((status ^ $?))
run_test "1+2" "3" $1 ; status=$((status ^ $?))
run_test "2+1" "3" $1 ; status=$((status ^ $?))
run_test "0*1" "0" $1 ; status=$((status ^ $?))
run_error "1/0" "Division by zero is impossible" $1 ; status=$((status ^ $?))
run_error "1 / (3 * 4 + (-12))" "Division by zero is impossible" $1 ; status=$((status ^ $?))

return $status
