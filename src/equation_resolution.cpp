/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation_resolution.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:12:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/07 15:42:13 by qpupier          ###   ########lyon.fr   */
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
	data.history_results.push_back(std::string(COLOR_GREEN) 		\
			+ polynomial->getName() + " = " + result->to_string() 	\
			+ std::string(COLOR_RESET));
	assignation(polynomial->getName(), data.stored, result);
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
	history_results.push_back(std::string(COLOR_GREEN) + result 	\
			+ std::string(COLOR_RESET));
}

void		assignation(std::string var, 				\
		std::map<std::pair<std::string, std::string>, 	\
			const IType*> &stored, 						\
		IType *result)
{
	std::pair<std::string, std::string>	pair(var, std::string());
	std::string							var_lower(to_lower(var));

	std::cout << COLOR_BOLD << var << " = " << *result << COLOR_RESET 	\
			<< std::endl;
	if (!result->in_Q())
	{
		std::cout << COLOR_BOLD << COLOR_DIM 							\
				<< "Real numbers cannot be stored as variables" 		\
				<< COLOR_RESET << std::endl;
		return ;
	}
	stop_preset_terms(var);
	result->print_rounded(var);
	for (std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator it(stored.begin()); it != stored.end();)
		if (to_lower(it->first.first) == var_lower)
		{
			delete it->second;
			it = stored.erase(it);
		}
		else
			it++;
	stored[pair] = result;
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
		throw UnsupportedError("The polynomial degree is strictly greater than \
				2, I can't solve.");
}
