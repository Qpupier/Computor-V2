/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:18:03 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 18:39:52 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Token.hpp"
#include "Complex.hpp"
#include "Rational.hpp"

// Constructors
AST::AST(const Token &token): _left(nullptr), _right(nullptr)
{
	switch (token.getType())
	{
		case Token::E_NUMBER:
		{
			_node = new Rational(token);
			break;
		}
		case Token::E_IMAGINARY:
		{
			_node = new Complex();
			break;
		}
		default:
			break;
	}
}


// Operator overloads
AST& AST::operator=(const AST &other)
{
	if (this != &other)
	{
		_node = other._node;
		_left = other._left;
		_right = other._right;
	}
	return (*this);
}
