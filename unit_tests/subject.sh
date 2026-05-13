#!/bin/sh

. ./unit_tests/utils.sh

echo "\033[34;1mTesting subject examples\033[0m\n"

status=0

run_batch "$(cat unit_tests/subject_tests/test1_*.txt)" "$(cat unit_tests/subject_tests/solution1_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test2_*.txt)" "$(cat unit_tests/subject_tests/solution2_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test3_*.txt)" "$(cat unit_tests/subject_tests/solution3_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test4_*.txt)" "$(cat unit_tests/subject_tests/solution4_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test5_*.txt)" "$(cat unit_tests/subject_tests/solution5_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test6_*.txt)" "$(cat unit_tests/subject_tests/solution6_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test7_*.txt)" "$(cat unit_tests/subject_tests/solution7_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test8_*.txt)" "$(cat unit_tests/subject_tests/solution8_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test9_*.txt)" "$(cat unit_tests/subject_tests/solution9_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test10_*.txt)" "$(cat unit_tests/subject_tests/solution10_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test11_*.txt)" "$(cat unit_tests/subject_tests/solution11_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test12_*.txt)" "$(cat unit_tests/subject_tests/solution12_*.txt)" $1 ; status=$((status | $?))
run_batch "$(cat unit_tests/subject_tests/test13_*.txt)" "$(cat unit_tests/subject_tests/solution13_*.txt)" $1 ; status=$((status | $?))

return $status
