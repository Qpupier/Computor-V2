/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:32 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/15 12:21:16 by qpupier          ###   ########lyon.fr   */
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

void			assignation(std::string var, std::map<std::pair<std::string, std::string>, const IType*> &stored, IType *result)
{
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	it;
	std::string																var_lower(to_lower(var));
	std::pair<std::string, std::string>										pair(var_lower, std::string());

	std::cout << COLOR_BOLD << var << " = " << *result << COLOR_RESET << std::endl;
	result->print_variable(var);
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

static void			trinomial(const Polynomial *polynomial, std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::cout << COLOR_BOLD << *polynomial << " = 0" << COLOR_RESET << std::endl;
	solve_trinomial(polynomial, stored);
}

static void			binomial(const Polynomial *polynomial, 				\
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

static void			monomial(const Polynomial *polynomial)
{
	if (polynomial->getTerms().empty())
		std::cout << COLOR_BOLD << "True" << COLOR_RESET << std::endl;
	else
		std::cout << COLOR_BOLD << "False" << COLOR_RESET << std::endl;
}

static void			display_result(const Polynomial *polynomial, std::map<std::pair<std::string, std::string>, const IType*> &stored)
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
		std::cout << COLOR_BOLD << "The polynomial degree is stricly greater than 2, I can't solve." << COLOR_RESET << std::endl;
}

static void				equation(AST *left_ast, AST *right_ast, 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	IType*		left;
	IType*		right;
	IType*		all_left;
	Polynomial*	polynomial;

	if (!left_ast || !left_ast->end_of_tree() || !right_ast || !right_ast->end_of_tree())
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

void	compute_equation(const std::string &line, t_data &data, const bool eval)
{
	AST*		left_ast;
	AST*		right_ast;
	std::size_t	pos;

	pos = line.find('=');
	left_ast = compute_expression(line.substr(0, pos), data, false, eval);
	right_ast = nullptr;
	try
	{
		right_ast = compute_expression(line.substr(pos + 1), data, true, eval);
	}
	catch (const LogicError &e)
	{
		if (left_ast)
			delete left_ast;
		throw LogicError(e.what());
	}
	catch (const std::exception &e)
	{
		equation_error(left_ast, right_ast, UnexpectedError(e.what()));
	}
	if (left_ast)
		equation(left_ast, right_ast, data.stored);
	if (left_ast)
		delete left_ast;
	if (right_ast)
		delete right_ast;
}
