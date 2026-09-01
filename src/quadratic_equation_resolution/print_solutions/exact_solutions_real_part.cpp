/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions_real_part.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 18:20:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/01 14:51:46 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static std::string	print_real_term1(	\
		const t_quadratic_solutions &solutions, const unsigned char index)
{
	std::string result;

	if (solutions.real_term1[index])
		result += solutions.real_term1[index].to_string();
	return (result);
}

static std::string	print_real_term2(								\
		const t_quadratic_solutions &solutions, 					\
		InfiniteInt &real_term2_factor, const unsigned char index, 	\
		const bool reduce_sqrt)
{
	std::string	result;

	if (real_term2_factor)
	{
		if (solutions.real_term1[index])
		{
			if (real_term2_factor > 0)
				result += " + ";
			else
			{
				result += " - ";
				real_term2_factor *= -1;
			}
		}
		if (real_term2_factor == -1)
			result += "-";
		else if (real_term2_factor != 1)
			result += real_term2_factor.to_string();
		if (reduce_sqrt)
			result += "√" + solutions.sqrt_real.to_string();
		else
			result += "√(" + get_sqrt_real(solutions) + ")";
	}
	return (result);
}

static std::string	print_real_term3(								\
		const t_quadratic_solutions &solutions, 					\
		InfiniteInt &real_term3_factor, const unsigned char index, 	\
		const bool reduce_sqrt)
{
	std::string	result;

	if (real_term3_factor)
	{
		if (solutions.real_term1[index] || solutions.real_term2_factor[index])
		{
			if (real_term3_factor > 0)
				result += " + ";
			else
			{
				result += " - ";
				real_term3_factor *= -1;
			}
		}
		if (real_term3_factor == -1)
			result += "-";
		else if (real_term3_factor != 1)
			result += real_term3_factor.to_string();
		if (reduce_sqrt)
			result += "√" + solutions.sqrt_imaginary.to_string();
		else
			result += "√(" + get_sqrt_imaginary(solutions) + ")";
	}
	return (result);
}

std::string			print_real_part(										\
		const t_quadratic_solutions &solutions, const unsigned char index, 	\
		const bool reduce_sqrt)
{
	InfiniteInt	real_term2_factor;
	InfiniteInt	real_term3_factor;
	std::string	result;
	bool		parentheses;

	real_term2_factor = solutions.real_term2_factor[index];
	real_term3_factor = solutions.real_term3_factor[index];
	if (!solutions.real_term1[index] && !real_term2_factor 			\
			&& !real_term3_factor)
		return (std::string("0"));
	parentheses = parentheses_needed(solutions.real_term1[index], 	\
			real_term2_factor, real_term3_factor);
	if (parentheses && solutions.real_denominator[index] > 1)
		result += "(";
	result += print_real_term1(solutions, index);
	result += print_real_term2(solutions, real_term2_factor, index, reduce_sqrt);
	result += print_real_term3(solutions, real_term3_factor, index, reduce_sqrt);
	if (solutions.real_denominator[index] > 1)
	{
		if (parentheses)
			result += ")";
		result += " / " + solutions.real_denominator[index].to_string();
	}
	return (result);
}
