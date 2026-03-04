#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting formatting\033[0m\n"

run_error "" "" $1
run_error "()" "Invalid expression format" $1
