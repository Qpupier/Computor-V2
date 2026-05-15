#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting polynomials in a single expression\033[0m\n"

status=0

run_test "2 * varA - 5 %4" "2varA - 1" $1 ; status=$((status | $?))
run_test "4 -5 + (x + 2)^2 - 4" "x^2 + 4x - 1" $1 ; status=$((status | $?))
run_test "4x + 5 - 2" "4x + 3" $1 ; status=$((status | $?))
run_test "2 * (4 + varA + 3)" "2varA + 14" $1 ; status=$((status | $?))
run_test "2 *(2 + 4 *varC -4 /3)" "(24varC + 4) / 3" $1 ; status=$((status | $?))
run_test "2*b+b" "3b" $1 ; status=$((status | $?))
run_test "2* y + 4 -2 * 4+1/3" "(6y - 11) / 3" $1 ; status=$((status | $?))
run_test "-x^2 +i * x" "-x^2 + i * x" $1 ; status=$((status | $?))
run_test "-1 * x^2 + i*x -3x +1" "-x^2 + (i - 3)x + 1" $1 ; status=$((status | $?))
run_test "x^2 +i * x -3x +1 -x2i" "x^2 - (3 + i)x + 1" $1 ; status=$((status | $?))

run_error "43 * y / (4 % 2 * y)" "Division by zero is impossible" $1 ; status=$((status | $?))

echo "\n\033[34;1mTesting polynomials in equations\033[0m\n"

run_test "2 * varA - 5 %4 = 0" "varA = 1/2
varA = 0.5" $1 ; status=$((status | $?))
run_test "4 -5 + (x + 2)^2 - 4 = 0" "x^2 + 4x - 1 = 0
Δ = 20
Two solutions in ℝ:
S = {-2 - √5, -2 + √5} ∈ ℝ
x1 ≈ -4.23607
x2 ≈ 0.236068" $1 ; status=$((status | $?))
run_test "x^2 + (-2 - 2i)x + 2i = 0" "x^2 - (2 + 2i)x + 2i = 0
Δ = 0
One solution in ℂ:
S = {1 + 1i} ∈ ℂ
x = 1 + i" $1 ; status=$((status | $?))
run_test "x^2 - (1 + 3i / 2)x - 5 / 16 + 3i / 4 = 0" "(16x^2 - (16 + 24i)x + (12i - 5)) / 16 = 0
Δ = 0
One solution in ℂ:
S = {1 / 2 + 3i / 4} ∈ ℂ
x = 1/2 + 3i/4
x = 0.5 + 0.75i" $1 ; status=$((status | $?))

return $status
