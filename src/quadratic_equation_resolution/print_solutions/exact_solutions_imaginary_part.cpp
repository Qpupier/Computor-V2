/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions_imaginary_part.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 18:20:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/12 19:14:48 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	print_imaginary_term1(t_quadratic_solutions &solutions, int index)
{
	if (solutions.imaginary_term1[index])
		std::cout << solutions.imaginary_term1[index];
}

static void	print_imaginary_term2(t_quadratic_solutions &solutions, 	\
		int &imaginary_term2_factor, int index, bool reduce_sqrt)
{
	if (imaginary_term2_factor)
	{
		if (solutions.imaginary_term1[index])
		{
			if (imaginary_term2_factor > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				imaginary_term2_factor *= -1;
			}
		}
		if (imaginary_term2_factor == -1)
			std::cout << "-";
		else if (imaginary_term2_factor != 1)
			std::cout << imaginary_term2_factor;
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_imaginary;
		else
			std::cout << "√(" << get_sqrt_imaginary(solutions) << ")";
	}
}

static void	print_imaginary_term3(t_quadratic_solutions &solutions, 	\
		int &imaginary_term3_factor, int index, bool reduce_sqrt)
{
	if (imaginary_term3_factor)
	{
		if (solutions.imaginary_term1[index] 	\
				|| solutions.imaginary_term2_factor[index])
		{
			if (imaginary_term3_factor > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				imaginary_term3_factor *= -1;
			}
		}
		if (imaginary_term3_factor == -1)
			std::cout << "-";
		else if (imaginary_term3_factor != 1)
			std::cout << imaginary_term3_factor;
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_imaginary;
		else
			std::cout << "√(" << get_sqrt_imaginary(solutions) << ")";
	}
}

void		print_imaginary_part(t_quadratic_solutions &solutions, int index, \
		bool reduce_sqrt)
{
	int		imaginary_term2_factor;
	int		imaginary_term3_factor;
	bool	parentheses;

	imaginary_term2_factor = solutions.imaginary_term2_factor[index];
	imaginary_term3_factor = solutions.imaginary_term3_factor[index];
	parentheses = parentheses_needed(solutions.imaginary_term1[index], 	\
			imaginary_term2_factor, imaginary_term3_factor);
	if (parentheses)
		std::cout << "(";
	print_imaginary_term1(solutions, index);
	print_imaginary_term2(solutions, imaginary_term2_factor, index, 	\
			reduce_sqrt);
	print_imaginary_term3(solutions, imaginary_term3_factor, index, 	\
			reduce_sqrt);
	if (parentheses)
		std::cout << ")";
	std::cout << "i";
	if (solutions.imaginary_denominator[index] > 1)
		std::cout << " / " << solutions.imaginary_denominator[index];
}
