/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:18:03 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/04 14:48:20 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Operator.hpp"
#include "Rational.hpp"
#include "Complex.hpp"

// Constructors and destructor
AST::AST(const Token &token): _left(nullptr), _right(nullptr)
{
	switch (token.getType())
	{
		case Token::E_OPERATOR:
		{
			_node = new Operator(token);
			break;
		}
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
			throw std::logic_error("Invalid token type for AST node");
	}
}

AST::AST(const Token &token, AST *left, AST *right): AST(token)
{
	this->_left = left;
	this->_right = right;
}

AST::~AST(void)
{
	if (this->_left)
		delete this->_left;
	if (this->_right)
		delete this->_right;
	if (this->_node)
		delete this->_node;
}


// Operator overloads
AST& AST::operator=(const AST &other)
{
	if (this != &other)
	{
		this->~AST();
		_node = other._node->clone();
		_left = other._left;
		_right = other._right;
	}
	return (*this);
}


// Getters
IType*	AST::getNode(void) const
{
	return (this->_node);
}


// Setters
void	AST::setLeft(AST *left)
{
	this->_left = left;
}

void	AST::setRight(AST *right)
{
	this->_right = right;
}


// Methods
std::ostream	&AST::print(std::ostream &os) const
{
	if (this->_left)
		os << *this->_left;
	if (this->getNode())
		os << *this->getNode();
	if (this->_right)
		os << *this->_right;
	return (os);
}

void	AST::reduce_expression(void)
{
	AST	*tmp;

	// TODO
	// Si pas de variable
	// return;
	if (!this->_left || !this->_right)
		return;
	this->_left->reduce_expression();
	this->_right->reduce_expression();
	IType	*left_entity = this->_left->getNode();
	IType	*right_entity = this->_right->getNode();
	if (!left_entity || !right_entity)
		return;
	Operator *op = dynamic_cast<Operator*>(this->_node);
	if (op)
	{
		IType* result = nullptr;
		try
		{
			switch (op->getOperator())
			{
				case Operator::E_ADD:
				{
					result = *left_entity + *right_entity;
					break;
				}
				case Operator::E_SUBTRACT:
				{
					result = *left_entity - *right_entity;
					break;
				}
				case Operator::E_MULTIPLY:
				{
					result = *left_entity * *right_entity;
					break;
				}
				case Operator::E_DIVIDE:
				{
					result = *left_entity / *right_entity;
					break;
				}
				case Operator::E_MODULO:
				{
					result = *left_entity % *right_entity;
					break;
				}
				case Operator::E_POWER:
				{
					result = *left_entity ^ *right_entity;
					break;
				}
				case Operator::E_MATRIX:
				{
					result = left_entity->matrix_operator(*right_entity);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown operator");
					break;
				}
			}
		}
		catch (const std::exception &e)
		{
			delete result;
			delete this;
			throw;
		}
		tmp = new AST(result);
		*this = *tmp;
		delete tmp;
	}
	return;
}


// Output stream operator overload
std::ostream &operator<<(std::ostream &os, const AST &ast)
{
	return (ast.print(os));
}
