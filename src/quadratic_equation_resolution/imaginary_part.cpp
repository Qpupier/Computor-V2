/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imaginary_part.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:24:35 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/21 17:49:11 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static Rational*	get_initial_imaginary_term1(Complex* a, Complex* b)
{
	Rational*	a_imaginary_b_real;
	Rational*	a_real_b_imaginary;
	Rational*	result;

	a_imaginary_b_real = a->getImaginary() * b->getReal();
	a_real_b_imaginary = a->getReal() * b->getImaginary();
	result = *a_imaginary_b_real - *a_real_b_imaginary;
	delete a_imaginary_b_real;
	delete a_real_b_imaginary;
	return (result);
}

static void			imaginary_reduce_terms(\
		t_quadratic_solutions &solutions, Rational* imaginary_term1, 	\
		Rational* imaginary_term2_factor, 								\
		Rational* imaginary_term3_factor, 								\
		Rational* imaginary_denominator, Rational* imaginary_gcd)
{
	Rational* tmp;

	tmp = *imaginary_term1 / *imaginary_gcd;
	solutions.imaginary_term1 = tmp->getNumerator();
	delete tmp;
	tmp = *imaginary_term2_factor / *imaginary_gcd;
	solutions.imaginary_term2_factor = tmp->getNumerator();
	delete tmp;
	tmp = *imaginary_term3_factor / *imaginary_gcd;
	solutions.imaginary_term3_factor = tmp->getNumerator();
	delete tmp;
	tmp = *imaginary_denominator / *imaginary_gcd;
	solutions.imaginary_denominator = tmp->getDenominator();
	delete tmp;
}

void	set_imaginary_terms(Complex* a, Complex* b, 		\
		t_quadratic_solutions &solutions, Rational sqrt_real_factor, 	\
		Rational* sqrt_imaginary_factor, Rational* denominator)
{
	Rational*	imaginary_term1;
	Rational*	imaginary_term2_factor;
	Rational*	imaginary_term3_factor;
	Rational*	imaginary_denominator;
	Rational*	imaginary_gcd;

	imaginary_term1 = get_initial_imaginary_term1(a, b);
	imaginary_term2_factor = a->getImaginary() * sqrt_real_factor;
	imaginary_term3_factor = a->getReal() * *sqrt_imaginary_factor;
	imaginary_denominator = denominator;
	imaginary_gcd = get_gcd_terms(imaginary_term1, imaginary_term2_factor, imaginary_term3_factor, imaginary_denominator);
	imaginary_reduce_terms(solutions, imaginary_term1, imaginary_term2_factor, imaginary_term3_factor, imaginary_denominator, imaginary_gcd);
}
