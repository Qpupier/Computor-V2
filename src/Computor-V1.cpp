/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 18:55:09 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"

// static void	find_complex_solutions(Polynomial *polynomial, Rational *discriminant)
// {
// 	(void)polynomial;
// 	(void)discriminant;
// 	std::cout << "The discriminant is strictly negative, the two solutions are complex conjugates." << std::endl;
// 	// To adjust
// }

// static void	find_one_solution(Polynomial *polynomial)
// {
// 	std::cout << "The discriminant is null, the solution is:" << std::endl;
// 	// To adjust
// }

// static void	find_two_solutions(Polynomial *polynomial, Rational *discriminant)
// {
// 	std::cout << "The discriminant is strictly positive, the two solutions are:" << std::endl;
// 	// To adjust
// }

// static void	solve_trinomial_rational(Polynomial *polynomial, Rational *discriminant)
// {
// 	if (*discriminant < Rational(0))
// 		find_complex_solutions(polynomial, discriminant);
// 	else if (!*discriminant)
// 		find_one_solution(polynomial);
// 	else
// 		find_two_solutions(polynomial, discriminant);
// 	delete discriminant;
// }

void	solve_trinomial(Polynomial *polynomial)
{
	IType		*tmp_b2;
	IType		*tmp_4a;
	IType		*tmp_4ac;
	IType		*tmp_b2_4ac;
	// Rational	*discriminant_rational;
	// Complex		*discriminant_complex;
	// Matrix		*discriminant_matrix;

	tmp_b2 = *polynomial->getPower1() * *polynomial->getPower1();
	tmp_4a = Rational(4) * *polynomial->getPower2();
	tmp_4ac = *tmp_4a * *polynomial->getPower0();
	tmp_b2_4ac = *tmp_b2 - *tmp_4ac;
	delete tmp_b2;
	delete tmp_4a;
	delete tmp_4ac;
	// discriminant_rational = dynamic_cast<Rational*>(tmp_b2_4ac);
	// if (discriminant_rational)
	// 	return solve_trinomial_rational(polynomial, discriminant_rational);
	// discriminant_complex = dynamic_cast<Complex*>(tmp_b2_4ac);
	// if (discriminant_complex)
	// 	return solve_trinomial_complex(polynomial, discriminant_complex);
	// discriminant_matrix = dynamic_cast<Matrix*>(tmp_b2_4ac);
	// if (discriminant_matrix)
	// 	return solve_trinomial_matrix(polynomial, discriminant_matrix);
	// delete tmp_b2_4ac;
	// throw UnexpectedError("Unexpected discriminant type");// To adjust
}
