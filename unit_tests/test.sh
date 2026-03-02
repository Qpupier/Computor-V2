#!/bin/sh
run()
{
	echo "Testing: \"$1\""
	printf '%s\n' "$1" | ./computor-v2 > out 2> err
	sed -i 's/\x1b\[[0-9;]*m//g' out err
}

print_error()
{
	echo "Standard output:"
	cat out
	echo "Error output:"
	cat err
	rm -f expected
	exit 1
}

run_test()
{
	run $1
	printf '> %s\n> \n' "$2" > expected
	if ! diff -u expected out > /dev/null; then
		echo "Output differs from expected:"
		diff -u expected out
		print_error
	fi
	rm -f expected
	echo "\n"
}

run_error()
{
	run $1
	printf '%s\n' "$2" > expected
	if ! diff -u expected err >/dev/null; then
		echo "Error output differs from expected:"
		diff -u expected err
		print_error
	fi
	rm -f expected
	echo "\n"
}

run_test "1+1" "2"
run_test "1+2" "3"
run_test "2+1" "3"
run_test "0*1" "0"
run_error "1/0" "Division by zero is impossible"
run_error "i[[42]]" "Matrix and imaginary unit cannot multiply each other"
