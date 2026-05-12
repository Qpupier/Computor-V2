/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratic.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:43:37 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/12 18:30:11 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUADRATIC_HPP
# define QUADRATIC_HPP

# include "Complex.hpp"

typedef struct	s_quadratic_solutions
{
	int	real_term1[2];				// A
	int	real_term2_factor[2];		// B
	int	real_term3_factor[2];		// C
	int	real_denominator[2];		// D
	int	imaginary_term1[2];			// E
	int	imaginary_term2_factor[2];	// F
	int	imaginary_term3_factor[2];	// G
	int	imaginary_denominator[2];	// H
	int	sqrt_term1_factor;			// I
	int	sqrt_term1_sqrt;			// J
	int	sqrt_term2_real;			// K
	int	sqrt_term2_imaginary;		// -K
	int	sqrt_real;					// I√J + K
	int	sqrt_imaginary;				// I√J - K
	// Representation:
	//
	//    (A - B√(I√J + K) - C√(I√J - K)) / D
	// + i(E + F√(I√J + K) - G√(I√J - K)) / H
	//
	// or
	//
	//    (A + B√(I√J + K) + C√(I√J - K)) / D
	// + i(E - F√(I√J + K) + G√(I√J - K)) / H
}				t_quadratic_solutions;

Rational*				get_gcd_terms(Rational* term1, Rational* term2, 	\
		Rational* term3, Rational* term4);
t_quadratic_solutions	get_solutions_structure(Complex* a, Complex* b, 	\
		Complex* delta);
std::string				get_sqrt_imaginary(t_quadratic_solutions &solutions);
std::string				get_sqrt_real(t_quadratic_solutions &solutions);
bool					parentheses_needed(const int term1, 				\
		const int term2, const int term3);
bool					print_imaginary_part(								\
		t_quadratic_solutions &solutions, int index, bool reduce_sqrt);
void					print_real_part(									\
		t_quadratic_solutions &solutions, int index, bool reduce_sqrt);
void					print_rounded_solutions(							\
		t_quadratic_solutions &solutions, bool reduce_sqrt, 				\
		std::string var, int nb_solutions);
void					print_solutions(t_quadratic_solutions& solutions, 	\
		std::string set, std::string var);
void					reduce_sqrt(int *factor, int *sqrt);
void					set_imaginary_terms(Complex* a, Complex* b, 		\
		t_quadratic_solutions &solutions, Rational sqrt_real_factor, 		\
		Rational* sqrt_imaginary_factor, Rational* denominator);
void					set_real_terms(Complex* a, Complex* b, 				\
		t_quadratic_solutions &solutions, Rational sqrt_real_factor, 		\
		Rational* sqrt_imaginary_factor, Rational* denominator);
void					simplify_deepest_sqrt(t_quadratic_solutions &solutions);
void					simplify_factors(t_quadratic_solutions &s);
void					simplify_factors2(t_quadratic_solutions &solutions);

#endif
