/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratic.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:43:37 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/07 14:44:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUADRATIC_HPP
# define QUADRATIC_HPP

# include "Complex.hpp"
# include "computor-v2.hpp"

typedef struct	s_quadratic_solutions
{
	InfiniteInt	real_term1[2];				// A
	InfiniteInt	real_term2_factor[2];		// B
	InfiniteInt	real_term3_factor[2];		// C
	InfiniteInt	real_denominator[2];		// D
	InfiniteInt	imaginary_term1[2];			// E
	InfiniteInt	imaginary_term2_factor[2];	// F
	InfiniteInt	imaginary_term3_factor[2];	// G
	InfiniteInt	imaginary_denominator[2];	// H
	InfiniteInt	sqrt_term1_factor;			// I
	InfiniteInt	sqrt_term1_sqrt;			// J
	InfiniteInt	sqrt_term2_real;			// K
	InfiniteInt	sqrt_term2_imaginary;		// -K
	InfiniteInt	sqrt_real;					// I√J + K
	InfiniteInt	sqrt_imaginary;				// I√J - K
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

t_quadratic_solutions	get_solutions_structure(const Complex* a, const Complex* b, const Complex* delta);
Rational				factorize_sqrt(t_quadratic_solutions &solutions, const Complex* delta);
std::string				get_sqrt_imaginary(const t_quadratic_solutions &solutions);
std::string				get_sqrt_real(const t_quadratic_solutions &solutions);
std::string				print_imaginary_part(const t_quadratic_solutions &solutions, const unsigned char index, const bool reduce_sqrt);
std::string				print_real_part(const t_quadratic_solutions &solutions, const unsigned char index, const bool reduce_sqrt);
bool					parentheses_needed(const InfiniteInt term1, const InfiniteInt term2, const InfiniteInt term3);
std::vector<Complex*>	cast_coefficients_in_complex(const IType* tmp_a, const IType* tmp_b, const IType* tmp_c, const IType* tmp_delta);
Rational*				get_module_square(const Complex* complex);
void					display_result(const Polynomial *polynomial, t_data &data);
void					find_solutions(const std::vector<Complex*> &coefficients, std::string var, t_data &data);
void					print_rounded_solutions(const t_quadratic_solutions &solutions, const bool reduce_sqrt, const std::string var, const unsigned char nb_solutions);
void					print_solutions(t_quadratic_solutions& solutions, std::string& set, const std::string var, t_data &data);
void					reduce_sqrt(InfiniteInt *factor, InfiniteInt *sqrt);
void					set_imaginary_terms(const std::vector<const Complex*> coefficients, t_quadratic_solutions &solutions, const std::vector<Rational> sqrt_factors, Rational* denominator);
void					set_terms(const std::vector<const Complex*> coefficients, t_quadratic_solutions &solutions, const std::vector<Rational> sqrt_factors, Rational* denominator);
void					simplify_deepest_sqrt(t_quadratic_solutions &solutions);
void					simplify_factors(t_quadratic_solutions &s);
void					simplify_factors2(t_quadratic_solutions &solutions);
void					solve_trinomial(const Polynomial *polynomial, t_data &data);

#endif
