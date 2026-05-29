/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factorize_sqrt.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 13:28:11 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/29 13:32:11 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void			sqrt_reduce_terms(t_quadratic_solutions &solutions, 	\
		const Rational term_1_factor, const Rational* term_2, 				\
		const Rational* small_gcd)
{
	Rational* tmp;

	tmp = term_1_factor / *small_gcd;
	solutions.sqrt_term1_factor = tmp->getNumerator();
	delete tmp;
	tmp = *term_2 / *small_gcd;
	solutions.sqrt_term2_real = tmp->getNumerator();
	solutions.sqrt_term2_imaginary = -tmp->getNumerator();
	delete tmp;
}

static Rational*	get_small_gcd(t_quadratic_solutions &solutions, 	\
		const Complex* delta, const Rational& term_1_factor)
{
	Rational*	small_gcd;
	Rational*	term_2;

	term_2 = delta->getReal() * 2;
	small_gcd = term_1_factor.gcd(*term_2);
	sqrt_reduce_terms(solutions, term_1_factor, term_2, small_gcd);
	delete term_2;
	return (small_gcd);
}

static Rational		factorize_sqrt_reduce_gcd(	\
		t_quadratic_solutions &solutions, const Rational* small_gcd)
{
	Rational	final_factor;
	InfiniteInt	factor(1);
	InfiniteInt	big_sqrt(small_gcd->getNumerator() 	\
			* small_gcd->getDenominator());

	reduce_sqrt(&factor, &big_sqrt);
	solutions.sqrt_term1_factor *= big_sqrt;
	solutions.sqrt_term2_real *= big_sqrt;
	solutions.sqrt_term2_imaginary *= big_sqrt;
	final_factor = Rational(factor, small_gcd->getDenominator() * 2);
	delete small_gcd;
	return (final_factor);
}

Rational*			get_module_square(const Complex* complex)
{
	Rational*	real_square;
	Rational*	imaginary_square;
	Rational*	module_square;

	real_square = complex->getReal() * complex->getReal();
	imaginary_square = complex->getImaginary() * complex->getImaginary();
	module_square = *real_square + *imaginary_square;
	delete real_square;
	delete imaginary_square;
	return (module_square);
}

Rational			factorize_sqrt(t_quadratic_solutions &solutions, 	\
		const Complex* delta)
{
	Rational*	module_square;
	Rational*	small_gcd;
	Rational	term_1_factor;

	module_square = get_module_square(delta);
	solutions.sqrt_term1_factor = 2;
	solutions.sqrt_term1_sqrt = module_square->getNumerator() 	\
			* module_square->getDenominator();
	reduce_sqrt(&solutions.sqrt_term1_factor, &solutions.sqrt_term1_sqrt);
	term_1_factor = Rational(solutions.sqrt_term1_factor, 		\
			module_square->getDenominator());
	delete module_square;
	small_gcd = get_small_gcd(solutions, delta, term_1_factor);
	return (factorize_sqrt_reduce_gcd(solutions, small_gcd));
}
