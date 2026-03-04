#!/bin/sh

delete_files()
{
	rm -f out err expected
}

run()
{
	echo -n "\033[35;3mTesting:\033[0m \"$1\" => \"$2\" "
	echo "$1" | MAKEFLAGS=--no-print-directory make run > out 2> err
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
	echo "$1" | MAKEFLAGS=--no-print-directory make valgrind > out 2> err
	if ! grep -q "All heap blocks were freed -- no leaks are possible" err || ! grep -q "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" err; then
		echo "❌"
		if [ "$2" != "Github" ]; then
			return 1
		fi
		cat err
		delete_files
		exit 1
	fi
	echo "✅"
	delete_files
	return 0
}

run_test()
{
	run "$1" "$2" "$3"
	printf '%s' "$2" > expected
	if ! diff -u expected out > /dev/null; then
		echo "❌"
		if [ "$3" != "Github" ]; then
			return 1
		fi
		echo "Output differs from expected:"
		diff -u expected out | grep -v "\ No newline at end of file"
		print_error
	fi
	test_leaks_and_errors "$1" "$3"
}

run_error()
{
	run "$1" "$2" "$3"
	printf '%s' "$2" > expected
	if ! diff -u expected err > /dev/null; then
		echo "❌"
		if [ "$3" != "Github" ]; then
			return 1
		fi
		echo "Error output differs from expected:"
		diff -u expected err | grep -v "\ No newline at end of file"
		print_error
	fi
	test_leaks_and_errors "$1" "$3"
}
