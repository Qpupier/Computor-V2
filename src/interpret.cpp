/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 19:11:26 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 14:29:50 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Node.hpp"
#include "Number.hpp"

static IType	*getIType(Node *node)
{
	if (!node)
		throw std::logic_error("Invalid AST: null node");
	switch (node->getToken().getType())
	{
		case Token::E_NUMBER:
			return (new Number(node));
		default:
			break;
	}
	throw std::logic_error("Invalid AST: leaf node with invalid token type");
	return (nullptr);
}

Node	*reduce_expression(Node *ast)
{
	// TODO
	if (!ast->getLeft() || !ast->getRight())
		return (ast);
	Node *left = reduce_expression(ast->getLeft());
	// std::cout << "Left: " << left->getEntity() << std::endl;
	Node *right = reduce_expression(ast->getRight());
	// std::cout << "Right: " << right->getEntity() << std::endl;
	IType	*left_entity = getIType(left);
	IType	*right_entity = getIType(right);
	return (*left_entity * *right_entity);
	// if (!dynamic_cast<Operator*>(ast->getEntity()))
	// 	throw std::logic_error("Invalid AST: non-operator node with two children");
	// if (dynamic_cast<Operator*>(ast->getEntity())->get_operator() == Operator::E_MULTIPLY)
	// 	return (*left * *right);
	// return (ast);
}
