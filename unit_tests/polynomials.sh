#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting polynomials in 1 expression\033[0m\n"

status=0

run_test "2 * varA - 5 %4" "2varA - 1" $1 ; status=$((status | $?))
run_test "4 -5 + (x + 2)^2 - 4" "x^2 + 4x - 1" $1 ; status=$((status | $?))
run_test "4x + 5 - 2" "4x + 3" $1 ; status=$((status | $?))
run_test "2 * (4 + varA + 3)" "2varA + 14" $1 ; status=$((status | $?))
run_test "2 *(2 + 4 *varC -4 /3)" "8varC + 4/3" $1 ; status=$((status | $?))
run_test "2*b+b" "3b" $1 ; status=$((status | $?))
run_test "2* y + 4 -2 * 4+1/3" "2y - 11/3" $1 ; status=$((status | $?))
run_test "-x^2 +i * x" "-x^2 + i * x" $1 ; status=$((status | $?))
run_test "-1 * x^2 + i*x -3x +1" "-x^2 + (i - 3)x + 1" $1 ; status=$((status | $?))

run_error "43 * y / (4 % 2 * y)" "Unsupported: Division not supported between polynomials with these powers" $1 ; status=$((status | $?))

return $status
