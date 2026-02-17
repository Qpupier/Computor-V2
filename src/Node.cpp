/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 13:42:09 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 16:53:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Node.hpp"

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
