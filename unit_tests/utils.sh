#!/bin/sh

delete_files()
{
	rm -f out err expected
}

run()
{
	echo "\nTesting: \"$1\""
	echo "$1" | make run > out 2> err
	truncate -s -1 out err
	sed -i 's/\x1b\[[0-9;]*m//g' out err
}

print_error()
{
	echo -n "\nStandard output:	["
	cat out
	echo -n "]\nError output:		["
	cat err
	echo "]"
	delete_files
	exit 1
}

test_leaks_and_errors()
{
	echo "$1" | make valgrind > out 2> err
	if ! grep -q "All heap blocks were freed -- no leaks are possible" err || ! grep -q "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" err; then
		cat err
		delete_files
		exit 1
	fi
	delete_files
}

run_test()
{
	run "$1"
	printf '%s' "$2" > expected
	if ! diff -u expected out > /dev/null; then
		echo "Output differs from expected:"
		diff -u expected out | grep -v "\ No newline at end of file"
		print_error
	fi
	test_leaks_and_errors
}

run_error()
{
	run "$1"
	printf '%s' "$2" > expected
	if ! diff -u expected err > /dev/null; then
		echo "Error output differs from expected:"
		diff -u expected err | grep -v "\ No newline at end of file"
		print_error
	fi
	test_leaks_and_errors
}
