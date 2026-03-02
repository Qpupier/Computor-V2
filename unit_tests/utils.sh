#!/bin/sh

delete_files()
{
	rm -f out err expected
}

run()
{
	echo "\nTesting: \"$1\""
	printf '%s\n' "$1" | ./computor-v2 > out 2> err
	tail -c +3 out > out.tmp && mv out.tmp out && rm -f out.tmp
	truncate -s -4 out
	truncate -s -1 err
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

run_test()
{
	run "$1"
	printf '%s' "$2" > expected
	if ! diff -u expected out > /dev/null; then
		echo "Output differs from expected:"
		diff -u expected out | grep -v "\ No newline at end of file"
		print_error
	fi
	delete_files
}

run_error()
{
	run "$1"
	printf '%s' "$2" > expected
	if ! diff -u expected err >/dev/null; then
		echo "Error output differs from expected:"
		diff -u expected err | grep -v "\ No newline at end of file"
		print_error
	fi
	delete_files
}
