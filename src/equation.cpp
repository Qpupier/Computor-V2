/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:32 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/16 16:52:14 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

std::string			to_lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){return (static_cast<char>(std::tolower(c)));});
	return (s);
}

static void			equation_error(AST *left_ast, AST *right_ast, 	\
		const UnexpectedError &error)
{
	if (left_ast)
		delete left_ast;
	if (right_ast)
		delete right_ast;
	throw error;
}

static void			assignation(std::string var, std::map<std::pair<std::string, std::string>, const IType*> &stored, IType *result)
{
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	it;
	std::string	var_lower(to_lower(var));
	std::pair<std::string, std::string>	pair(var_lower, std::string());

	std::cout << var << " = " << *result << std::endl;
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

static void			trinomial(Polynomial *polynomial)
{
	std::cout << *polynomial << " = 0" << std::endl;
	solve_trinomial(polynomial);
}

static void			binomial(Polynomial *polynomial, 				\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	IType*							tmp;
	IType*							result;

	tmp = *terms[0].coefficient * Rational(-1);
	result = *tmp / *terms[1].coefficient;
	delete tmp;
	assignation(polynomial->getName(), stored, result);
}

static void			monomial(Polynomial *polynomial)
{
	if (polynomial->getTerms().empty())
		std::cout << "True" << std::endl;
	else
		std::cout << "False" << std::endl;
}

static void			display_result(Polynomial *polynomial, std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	unsigned long int	degree;

	// std::cout << "Reduced form: " << *polynomial << " = 0" << std::endl; // TODO: To remove
	degree = polynomial->getTerms().size() - 1;
	if (polynomial->getTerms().empty() || degree == 0)
		monomial(polynomial);
	else if (degree == 1)
		binomial(polynomial, stored);
	else if (degree == 2)
		trinomial(polynomial);
	else
		std::cout << "The polynomial degree is stricly greater than 2, I can't solve." << std::endl;
}

void				equation(AST *left_ast, AST *right_ast, 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	IType*		left;
	IType*		right;
	IType*		all_left;
	Polynomial*	polynomial;

	if (!left_ast)
		return ;
	if (!left_ast->end_of_tree() || !right_ast->end_of_tree())
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: not an expression"));
	left = left_ast->getNode()->clone();
	right = right_ast->getNode()->clone();
	if (!left || !right)
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: null node"));
	all_left = *left - *right;
	delete left;
	delete right;
	polynomial = new Polynomial(*all_left);
	delete all_left;
	display_result(polynomial, stored);
	delete polynomial;
}
