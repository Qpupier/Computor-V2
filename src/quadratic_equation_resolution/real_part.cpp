/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_part.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:24:12 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/04 15:13:29 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static Rational*	get_initial_real_term1(Complex* a, Complex* b)
{
	Rational*	a_real_negative;
	Rational*	minus_a_real_b_real;
	Rational*	a_imaginary_b_imaginary;
	Rational*	result;

	a_real_negative = Rational(-1) * a->getReal();
	minus_a_real_b_real = *a_real_negative * b->getReal();
	delete a_real_negative;
	a_imaginary_b_imaginary = a->getImaginary() * b->getImaginary();
	result = *minus_a_real_b_real - *a_imaginary_b_imaginary;
	delete minus_a_real_b_real;
	delete a_imaginary_b_imaginary;
	return (result);
}

static void			real_reduce_terms(t_quadratic_solutions &solutions, 	\
	Rational* real_term1, Rational* real_term2_factor, 						\
	Rational* real_term3_factor, Rational* real_denominator, Rational* real_gcd)
{
	Rational*	tmp;

	tmp = *real_term1 / *real_gcd;
	solutions.real_term1[0] = tmp->getNumerator();
	solutions.real_term1[1] = tmp->getNumerator();
	delete tmp;
	tmp = *real_term2_factor / *real_gcd;
	solutions.real_term2_factor[0] = -tmp->getNumerator();
	solutions.real_term2_factor[1] = tmp->getNumerator();
	delete tmp;
	tmp = *real_term3_factor / *real_gcd;
	solutions.real_term3_factor[0] = -tmp->getNumerator();
	solutions.real_term3_factor[1] = tmp->getNumerator();
	delete tmp;
	tmp = *real_denominator / *real_gcd;
	solutions.real_denominator[0] = tmp->getNumerator();
	solutions.real_denominator[1] = tmp->getNumerator();
	delete tmp;
}

void	set_real_terms(Complex* a, Complex* b, 							\
		t_quadratic_solutions &solutions, Rational sqrt_real_factor, 	\
		Rational* sqrt_imaginary_factor, Rational* denominator)
{
	Rational*	real_term1;
	Rational*	real_term2_factor;
	Rational*	real_term3_factor;
	Rational*	real_denominator;
	Rational*	real_gcd;

	real_term1 = get_initial_real_term1(a, b);
	real_term2_factor = a->getReal() * sqrt_real_factor;
	real_term3_factor = a->getImaginary() * *sqrt_imaginary_factor;
	real_denominator = denominator;
	real_gcd = get_gcd_terms(real_term1, real_term2_factor, real_term3_factor, real_denominator);
	real_reduce_terms(solutions, real_term1, real_term2_factor, real_term3_factor, real_denominator, real_gcd);
}
