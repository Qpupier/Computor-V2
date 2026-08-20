/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_preset_terms.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 16:57:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/20 18:08:35 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"
#include "Operator.hpp"

void	stop_preset_terms(const std::string& name, bool change)
{
	std::string	message;

	message = std::string("\"") + name 	\
			+ std::string("\" is a pre-set variable or function");
	if (change)
		message += std::string(" and cannot be changed");
	if (std::regex_match(name, std::regex(TOKEN_NO_VARIABLE)))
		throw LogicError(message);
}

void	verif_preset_terms(AST* ast, bool left, bool last_operator_is_function)
{
	Operator*	op;
	Polynomial*	polynomial;
	bool		operator_is_function;

	if (!ast)
		return ;
	polynomial = dynamic_cast<Polynomial*>(ast->getNode());
	if (polynomial && !(left && last_operator_is_function))
		stop_preset_terms(polynomial->getName(), false);
	op = dynamic_cast<Operator*>(ast->getNode());
	operator_is_function = op && op->getOperator() == Operator::E_FUNCTION;
	verif_preset_terms(ast->getLeft(), true, operator_is_function);
	verif_preset_terms(ast->getRight(), false, operator_is_function);
}
