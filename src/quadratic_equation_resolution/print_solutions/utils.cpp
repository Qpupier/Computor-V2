/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:22:50 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/13 12:31:41 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

std::string	get_sqrt_real(const t_quadratic_solutions &solutions)
{
	std::string	small_sqrt;
	int 		sqrt_term2_real;
	
	sqrt_term2_real = solutions.sqrt_term2_real;
	if (solutions.sqrt_term1_factor == -1)
		small_sqrt += "-";
	else if (solutions.sqrt_term1_factor != 1)
		small_sqrt += std::to_string(solutions.sqrt_term1_factor);
	small_sqrt += "√" + std::to_string(solutions.sqrt_term1_sqrt);
	if (sqrt_term2_real)
	{
		if (sqrt_term2_real > 0)
			small_sqrt += " + ";
		else
		{
			small_sqrt += " - ";
			sqrt_term2_real *= -1;
		}
		small_sqrt += std::to_string(sqrt_term2_real);
	}
	return (small_sqrt);
}

std::string	get_sqrt_imaginary(const t_quadratic_solutions &solutions)
{
	std::string	small_sqrt;
	int			sqrt_term2_imaginary;
	
	sqrt_term2_imaginary = solutions.sqrt_term2_imaginary;
	if (solutions.sqrt_term1_factor == -1)
		small_sqrt += "-";
	else if (solutions.sqrt_term1_factor != 1)
		small_sqrt += std::to_string(solutions.sqrt_term1_factor);
	small_sqrt += "√" + std::to_string(solutions.sqrt_term1_sqrt);
	if (sqrt_term2_imaginary)
	{
		if (sqrt_term2_imaginary > 0)
			small_sqrt += " + ";
		else
		{
			small_sqrt += " - ";
			sqrt_term2_imaginary *= -1;
		}
		small_sqrt += std::to_string(sqrt_term2_imaginary);
	}
	return (small_sqrt);
}

void		simplify_deepest_sqrt(t_quadratic_solutions &solutions)
{
	int	sqrt_term1;
	int	tmp;

	sqrt_term1 = solutions.sqrt_term1_factor * solutions.sqrt_term1_sqrt;
	solutions.sqrt_real = sqrt_term1 + solutions.sqrt_term2_real;
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.real_term2_factor[0], &tmp);
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.real_term2_factor[1], &tmp);
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.imaginary_term2_factor[0], &tmp);
	reduce_sqrt(&solutions.imaginary_term2_factor[1], &solutions.sqrt_real);
	solutions.sqrt_imaginary = sqrt_term1 + solutions.sqrt_term2_imaginary;
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.real_term3_factor[0], &tmp);
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.real_term3_factor[1], &tmp);
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.imaginary_term3_factor[0], &tmp);
	reduce_sqrt(&solutions.imaginary_term3_factor[1], 	\
			&solutions.sqrt_imaginary);
}
