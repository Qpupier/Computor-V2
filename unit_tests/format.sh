#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting formatting\033[0m\n"

status=0

run_error "" "" $1 ; status=$((status ^ $?))
run_error " " "Invalid expression format" $1 ; status=$((status ^ $?))
run_error "42 = 42 = 42" "Too many '=' in the expression" $1 ; status=$((status ^ $?))
run_error "42 = ??" "Too many '?' in the expression" $1 ; status=$((status ^ $?))
run_error "()" "Invalid expression format" $1 ; status=$((status ^ $?))

return $status
