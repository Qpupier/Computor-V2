/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_solutions_structure.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 15:46:39 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/11 11:02:44 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static Rational*		get_initial_denominator(const Complex* a)
{
	Rational*	module_square;
	Rational*	result;

	module_square = get_module_square(a);
	result = Rational(2) * *module_square;
	delete module_square;
	return (result);
}

static inline void		solutions_structure_init(	\
		t_quadratic_solutions& solutions)
{
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
		.sqrt_imaginary = 0
	};
}

Rational*				get_gcd_terms(const Rational* term1, 	\
		const Rational* term2, const Rational* term3, const Rational* term4)
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

t_quadratic_solutions	get_solutions_structure(const Complex* a, 	\
		const Complex* b, const Complex* delta)
{
	t_quadratic_solutions	solutions;
	Rational				sqrt_real_factor;
	Rational*				sqrt_imaginary_factor;
	Rational*				denominator;

	solutions_structure_init(solutions);
	sqrt_real_factor = factorize_sqrt(solutions, delta);
	sqrt_imaginary_factor = sqrt_real_factor 								\
			* (delta->getImaginary() < 0 ? -1 : 1);
	denominator = get_initial_denominator(a);
	std::vector<Rational> sqrt_factors({sqrt_real_factor, *sqrt_imaginary_factor});
	set_real_terms(std::vector<const Complex*>({a, b}), solutions, 			\
			std::vector<Rational>											\
				({sqrt_real_factor, *sqrt_imaginary_factor}), 				\
			denominator);// TODO: Doublon variables vectors
	set_imaginary_terms(std::vector<const Complex*>({a, b}), solutions, 	\
			std::vector<Rational>											\
				({sqrt_real_factor, *sqrt_imaginary_factor}), 				\
			denominator);
	delete sqrt_imaginary_factor;
	delete denominator;
	return (solutions);
}
