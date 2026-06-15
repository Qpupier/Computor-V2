/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions_imaginary_part.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 18:20:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 14:12:22 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static std::string	print_imaginary_term1(	\
		const t_quadratic_solutions &solutions, const unsigned char index)
{
	std::string result;

	if (solutions.imaginary_term1[index] 	\
			&& solutions.imaginary_term1[index] != 1)
		result += solutions.imaginary_term1[index].to_string();
	return (result);
}

static std::string	print_imaginary_term2(									\
		const t_quadratic_solutions &solutions, 							\
		InfiniteInt &imaginary_term2_factor, const unsigned char index, 	\
		const bool reduce_sqrt)
{
	std::string result;

	if (imaginary_term2_factor)
	{
		if (solutions.imaginary_term1[index])
		{
			if (imaginary_term2_factor > 0)
				result += " + ";
			else
			{
				result += " - ";
				imaginary_term2_factor *= -1;
			}
		}
		if (imaginary_term2_factor == -1)
			result += "-";
		else if (imaginary_term2_factor != 1)
			result += imaginary_term2_factor.to_string();
		if (reduce_sqrt)
			result += "√" + solutions.sqrt_real.to_string();
		else
			result += "√(" + get_sqrt_real(solutions) + ")";
	}
	return (result);
}

static std::string	print_imaginary_term3(									\
		const t_quadratic_solutions &solutions, 							\
		InfiniteInt &imaginary_term3_factor, const unsigned char index, 	\
		const bool reduce_sqrt)
{
	std::string result;

	if (imaginary_term3_factor)
	{
		if (solutions.imaginary_term1[index] 	\
				|| solutions.imaginary_term2_factor[index])
		{
			if (imaginary_term3_factor > 0)
				result += " + ";
			else
			{
				result += " - ";
				imaginary_term3_factor *= -1;
			}
		}
		if (imaginary_term3_factor == -1)
			result += "-";
		else if (imaginary_term3_factor != 1)
			result += imaginary_term3_factor.to_string();
		if (reduce_sqrt)
			result += "√" + solutions.sqrt_imaginary.to_string();
		else
			result += "√(" + get_sqrt_imaginary(solutions) + ")";
	}
	return (result);
}

std::string			print_imaginary_part(									\
		const t_quadratic_solutions &solutions, const unsigned char index, 	\
		const bool reduce_sqrt)
{
	InfiniteInt	imaginary_term2_factor;
	InfiniteInt	imaginary_term3_factor;
	std::string	result;
	bool		parentheses;

	imaginary_term2_factor = solutions.imaginary_term2_factor[index];
	imaginary_term3_factor = solutions.imaginary_term3_factor[index];
	parentheses = parentheses_needed(solutions.imaginary_term1[index], 	\
			imaginary_term2_factor, imaginary_term3_factor);
	if (parentheses)
		result += "(";
	result += print_imaginary_term1(solutions, index);
	result += print_imaginary_term2(solutions, imaginary_term2_factor, 	\
		index, reduce_sqrt);
	result += print_imaginary_term3(solutions, imaginary_term3_factor, 	\
			index, reduce_sqrt);
	if (parentheses)
		result += ")";
	result += "i";
	if (solutions.imaginary_denominator[index] > 1)
		result += " / " + solutions.imaginary_denominator[index].to_string();
	return (result);
}
