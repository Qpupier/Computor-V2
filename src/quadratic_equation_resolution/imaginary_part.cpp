/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imaginary_part.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:24:35 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/29 12:13:34 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static Rational*	get_initial_imaginary_term1(const Complex* a, 	\
		const Complex* b)
{
	Rational*	a_imaginary_b_real;
	Rational*	a_real_b_imaginary;
	Rational*	result;

	a_imaginary_b_real = dynamic_cast<Rational*>(*a->getImaginary() * *b->getReal());// [ ] Changer ca
	a_real_b_imaginary = dynamic_cast<Rational*>(*a->getReal() * *b->getImaginary());
	result = dynamic_cast<Rational*>(*a_imaginary_b_real - *a_real_b_imaginary);
	delete a_imaginary_b_real;
	delete a_real_b_imaginary;
	return (result);
}

static void			update_imaginary_term1(									\
		t_quadratic_solutions &solutions, const Rational* imaginary_term1, 	\
		const Rational* imaginary_gcd)
{
	Rational*	tmp;

	tmp = *imaginary_term1 / *imaginary_gcd;
	solutions.imaginary_term1[0] = tmp->getNumerator();
	solutions.imaginary_term1[1] = tmp->getNumerator();
	delete tmp;
}

static void			update_imaginary_factors(		\
		t_quadratic_solutions &solutions, 			\
		const Rational* imaginary_term2_factor, 	\
		const Rational* imaginary_term3_factor, const Rational* imaginary_gcd)
{
	Rational*	tmp;

	tmp = *imaginary_term2_factor / *imaginary_gcd;
	solutions.imaginary_term2_factor[0] = tmp->getNumerator();
	solutions.imaginary_term2_factor[1] = -tmp->getNumerator();
	delete tmp;
	tmp = *imaginary_term3_factor / *imaginary_gcd;
	solutions.imaginary_term3_factor[0] = -tmp->getNumerator();
	solutions.imaginary_term3_factor[1] = tmp->getNumerator();
	delete tmp;
}

static void			update_imaginary_denominator(	\
		t_quadratic_solutions &solutions, 			\
		const Rational* imaginary_denominator, const Rational* imaginary_gcd)
{
	Rational*	tmp;

	tmp = *imaginary_denominator / *imaginary_gcd;
	solutions.imaginary_denominator[0] = tmp->getNumerator();
	solutions.imaginary_denominator[1] = tmp->getNumerator();
	delete tmp;
}

void				set_imaginary_terms(					\
		const std::vector<const Complex*> coefficients, 	\
		t_quadratic_solutions &solutions, 					\
		const std::vector<Rational> sqrt_factors, Rational* denominator)
{
	Rational*	imaginary_term1;
	Rational*	imaginary_term2_factor;
	Rational*	imaginary_term3_factor;
	Rational*	imaginary_gcd;

	imaginary_term1 = get_initial_imaginary_term1(coefficients[0], 			\
			coefficients[1]);
	imaginary_term2_factor = dynamic_cast<Rational*>(*coefficients[0]->getImaginary() * sqrt_factors[0]);// [ ] Changer ca
	imaginary_term3_factor = dynamic_cast<Rational*>(*coefficients[0]->getReal() * sqrt_factors[1]);
	imaginary_gcd = get_gcd_terms(imaginary_term1, imaginary_term2_factor, 	\
			imaginary_term3_factor, denominator);
	update_imaginary_term1(solutions, imaginary_term1, imaginary_gcd);
	update_imaginary_factors(solutions, imaginary_term2_factor, 			\
		imaginary_term3_factor, imaginary_gcd);
	update_imaginary_denominator(solutions, denominator, imaginary_gcd);
	delete imaginary_term1;
	delete imaginary_term2_factor;
	delete imaginary_term3_factor;
	delete imaginary_gcd;
}
