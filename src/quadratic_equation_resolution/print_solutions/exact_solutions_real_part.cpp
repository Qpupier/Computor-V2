/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions_real_part.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 18:20:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/29 10:46:19 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	print_real_term1(const t_quadratic_solutions &solutions, 	\
		const unsigned char index)
{
	if (solutions.real_term1[index])
		std::cout << solutions.real_term1[index];
}

static void	print_real_term2(const t_quadratic_solutions &solutions, 	\
		InfiniteInt &real_term2_factor, const unsigned char index, 		\
		const bool reduce_sqrt)
{
	if (real_term2_factor)
	{
		if (solutions.real_term1[index])
		{
			if (real_term2_factor > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				real_term2_factor *= -1;
			}
		}
		if (real_term2_factor == -1)
			std::cout << "-";
		else if (real_term2_factor != 1)
			std::cout << real_term2_factor;
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_real;
		else
			std::cout << "√(" << get_sqrt_real(solutions) << ")";
	}
}

static void	print_real_term3(const t_quadratic_solutions &solutions, 	\
		InfiniteInt &real_term3_factor, const unsigned char index, 		\
		const bool reduce_sqrt)
{
	if (real_term3_factor)
	{
		if (solutions.real_term1[index] || solutions.real_term2_factor[index])
		{
			if (real_term3_factor > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				real_term3_factor *= -1;
			}
		}
		if (real_term3_factor == -1)
			std::cout << "-";
		else if (real_term3_factor != 1)
			std::cout << real_term3_factor;
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_imaginary;
		else
			std::cout << "√(" << get_sqrt_imaginary(solutions) << ")";
	}
}

void		print_real_part(const t_quadratic_solutions &solutions, 	\
		const unsigned char index, const bool reduce_sqrt)
{
	InfiniteInt	real_term2_factor;
	InfiniteInt	real_term3_factor;
	bool		parentheses;

	real_term2_factor = solutions.real_term2_factor[index];
	real_term3_factor = solutions.real_term3_factor[index];
	if (!solutions.real_term1[index] && !real_term2_factor 			\
			&& !real_term3_factor)
	{
		std::cout << "0";
		return ;
	}
	parentheses = parentheses_needed(solutions.real_term1[index], 	\
			real_term2_factor, real_term3_factor);
	if (parentheses && solutions.real_denominator[index] > 1)
		std::cout << "(";
	print_real_term1(solutions, index);
	print_real_term2(solutions, real_term2_factor, index, reduce_sqrt);
	print_real_term3(solutions, real_term3_factor, index, reduce_sqrt);
	if (solutions.real_denominator[index] > 1)
	{
		if (parentheses)
			std::cout << ")";
		std::cout << " / " << solutions.real_denominator[index];
	}
}
