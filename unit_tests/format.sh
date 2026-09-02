#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting formatting\033[0m\n"

status=0

run_error "" "" $1 ; status=$((status | $?))
run_error " " "" $1 ; status=$((status | $?))
run_error "42 = 42 = 42" "Too many '=' in the expression" $1 ; status=$((status | $?))
run_error "42 = ??" "Too many '?' in the expression" $1 ; status=$((status | $?))
run_error "()" "Brackets (parentheses, absolute values or norms) cannot be empty" $1 ; status=$((status | $?))
run_error "42()" "Brackets (parentheses, absolute values or norms) cannot be empty" $1 ; status=$((status | $?))
run_error "*1" "Operator cannot be at the beginning or end of an expression" $1 ; status=$((status | $?))
run_error "1*/1" "Two operators cannot be adjacent" $1 ; status=$((status | $?))

return $status
