#!/bin/sh

delete_files()
{
	rm -f output error expected
}

run()
{
	echo -n "\033[35;3mTesting:\033[0m \"$1\"\n$2 "
	echo "$1" | ./computor-v2 > output 2> error
	sed -i -z 's/\x1b\[[0-9;]*m//g; s/\n*$//' output error
}

run_output()
{
	echo -n "\033[35;3mTesting:\033[0m \"$1\"\n$2 "
	echo "$1" | ./computor-v2 > output 2>&1
	sed -i -z 's/\x1b\[[0-9;]*m//g; s/\n*$//' output
}

print_error()
{
	echo "\033[30mStandard output:\033[0m"
	cat output
	if [ -s output ]; then
		echo
	fi
	echo "\033[30mError output:\033[0m"
	cat error
	if [ -s error ]; then
		echo
	fi
	delete_files
	exit 1
}

test_leaks_and_errors()
{
	echo "$1" | valgrind --leak-check=full ./computor-v2 > output 2> error
	if ! grep -q "All heap blocks were freed -- no leaks are possible" error || ! grep -q "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" error; then
		echo "❌"
		if [ "$2" != "debug" ]; then
			return 1
		fi
		cat error
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
	echo -n "$2" > expected
	if ! diff -u expected output > /dev/null; then
		echo "❌"
		if [ "$3" != "debug" ]; then
			return 1
		fi
		echo "\n\033[30mOutput differs from expected:\033[0m"
		echo -n "\033[31m"
		diff -u expected output | grep -v "\ No newline at end of file"
		echo "\033[0m"
		print_error
	fi
	test_leaks_and_errors "$1" "$3"
}

run_error()
{
	run "$1" "$2" "$3"
	echo -n "$2" > expected
	if ! diff -u expected error > /dev/null; then
		echo "❌"
		if [ "$3" != "debug" ]; then
			return 1
		fi
		echo "\n\033[30mError output differs from expected:\033[0m"
		echo -n "\033[31m"
		diff -u expected error | grep -v "\ No newline at end of file"
		echo "\033[0m"
		print_error
	fi
	test_leaks_and_errors "$1" "$3"
}

run_batch()
{
	run_output "$1" "$2" "$3"
	echo -n "$2" > expected
	if ! diff -u expected output > /dev/null; then
		echo "❌"
		if [ "$3" != "debug" ]; then
			return 1
		fi
		echo "\n\033[30mOutput differs from expected:\033[0m"
		echo -n "\033[31m"
		diff -u expected output | grep -v "\ No newline at end of file"
		echo "\033[0m"
		delete_files
		exit 1
	fi
	test_leaks_and_errors "$1" "$3"
}
