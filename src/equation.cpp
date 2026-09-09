/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:32 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/09 12:18:20 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "quadratic.hpp"
#include "Boolean.hpp"

static void	equation_error(AST *left_ast, AST *right_ast, 					\
		const std::exception& error, bool throw_error = true)
{
	if (left_ast)
		delete left_ast;
	if (right_ast)
		delete right_ast;
	if (throw_error)
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

static void	handle_equation(AST* left_ast, AST* right_ast, t_data& data, 	\
		const bool eval)
{
	Boolean*	tmp;

	if (left_ast)
		equation(left_ast, right_ast, data);
	else if (eval)
	{
		std::cout << COLOR_BOLD << "False" << COLOR_RESET << std::endl;
		tmp = new Boolean(false);
		assign_last(data, tmp);
		delete tmp;
		add_history_result(data.history_results, "False");
	}
	delete left_ast;
	delete right_ast;
}

void		compute_equation(const std::string &line, t_data &data, 		\
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
	catch (...)
	{
		equation_error(left_ast, right_ast, std::exception(), false);
		throw;
	}
	handle_equation(left_ast, right_ast, data, eval);
}
