/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:19:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/04 17:52:22 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	print_real_part(t_quadratic_solutions &solutions, int index, bool reduce_sqrt)
{
	int	real_term2_factor;
	int	real_term3_factor;

	real_term2_factor = solutions.real_term2_factor[index];
	real_term3_factor = solutions.real_term3_factor[index];
	std::string sqrt_real = get_sqrt_real(solutions);
	std::string sqrt_imaginary = get_sqrt_imaginary(solutions);
	if (!solutions.real_term1[index] && !real_term2_factor && !real_term3_factor)
	{
		std::cout << "0";
		return ;
	}
	if (solutions.real_denominator[index] > 1)// [ ] mais pas si 1 seul term
			std::cout << "(";
	if (solutions.real_term1[index])
		std::cout << solutions.real_term1[index];
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
			std::cout << "√(" << sqrt_real << ")";
	}
	if (real_term3_factor)
	{
		if (solutions.real_term1[index] || real_term2_factor)
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
			std::cout << "√(" << sqrt_imaginary << ")";
	}
	if (solutions.real_denominator[index] > 1)
		std::cout << ") / " << solutions.real_denominator[index];
}

static bool	print_imaginary_part(t_quadratic_solutions &solutions, int index, bool reduce_sqrt)
{
	int	imaginary_term2_factor;
	int	imaginary_term3_factor;

	imaginary_term2_factor = solutions.imaginary_term2_factor[index];
	imaginary_term3_factor = solutions.imaginary_term3_factor[index];
	if (!solutions.imaginary_term1[index] && !imaginary_term2_factor && !imaginary_term3_factor)
		return false;
	std::string sqrt_real = get_sqrt_real(solutions);
	std::string sqrt_imaginary = get_sqrt_imaginary(solutions);
	std::cout << " + i(";// [ ] si 1 term ou plus
	if (solutions.imaginary_term1[index])
		std::cout << solutions.imaginary_term1[index];
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
			std::cout << "√" << solutions.sqrt_real;
		else
			std::cout << "√(" << sqrt_real << ")";
	}
	if (imaginary_term3_factor)
	{
		if (solutions.imaginary_term1[index] || imaginary_term2_factor)
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
			std::cout << "√(" << sqrt_imaginary << ")";
	}
	std::cout << ")";
	if (solutions.imaginary_denominator[index] > 1)
		std::cout << " / " << solutions.imaginary_denominator[index];
	return true;
}

static void	print_exact_solutions(t_quadratic_solutions &solutions, 	\
		std::string set, bool reduce_sqrt = false)
{
	bool	has_imaginary_part;

	std::cout << "S = {";
	print_real_part(solutions, 0, reduce_sqrt);
	has_imaginary_part = print_imaginary_part(solutions, 0, reduce_sqrt);
	std::cout << ", ";
	print_real_part(solutions, 1, reduce_sqrt);
	has_imaginary_part |= print_imaginary_part(solutions, 1, reduce_sqrt);
	std::cout << "} ∈ ";
	if (has_imaginary_part)
		std::cout << "ℂ" << std::endl;
	else
		std::cout << set << std::endl;
}

void		print_solutions(t_quadratic_solutions& solutions, 	\
		std::string set, std::string var)
{
	bool	reduce_sqrt;

	if (!solutions.sqrt_term1_factor || solutions.sqrt_term1_sqrt <= 1)
	{
		simplify_deepest_sqrt(solutions);
		simplify_factors(solutions);
		reduce_sqrt = true;
	}
	else
	{
		simplify_factors2(solutions);
		reduce_sqrt = false;
	}
	std::cout << COLOR_GREEN;
	print_exact_solutions(solutions, set, reduce_sqrt);
	std::cout << COLOR_RESET;
	print_rounded_solutions(solutions, reduce_sqrt, var);
}
