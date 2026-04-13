/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:32 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/13 16:37:55 by qpupier          ###   ########lyon.fr   */
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

static Polynomial	*get_polynomial(IType *left, IType *right)
{
	Polynomial*						left_var;
	Polynomial*						right_var;
	std::string						var;
	Polynomial*						polynomial;
	std::vector<Polynomial::t_term>	dividers;

	left_var = dynamic_cast<Polynomial*>(left);
	right_var = dynamic_cast<Polynomial*>(right);
	if (!left_var && !right_var)
		var = "";
	else if (left_var)
		var = left_var->getName();
	else
		var = right_var->getName();
	if (!left_var)
		left_var = new Polynomial(var, (Polynomial::t_term){left->clone(), 0});
	if (!right_var)
		right_var = new Polynomial(var, (Polynomial::t_term){right->clone(), 0});
	polynomial = *left_var - *right_var;
	delete left_var;
	delete right_var;
	dividers = polynomial->getDividers();
	for (std::vector<Polynomial::t_term>::const_iterator it = dividers.begin(); it != dividers.end(); it++)
		delete it->coefficient;
	polynomial->getDividers().clear();
	polynomial->getDividers().push_back((Polynomial::t_term){new Rational(1), 0});
	return (polynomial);
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

static void			binomial(Polynomial *polynomial, 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	IType*		power0;
	IType*		power1;
	IType*		tmp;
	IType*		result;

	if (polynomial->getDividers().size() != 1 							\
			|| *polynomial->getDividers()[0].coefficient != Rational(1) 	\
			|| polynomial->getDividers()[0].power)
		throw ERROR_UNEXPECTED;
	power0 = nullptr;
	power1 = nullptr;
	for (std::vector<Polynomial::t_term>::const_iterator it = terms.begin(); it != terms.end(); it++)
	{
		if (!it->power)
			power0 = it->coefficient;
		else if (it->power == 1)
			power1 = it->coefficient;
		else
			throw ERROR_UNEXPECTED;
	}
	if (!power0)
		power0 = new Rational(0);
	if (!power1)
		power1 = new Rational(0);
	tmp = *power0 * Rational(-1);
	result = *tmp / *power1;
	delete tmp;
	assignation(polynomial->getName(), stored, result);
}

static void			monomial(Polynomial *polynomial)
{
	if (polynomial->getTerms().size() != 1 									\
			|| polynomial->getTerms()[0].power 								\
			|| polynomial->getDividers().size() != 1 						\
			|| *polynomial->getDividers()[0].coefficient != Rational(1) 	\
			|| polynomial->getDividers()[0].power)
		throw ERROR_UNEXPECTED;
	if (*polynomial->getTerms()[0].coefficient == Rational(0))
		std::cout << "True" << std::endl;
	else
		std::cout << "False" << std::endl;
}

void				equation(AST *left_ast, AST *right_ast, 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	IType			*left;
	IType			*right;
	Polynomial		*polynomial;
	unsigned int	degree;

	if (!left_ast)
		return ;
	if (!left_ast->end_of_tree() || !right_ast->end_of_tree())
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: not an expression"));
	left = left_ast->getNode()->clone();
	right = right_ast->getNode()->clone();
	if (!left || !right)
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: null node"));
	polynomial = get_polynomial(left, right);
	degree = polynomial->get_degree();
	if (degree > 2)
		std::cout << "The polynomial degree is stricly greater than 2, I can't solve." << std::endl;
	else if (degree == 2)
		trinomial(polynomial);
	else if (degree == 1)
		binomial(polynomial, stored);
	else if (degree == 0)
		monomial(polynomial);
	delete polynomial;
}
