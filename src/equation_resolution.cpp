/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation_resolution.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:12:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 18:02:29 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "quadratic.hpp"

static void	trinomial(const Polynomial *polynomial, t_data &data)
{
	std::cout << COLOR_BOLD << *polynomial << " = 0" << COLOR_RESET 	\
			<< std::endl;
	solve_trinomial(polynomial, data);
}

static void	binomial(const Polynomial *polynomial, t_data &data)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	IType*							tmp;
	IType*							result;

	tmp = -(*terms[0].coefficient);
	result = *tmp / *terms[1].coefficient;
	delete tmp;
	assignation(polynomial->getName(), data.stored, result);
	add_history_result(data.history_results, polynomial->getName() 	\
			+ " = " + result->to_string());
}

static void	monomial(const Polynomial *polynomial, 	\
		std::vector<std::string> &history_results)
{
	std::string	result;

	if (!*polynomial)
		result = "True";
	else
		result = "False";
	std::cout << COLOR_BOLD << result << COLOR_RESET << std::endl;
	add_history_result(history_results, result);
}

void		display_result(const Polynomial *polynomial, t_data &data)
{
	unsigned long int	degree;

	degree = polynomial->getTerms().size() - 1;
	if (polynomial->getTerms().empty() || degree == 0)
		monomial(polynomial, data.history_results);
	else if (degree == 1)
		binomial(polynomial, data);
	else if (degree == 2)
		trinomial(polynomial, data);
	else
		throw UnsupportedError("The polynomial degree is strictly greater"	\
				" than 2, I can't solve.");
}
