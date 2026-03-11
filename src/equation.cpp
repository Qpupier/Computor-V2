/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:32 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 20:01:07 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

static void	equation_error(AST *left_ast, AST *right_ast, 	\
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
	Polynomial	*left_var;
	Polynomial	*right_var;
	std::string	var;
	Polynomial	*polynomial;

	left_var = dynamic_cast<Polynomial*>(left);
	right_var = dynamic_cast<Polynomial*>(right);
	if (!left_var && !right_var)
		var = "";
	else if (left_var)
		var = left_var->getName();
	else
		var = right_var->getName();
	if (!left_var)
		left_var = new Polynomial(var, new Rational(0), new Rational(0), left);
	if (!right_var)
		right_var = new Polynomial(var, new Rational(0), new Rational(0), right);
	polynomial = *left_var - *right_var;
	delete left_var;
	delete right_var;
	return (polynomial);
}

static void	trinomial(Polynomial *polynomial)
{
	std::cout << *polynomial << " = 0" << std::endl;
	solve_trinomial(polynomial);
}

static void	binomial(Polynomial *polynomial, std::map<std::string, const IType*> &stored)
{
	IType	*tmp;
	IType	*result;

	tmp = *polynomial->getPower0() * Rational(-1);
	result = *tmp / *polynomial->getPower1();
	delete tmp;
	std::cout << polynomial->getName() << " = " << *result << std::endl;
	stored[polynomial->getName()] = result;
}

void	equation(AST *left_ast, AST *right_ast, std::map<std::string, const IType*> &stored)
{
	IType		*left;
	IType		*right;
	Polynomial	*polynomial;

	if (!left_ast || !right_ast)
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: null pointer"));
	if (!left_ast->end_of_tree() || !right_ast->end_of_tree())
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: not an expression"));
	left = left_ast->getNode()->clone();
	right = right_ast->getNode()->clone();
	if (!left || !right)
		equation_error(left_ast, right_ast, UnexpectedError("Invalid AST: null node"));
	polynomial = get_polynomial(left, right);
	if (*polynomial->getPower2())
		trinomial(polynomial);
	else if (*polynomial->getPower1())
		binomial(polynomial, stored);
	else if (*polynomial->getPower0())
		std::cout << *polynomial << " = 0" << std::endl << "No solution" << std::endl;// To adjust
	else
		std::cout << *polynomial << " = 0" << std::endl << "All real numbers are solutions" << std::endl;// To adjust
	delete polynomial;
}
