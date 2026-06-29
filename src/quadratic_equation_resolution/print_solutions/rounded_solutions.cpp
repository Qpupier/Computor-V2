/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rounded_solutions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:27:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/29 10:31:50 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	sqrt_calculations(const t_quadratic_solutions &solutions, 	\
		const bool reduce_sqrt, InfiniteFloat &sqrt_real, 				\
		InfiniteFloat &sqrt_imaginary)
{
	InfiniteFloat	term1;

	if (reduce_sqrt)
	{
		sqrt_real = InfiniteFloat(solutions.sqrt_real).sqrt();
		sqrt_imaginary = InfiniteFloat(solutions.sqrt_imaginary).sqrt();
	}
	else
	{
		term1 = InfiniteFloat(solutions.sqrt_term1_factor) 	\
				* InfiniteFloat(solutions.sqrt_term1_sqrt).sqrt();
		sqrt_real = InfiniteFloat(term1 + solutions.sqrt_term2_real).sqrt();
		sqrt_imaginary 											\
				= InfiniteFloat(term1 + solutions.sqrt_term2_imaginary).sqrt();
	}
}

static void	rounded_calculations(const t_quadratic_solutions &solutions, 	\
		const InfiniteFloat sqrt[2], InfiniteFloat x_real[2], 			\
		InfiniteFloat x_imaginary[2])
{
	for (unsigned char i = 0; i < 2; i++)
	{
		x_real[i] = (solutions.real_term1[i] 								\
					+ solutions.real_term2_factor[i] * sqrt[0] 				\
					+ solutions.real_term3_factor[i] * sqrt[1]) 			\
				/ solutions.real_denominator[i];
		x_imaginary[i] = (solutions.imaginary_term1[i] 						\
					+ solutions.imaginary_term2_factor[i] * sqrt[0] 		\
					+ solutions.imaginary_term3_factor[i] * sqrt[1]) 		\
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

void		print_rounded_solutions(										\
		const t_quadratic_solutions &solutions, const bool reduce_sqrt, 	\
		const std::string var, const unsigned char nb_solutions)
{
	InfiniteFloat	sqrt[2];
	InfiniteFloat	sqrt_imaginary;
	InfiniteFloat	x_real[2];
	InfiniteFloat	x_imaginary[2];

	sqrt_calculations(solutions, reduce_sqrt, sqrt[0], sqrt[1]);
	rounded_calculations(solutions, sqrt, x_real, x_imaginary);
	if (nb_solutions == 1)
		return ;
	std::cout << COLOR_DIM;
	print_complex_rounded_value(var + (nb_solutions == 2 ? "1" : ""), 	\
			x_real[0], x_imaginary[0]);
	std::cout << std::endl;
	print_complex_rounded_value(var + (nb_solutions == 2 ? "2" : ""), 	\
			x_real[1], x_imaginary[1]);
	std::cout << COLOR_RESET << std::endl;
}
