#!/bin/sh

. ./unit_tests/utils.sh

echo "Testing numbers in 1 epression\n"

run_test "1+1" "2"
run_test " 1    	+	1" "2"
run_test "1+2" "3"
run_test "2+1" "3"
run_test "0*1" "0"
run_error "1/0" "Division by zero is impossible"
run_error "1 / (3 * 4 + (-12))" "Division by zero is impossible"

run_error "i[[42]]" "Matrix and imaginary unit cannot multiply each other"
