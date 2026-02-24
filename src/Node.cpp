/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 13:42:09 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 18:00:48 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Node.hpp"
#include "Token.hpp"
// #include "Number.hpp"
#include "Variable.hpp"

// Operator overloads
Node&	Node::operator=(const Node &other)
{
	if (this != &other)
	{
		this->_token = other._token;
		this->_left = other._left;
		this->_right = other._right;
	}
	return (*this);
}

// Node*	Node::operator*(const Node &other) const
// {
// 	Node *result = new Node();

// 	result->_tokens = { Token(Token::E_NUMBER, std::to_string(std::stoi(this->_tokens[0].get_value()) * std::stoi(other._tokens[0].get_value()))) };
// 	return (result);
// }


// Getters
Token	Node::getToken(void) const
{
	return (this->_token);
}

Node*	Node::getLeft(void) const
{
	return (this->_left);
}

Node*	Node::getRight(void) const
{
	return (this->_right);
}


// Setters
void	Node::setLeft(Node* left)
{
	this->_left = left;
}

void	Node::setRight(Node* right)
{
	this->_right = right;
}
