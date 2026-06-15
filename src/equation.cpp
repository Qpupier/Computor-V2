/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:32 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 13:19:58 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "quadratic.hpp"

static void	equation_error(AST *left_ast, AST *right_ast, 	\
		const UnexpectedError &error)
{
	if (left_ast)
		delete left_ast;
	if (right_ast)
		delete right_ast;
	throw error;
}

static void	equation(AST *left_ast, AST *right_ast, t_data &data)
{
	IType*		left;
	IType*		right;
	IType*		all_left;
	Polynomial*	polynomial;

	if (!left_ast || !left_ast->end_of_tree() || !right_ast 	\
			|| !right_ast->end_of_tree())
		equation_error(left_ast, right_ast, 					\
				UnexpectedError("Invalid AST: not an expression"));
	left = left_ast->getNode()->clone();
	right = right_ast->getNode()->clone();
	if (!left || !right)
		equation_error(left_ast, right_ast, 					\
				UnexpectedError("Invalid AST: null node"));
	all_left = *left - *right;
	delete left;
	delete right;
	polynomial = new Polynomial(*all_left);
	delete all_left;
	display_result(polynomial, data);
	delete polynomial;
}

void		compute_equation(const std::string &line, t_data &data, 	\
		const bool eval)
{
	std::size_t	pos;
	AST*		left_ast;
	AST*		right_ast;

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
		equation(left_ast, right_ast, data);
	delete left_ast;
	delete right_ast;
}
