/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_solutions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:19:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/11 17:58:23 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	print_exact_solutions(t_quadratic_solutions &solutions, 	\
		std::string set, bool reduce_sqrt = false)
{
	bool	has_imaginary_part;

	std::cout << "S = {";// TODO: Fusionner les 2 solutions si elles sont égales
	print_real_part(solutions, 0, reduce_sqrt);
	has_imaginary_part = print_imaginary_part(solutions, 0, reduce_sqrt);
	std::cout << ", ";
	print_real_part(solutions, 1, reduce_sqrt);
	has_imaginary_part |= print_imaginary_part(solutions, 1, reduce_sqrt);
	std::cout << "} ∈ ";
	if (has_imaginary_part)
		std::cout << "ℂ" << std::endl;
	else
		std::cout << set << std::endl;
}

bool		parentheses_needed(const int term1, const int term2, const int term3)
{
	int	count_terms;
	int	term;

	count_terms = 0;
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

void		print_solutions(t_quadratic_solutions& solutions, 	\
		std::string set, std::string var)
{
	bool	reduce_sqrt;

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
	std::cout << COLOR_GREEN;
	print_exact_solutions(solutions, set, reduce_sqrt);
	std::cout << COLOR_RESET;
	print_rounded_solutions(solutions, reduce_sqrt, var);
}
