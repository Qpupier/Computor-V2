#!/bin/sh

status=0

echo "\033[34;1mRUNNING UNIT TESTS\033[0m\n"
echo "\033[30m==============================\033[0m\n"

./unit_tests/format.sh ; status=$((status ^ $?))
echo "\n\033[30m==============================\033[0m\n"
./unit_tests/logic_error.sh ; status=$((status ^ $?))
echo "\n\033[30m==============================\033[0m\n"
./unit_tests/numbers.sh ; status=$((status ^ $?))

if [ $status -eq 0 ]; then
	echo "\n\033[32;1mAll tests passed! 🎉\033[0m"
else
	echo "\n\033[31;1mSome tests failed. ❌\033[0m"
fi

return $status
