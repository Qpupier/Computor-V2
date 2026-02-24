/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 19:11:26 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 17:48:18 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// #include "Node.hpp"
#include "Number.hpp"
#include "Imaginary.hpp"

static IType	*getIType(Node *node)
{
	if (!node)
		throw std::logic_error("Invalid AST: null node");
	switch (node->getToken().getType())
	{
		case Token::E_NUMBER:
			return (new Number(node));
		case Token::E_IMAGINARY:
			return (new Imaginary());
		default:
			break;
	}
	// std::cerr << "Invalid token type in leaf node: " << node->getToken().getValue() << std::endl;
	// throw std::logic_error("Invalid AST: leaf node with invalid token type");
	return (nullptr);
}

Node	*reduce_expression(Node *ast)
{
	// TODO
	// Si pas de variable
	// return (ast);
	if (!ast->getLeft() || !ast->getRight())
		return (ast);
	Node *left = reduce_expression(ast->getLeft());
	Node *right = reduce_expression(ast->getRight());
	IType	*left_entity = getIType(left);
	IType	*right_entity = getIType(right);
	if (!left_entity || !right_entity)
		return (ast);
	if (ast->getToken().getValue() == "+")
		return (*left_entity + *right_entity);
	if (ast->getToken().getValue() == "-")
		return (*left_entity - *right_entity);
	if (ast->getToken().getValue() == "*")
		return (*left_entity * *right_entity);
	if (ast->getToken().getValue() == "/")
		return (*left_entity / *right_entity);
	if (ast->getToken().getValue() == "%")
		return (*left_entity % *right_entity);
	if (ast->getToken().getValue() == "^")
		return (*left_entity ^ *right_entity);
	if (ast->getToken().getValue() == "**")
		return (left_entity->matrix_operator(*right_entity));
	throw std::logic_error("Invalid AST: non-operator node with two children (" + ast->getToken().getValue() + ")");
	return (nullptr);
}
