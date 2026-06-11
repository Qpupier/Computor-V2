/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplify_factors.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 14:48:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/11 11:44:47 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	factorize(t_quadratic_solutions &solutions, int index)
{
	InfiniteInt	factor;

	factor = compute_gcd(compute_gcd(compute_gcd(	\
			solutions.real_term1[index], 			\
			solutions.real_term2_factor[index]), 	\
			solutions.real_term3_factor[index]), 	\
			solutions.real_denominator[index]);
	solutions.real_term1[index] /= factor;
	solutions.real_term2_factor[index] /= factor;
	solutions.real_term3_factor[index] /= factor;
	solutions.real_denominator[index] /= factor;
	factor = compute_gcd(compute_gcd(compute_gcd(		\
			solutions.imaginary_term1[index], 			\
			solutions.imaginary_term2_factor[index]), 	\
			solutions.imaginary_term3_factor[index]), 	\
			solutions.imaginary_denominator[index]);
	solutions.imaginary_term1[index] /= factor;
	solutions.imaginary_term2_factor[index] /= factor;
	solutions.imaginary_term3_factor[index] /= factor;
	solutions.imaginary_denominator[index] /= factor;
}

static void	simplify_factors_sqrt(t_quadratic_solutions &s, int i)
{
	if (!s.sqrt_real)
	{
		s.real_term2_factor[i] = 0;
		s.imaginary_term2_factor[i] = 0;
	}
	if (!s.sqrt_imaginary)
	{
		s.real_term3_factor[i] = 0;
		s.imaginary_term3_factor[i] = 0;
	}
}

static void	simplify_factors_terms(t_quadratic_solutions &s, int i)
{
	if (!s.real_term2_factor[i] || s.sqrt_real <= 1)
	{
		s.real_term1[i] += s.real_term2_factor[i] * s.sqrt_real;
		s.real_term2_factor[i] = 0;
	}
	if (!s.real_term3_factor[i] || s.sqrt_imaginary <= 1)
	{
		s.real_term1[i] += s.real_term3_factor[i] * s.sqrt_imaginary;
		s.real_term3_factor[i] = 0;
	}
	if (!s.imaginary_term2_factor[i] || s.sqrt_real <= 1)
	{
		s.imaginary_term1[i] += s.imaginary_term2_factor[i] * s.sqrt_real;
		s.imaginary_term2_factor[i] = 0;
	}
	if (!s.imaginary_term3_factor[i] || s.sqrt_imaginary <= 1)
	{
		s.imaginary_term1[i] += s.imaginary_term3_factor[i] * s.sqrt_imaginary;
		s.imaginary_term3_factor[i] = 0;
	}
}

void		simplify_factors(t_quadratic_solutions &s)
{
	for (int i = 0; i < 2; i++)
	{
		simplify_factors_sqrt(s, i);
		simplify_factors_terms(s, i);
		if (s.sqrt_real == s.sqrt_imaginary)
		{
			s.real_term2_factor[i] += s.real_term3_factor[i];
			s.real_term3_factor[i] = 0;
			s.imaginary_term2_factor[i] += s.imaginary_term3_factor[i];
			s.imaginary_term3_factor[i] = 0;
		}
		factorize(s, i);
	}
}

void		simplify_factors2(t_quadratic_solutions &solutions)
{
	for (int i = 0; i < 2; i++)
	{
		if (!solutions.sqrt_term2_real)
		{
			solutions.real_term2_factor[i] += solutions.real_term3_factor[i];
			solutions.real_term3_factor[i] = 0;
			solutions.imaginary_term2_factor[i] 	\
				+= solutions.imaginary_term3_factor[i];
			solutions.imaginary_term3_factor[i] = 0;
			factorize(solutions, i);
		}
	}
}
