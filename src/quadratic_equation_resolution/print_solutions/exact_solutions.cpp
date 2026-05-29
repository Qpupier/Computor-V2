/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:19:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/29 10:49:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static inline bool	solutions_equal(const t_quadratic_solutions& 	\
		solutions)
{
	return (solutions.real_term1[0] == solutions.real_term1[1] 	\
			&& solutions.real_term2_factor[0] 					\
				== solutions.real_term2_factor[1] 				\
			&& solutions.real_term3_factor[0] 					\
				== solutions.real_term3_factor[1] 				\
			&& solutions.real_denominator[0] 					\
				== solutions.real_denominator[1] 				\
			&& solutions.imaginary_term1[0] 					\
				== solutions.imaginary_term1[1] 				\
			&& solutions.imaginary_term2_factor[0] 				\
				== solutions.imaginary_term2_factor[1] 			\
			&& solutions.imaginary_term3_factor[0] 				\
				== solutions.imaginary_term3_factor[1] 			\
			&& solutions.imaginary_denominator[0] 				\
				== solutions.imaginary_denominator[1]);
}

static void			print_exact_solution(const t_quadratic_solutions& 	\
		solutions, const unsigned char i, const bool reduce_sqrt)
{
	bool	has_real_part;
	bool	has_imaginary_part;

	if (i)
		std::cout << ", ";
	has_real_part = solutions.real_term1[i] 			\
			|| solutions.real_term2_factor[i] 			\
			|| solutions.real_term3_factor[i];
	has_imaginary_part = solutions.imaginary_term1[i] 	\
			|| solutions.imaginary_term2_factor[i] 		\
			|| solutions.imaginary_term3_factor[i];
	if (has_real_part || !has_imaginary_part)
		print_real_part(solutions, i, reduce_sqrt);
	if (has_imaginary_part)
	{
		if (has_real_part)
			std::cout << " + ";
		print_imaginary_part(solutions, i, reduce_sqrt);
	}
}

static void			print_exact_solutions(const t_quadratic_solutions& 	\
		solutions, const std::string set, const bool reduce_sqrt, 		\
		const unsigned char nb_solutions)
{
	std::cout << COLOR_GREEN;
	if (nb_solutions == 1)
		std::cout << "One solution in ";
	else
		std::cout << "Two solutions in ";
	std::cout << set << ":" << std::endl << "S = {";
	for (unsigned char i = 0; i < nb_solutions; i++)
	{
		print_exact_solution(solutions, i, reduce_sqrt);
		if (nb_solutions == 1)
			break;
	}
	std::cout << "} ∈ " << set << COLOR_RESET << std::endl;
}

bool				parentheses_needed(const InfiniteInt term1, 	\
		const InfiniteInt term2, const InfiniteInt term3)
{
	InfiniteInt	count_terms(0);
	InfiniteInt	term;

	if (term1)
	{
		count_terms++;
		term = term1;
	}
	if (term2)
	{
		count_terms++;
		term = term2;
	}
	if (term3)
	{
		count_terms++;
		term = term3;
	}
	return (count_terms > 1 || (count_terms == 1 && term < 0));
}

void				print_solutions(t_quadratic_solutions& solutions, 	\
		const std::string set, const std::string var, 					\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	unsigned char	nb_solutions;
	bool			reduce_sqrt;

	if (!solutions.sqrt_term1_factor || solutions.sqrt_term1_sqrt <= 1)
	{
		simplify_deepest_sqrt(solutions);
		simplify_factors(solutions);
		reduce_sqrt = true;
	}
	else
	{
		simplify_factors2(solutions);
		reduce_sqrt = false;
	}
	nb_solutions = solutions_equal(solutions) ? 1 : 2;
	print_exact_solutions(solutions, set, reduce_sqrt, nb_solutions);
	if (nb_solutions == 1)
		assignation(var, stored, new Complex(			\
				Rational(solutions.real_term1[0], 		\
					solutions.real_denominator[0]), 	\
				Rational(solutions.imaginary_term1[0], 	\
					solutions.imaginary_denominator[0])));
	print_rounded_solutions(solutions, reduce_sqrt, var, nb_solutions);
}
