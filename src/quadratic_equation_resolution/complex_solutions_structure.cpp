/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_solutions_structure.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 15:46:39 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/11 15:58:40 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static Rational*	get_module_square(Complex* complex)
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

static void			sqrt_reduce_terms(t_quadratic_solutions &solutions, 	\
		Rational term_1_factor, Rational* term_2, Rational* small_gcd)
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

static Rational		factorize_sqrt(Complex* delta, 	\
		t_quadratic_solutions &solutions)
{
	Rational*	module_square;
	Rational*	small_gcd;
	Rational*	term_2;
	Rational	term_1_factor;
	Rational	final_factor;
	int			factor;
	int			big_sqrt;

	module_square = get_module_square(delta);
	solutions.sqrt_term1_factor = 2;
	solutions.sqrt_term1_sqrt = module_square->getNumerator() * module_square->getDenominator();
	reduce_sqrt(&solutions.sqrt_term1_factor, &solutions.sqrt_term1_sqrt);
	term_1_factor = Rational(solutions.sqrt_term1_factor, module_square->getDenominator());
	delete module_square;
	term_2 = Rational(2) * delta->getReal();
	small_gcd = term_1_factor.gcd(*term_2);
	sqrt_reduce_terms(solutions, term_1_factor, term_2, small_gcd);
	delete term_2;
	factor = 1;
	big_sqrt = small_gcd->getNumerator() * small_gcd->getDenominator();
	reduce_sqrt(&factor, &big_sqrt);
	solutions.sqrt_term1_factor *= big_sqrt;
	solutions.sqrt_term2_real *= big_sqrt;
	solutions.sqrt_term2_imaginary *= big_sqrt;
	final_factor = Rational(factor, 2 * small_gcd->getDenominator());
	delete small_gcd;
	return (final_factor);
}

static Rational*	get_initial_denominator(Complex* a)
{
	Rational*	module_square;
	Rational*	result;

	module_square = get_module_square(a);
	result = Rational(2) * *module_square;
	delete module_square;
	return (result);
}

Rational*	get_gcd_terms(Rational* term1, Rational* term2, 	\
		Rational* term3, Rational* term4)
{
	Rational*	tmp;
	Rational*	result;

	result = term1->gcd(*term2);
	tmp = result;
	result = tmp->gcd(*term3);
	delete tmp;
	tmp = result;
	result = tmp->gcd(*term4);
	delete tmp;
	return (result);
}

t_quadratic_solutions	get_solutions_structure(Complex* a, Complex* b, 	\
		Complex* delta)
{
	Rational*				denominator;
	Rational*				sqrt_imaginary_factor;
	Rational				sqrt_real_factor;
	t_quadratic_solutions	solutions;

	solutions = {
		.real_term1 = {0, 0},
		.real_term2_factor = {0, 0},
		.real_term3_factor = {0, 0},
		.real_denominator = {0, 0},
		.imaginary_term1 = {0, 0},
		.imaginary_term2_factor = {0, 0},
		.imaginary_term3_factor = {0, 0},
		.imaginary_denominator = {0, 0},
		.sqrt_term1_factor = 0,
		.sqrt_term1_sqrt = 0,
		.sqrt_term2_real = 0,
		.sqrt_term2_imaginary = 0,
		.sqrt_real = 0,
		.sqrt_imaginary = 0,
	};
	sqrt_real_factor = factorize_sqrt(delta, solutions);
	sqrt_imaginary_factor = sqrt_real_factor * Rational(delta->getImaginary() < Rational(0) ? -1 : 1);
	denominator = get_initial_denominator(a);
	set_real_terms(a, b, solutions, sqrt_real_factor, sqrt_imaginary_factor, denominator);
	set_imaginary_terms(a, b, solutions, sqrt_real_factor, sqrt_imaginary_factor, denominator);
	delete sqrt_imaginary_factor;
	delete denominator;
	return (solutions);
}
