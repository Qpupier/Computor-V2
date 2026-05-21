/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rounded_solutions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:27:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/21 18:39:38 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	sqrt_calculations(bool reduce_sqrt, 							\
		const t_quadratic_solutions &solutions, InfiniteDouble &sqrt_real, 	\
		InfiniteDouble &sqrt_imaginary)
{
	InfiniteDouble	term1;

	if (reduce_sqrt)
	{
		sqrt_real = InfiniteDouble(solutions.sqrt_real).sqrt();
		sqrt_imaginary = InfiniteDouble(solutions.sqrt_imaginary).sqrt();
	}
	else
	{
		term1 = InfiniteDouble(solutions.sqrt_term1_factor) 	\
				* InfiniteDouble(solutions.sqrt_term1_sqrt).sqrt();
		sqrt_real = InfiniteDouble(term1 + solutions.sqrt_term2_real).sqrt();
		sqrt_imaginary 											\
				= InfiniteDouble(term1 + solutions.sqrt_term2_imaginary).sqrt();
	}
}

static void	rounded_calculations(const t_quadratic_solutions &solutions, 	\
		InfiniteDouble sqrt_real, InfiniteDouble sqrt_imaginary, InfiniteDouble x_real[2], 			\
		InfiniteDouble x_imaginary[2])
{
	for (unsigned char i = 0; i < 2; i++)
	{
		x_real[i] = (solutions.real_term1[i] 						\
				+ solutions.real_term2_factor[i] * sqrt_real 		\
				+ solutions.real_term3_factor[i] * sqrt_imaginary) 	\
				/ solutions.real_denominator[i];
		x_imaginary[i] = (solutions.imaginary_term1[i] 				\
				+ solutions.imaginary_term2_factor[i] * sqrt_real 	\
				+ solutions.imaginary_term3_factor[i] * sqrt_imaginary) \
				/ solutions.imaginary_denominator[i];
	}
	if ((x_imaginary[0] == 0 && x_imaginary[1] == 0 						\
				&& x_real[0] > x_real[1]) 									\
			|| (x_imaginary[0] != 0 && x_imaginary[1] != 0 					\
				&& x_real[0] * x_real[0] + x_imaginary[0] * x_imaginary[0] 	\
				> x_real[1] * x_real[1] + x_imaginary[1] * x_imaginary[1]))
	{
		std::swap(x_real[0], x_real[1]);
		std::swap(x_imaginary[0], x_imaginary[1]);
	}
}

static void	print_solution(std::string var, InfiniteDouble x_real[2], 	\
		InfiniteDouble x_imaginary[2], unsigned char i, bool print_solution_number)
{
	std::cout << var;
	if (print_solution_number)
		std::cout << i + 1;
	if (x_real[i] == x_real[i].getIntegerPart() && x_imaginary[i] == x_imaginary[i].getIntegerPart())// TODO: Methode isInteger()
		std::cout << " = ";
	else
		std::cout << " ≈ ";
	if (x_real[i] != 0 || x_imaginary[i] == 0)
		std::cout << x_real[i];
	if (x_imaginary[i] > 0)
	{
		if (x_real[i] != 0)
			std::cout << " + ";
		std::cout << x_imaginary[i] << "i";
	}
	else if (x_imaginary[i] < 0)
	{
		if (x_real[i] != 0)
			std::cout << " - ";
		else
			std::cout << "-";
		std::cout << -x_imaginary[i] << "i";
	}
}

void		print_rounded_solutions(								\
		const t_quadratic_solutions &solutions, bool reduce_sqrt, 	\
		std::string var, unsigned char nb_solutions)
{
	InfiniteDouble	sqrt_real;
	InfiniteDouble	sqrt_imaginary;
	InfiniteDouble	x_real[2];
	InfiniteDouble	x_imaginary[2];

	sqrt_calculations(reduce_sqrt, solutions, sqrt_real, sqrt_imaginary);
	rounded_calculations(solutions, sqrt_real, sqrt_imaginary, 	\
			x_real, x_imaginary);
	if (nb_solutions == 1)
		return ;
	std::cout << COLOR_DIM;
	print_solution(var, x_real, x_imaginary, 0, nb_solutions == 2);
	std::cout << std::endl;
	print_solution(var, x_real, x_imaginary, 1, nb_solutions == 2);
	std::cout << COLOR_RESET << std::endl;
}
