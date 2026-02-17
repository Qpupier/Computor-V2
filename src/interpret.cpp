/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 19:11:26 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 19:33:39 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Node.hpp"

Node	*reduce_expression(Node *ast)
{
	// TODO
	if (ast->getTokens().size() > 1 || ast->getTokens()[0].get_type() != Token::E_OPERATOR)
	{
		return (ast);
	}
	if (ast->getTokens()[0].get_token() == "*")
	{
		Node *left = reduce_expression(ast->getLeft());
		Node *right = reduce_expression(ast->getRight());
		// multiply the underlying Node objects and return a newly allocated result
		return (*left) * (*right);
	}
	return (ast);
}
