#!/bin/sh

. ./unit_tests/utils.sh

echo "Testing formatting\n"

run_error "" ""
run_error "()" "Invalid expression format"
