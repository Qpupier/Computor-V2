/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_part.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:24:12 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/01 19:08:55 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static Rational*	get_initial_real_term1(const Complex* a, const Complex* b)
{
	Rational*	a_real_negative;
	Rational*	minus_a_real_b_real;
	Rational*	a_imaginary_b_imaginary;
	Rational*	result;

	a_real_negative = dynamic_cast<Rational*>(-*a->getReal());
	minus_a_real_b_real = dynamic_cast<Rational*>(*a_real_negative * *b->getReal());
	delete a_real_negative;
	a_imaginary_b_imaginary = dynamic_cast<Rational*>(*a->getImaginary() * *b->getImaginary());
	result = dynamic_cast<Rational*>(*minus_a_real_b_real - *a_imaginary_b_imaginary);
	delete minus_a_real_b_real;
	delete a_imaginary_b_imaginary;
	return (result);
}

static void			update_real_term1(t_quadratic_solutions &solutions, 	\
		const Rational* real_term1, const Rational* real_gcd)
{
	Rational*	tmp;

	tmp = *real_term1 / *real_gcd;
	solutions.real_term1[0] = tmp->getNumerator();
	solutions.real_term1[1] = tmp->getNumerator();
	delete tmp;
}

static void			update_real_factors(t_quadratic_solutions &solutions, 	\
		const Rational* real_term2_factor, 									\
		const Rational* real_term3_factor, const Rational* real_gcd)
{
	Rational*	tmp;

	tmp = *real_term2_factor / *real_gcd;
	solutions.real_term2_factor[0] = -tmp->getNumerator();
	solutions.real_term2_factor[1] = tmp->getNumerator();
	delete tmp;
	tmp = *real_term3_factor / *real_gcd;
	solutions.real_term3_factor[0] = -tmp->getNumerator();
	solutions.real_term3_factor[1] = tmp->getNumerator();
	delete tmp;
}

static void			update_real_denominator(	\
		t_quadratic_solutions &solutions, 		\
		const Rational* real_denominator, const Rational* real_gcd)
{
	Rational*	tmp;

	tmp = *real_denominator / *real_gcd;
	solutions.real_denominator[0] = tmp->getNumerator();
	solutions.real_denominator[1] = tmp->getNumerator();
	delete tmp;
}

void				set_terms(								\
		const std::vector<const Complex*> coefficients, 	\
		t_quadratic_solutions &solutions, 					\
		const std::vector<Rational> sqrt_factors, Rational* denominator)
{
	Rational*	real_term1;
	Rational*	real_term2_factor;
	Rational*	real_term3_factor;
	Rational*	real_gcd;

	real_term1 = get_initial_real_term1(coefficients[0], coefficients[1]);
	real_term2_factor = dynamic_cast<Rational*>(*coefficients[0]->getReal() * sqrt_factors[0]);
	real_term3_factor = dynamic_cast<Rational*>(*coefficients[0]->getImaginary() * sqrt_factors[1]);
	real_gcd = get_gcd_terms(real_term1, real_term2_factor, 				\
			real_term3_factor, denominator);
	update_real_term1(solutions, real_term1, real_gcd);
	update_real_factors(solutions, real_term2_factor, real_term3_factor, 	\
			real_gcd);
	update_real_denominator(solutions, denominator, real_gcd);
	delete real_term1;
	delete real_term2_factor;
	delete real_term3_factor;
	delete real_gcd;
	set_imaginary_terms(coefficients, solutions, sqrt_factors, denominator);
}
