# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    utils.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/02 20:26:58 by qpupier           #+#    #+#              #
#    Updated: 2026/03/02 20:27:30 by qpupier          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

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
}
