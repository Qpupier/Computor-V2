# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/02 20:27:05 by qpupier           #+#    #+#              #
#    Updated: 2026/03/02 20:27:44 by qpupier          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#!/bin/sh

. ./utils.sh

run_test "1+1" "2"
run_test "1+2" "3"
run_test "2+1" "3"
run_test "0*1" "0"
run_error "1/0" "Division by zero is impossible"
run_error "i[[42]]" "Matrix and imaginary unit cannot multiply each other"
