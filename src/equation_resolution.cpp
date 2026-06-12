/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation_resolution.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:12:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/12 14:16:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "quadratic.hpp"

static void		trinomial(const Polynomial *polynomial, 	\
		std::map<std::pair<std::string, std::string>, const IType*>& stored)
{
	std::cout << COLOR_BOLD << *polynomial << " = 0" << COLOR_RESET 	\
			<< std::endl;
	solve_trinomial(polynomial, stored);
}

static void		binomial(const Polynomial *polynomial, 				\
		std::map<std::pair<std::string, std::string>, const IType*>& stored)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	IType*							tmp;
	IType*							result;

	tmp = -(*terms[0].coefficient);
	result = *tmp / *terms[1].coefficient;
	delete tmp;
	assignation(polynomial->getName(), stored, result);
}

static void		monomial(const Polynomial *polynomial)
{
	if (polynomial->getTerms().empty())
		std::cout << COLOR_BOLD << "True" << COLOR_RESET << std::endl;
	else
		std::cout << COLOR_BOLD << "False" << COLOR_RESET << std::endl;
}

void			assignation(std::string var, 							\
		std::map<std::pair<std::string, std::string>, const IType*>& 	\
			stored, 													\
		IType *result)
{
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	it;
	std::string							\
			var_lower(to_lower(var));
	std::pair<std::string, std::string>	\
			pair(var_lower, std::string());

	std::cout << COLOR_BOLD << var << " = " << *result << COLOR_RESET << std::endl;
	result->print_rounded(var);
	for (it = stored.begin(); it != stored.end();)
	{
		if (it->first.first == var_lower)
		{
			delete it->second;
			it = stored.erase(it);
		}
		else
			it++;
	}
	stored[pair] = result;
}

void			display_result(const Polynomial *polynomial, 	\
		std::map<std::pair<std::string, std::string>, const IType*>& stored)
{
	unsigned long int	degree;

	degree = polynomial->getTerms().size() - 1;
	if (polynomial->getTerms().empty() || degree == 0)
		monomial(polynomial);
	else if (degree == 1)
		binomial(polynomial, stored);
	else if (degree == 2)
		trinomial(polynomial, stored);
	else
		std::cout << COLOR_BOLD << "The polynomial degree is stricly \
			greater than 2, I can't solve." << COLOR_RESET << std::endl;
}
