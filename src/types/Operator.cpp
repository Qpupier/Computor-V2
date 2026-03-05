/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:00:31 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/05 19:58:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Operator.hpp"

// Constructors
Operator::Operator(std::string op)
{
	if (op == "+")
		_op = E_ADD;
	else if (op == "-")
		_op = E_SUBTRACT;
	else if (op == "*")
		_op = E_MULTIPLY;
	else if (op == "/")
		_op = E_DIVIDE;
	else if (op == "%")
		_op = E_MODULO;
	else if (op == "**")
		_op = E_MATRIX;
	else if (op == "^")
		_op = E_POWER;
	else if (op == "***")
		_op = E_UNKNOWN;
	else
		throw std::runtime_error("Invalid operator: " + op);
}


// Operator overloads
Operator&	Operator::operator=(const Operator &other)
{
	if (this != &other)
		this->_op = other._op;
	return (*this);
}

IType*	Operator::operator+(const IType &other) const
{
	(void)other;
	return (nullptr);
}

IType*	Operator::operator-(const IType &other) const
{
	(void)other;
	return (nullptr);
}

IType*	Operator::operator*(const IType &other) const
{
	(void)other;
	return (nullptr);
}

IType*	Operator::operator/(const IType &other) const
{
	(void)other;
	return (nullptr);
}

IType*	Operator::operator%(const IType &other) const
{
	(void)other;
	return (nullptr);
}

IType*	Operator::operator^(const IType &other) const
{
	(void)other;
	return (nullptr);
}


// Getters
Operator::t_operator	Operator::getOperator(void) const
{
	return (this->_op);
}


// Methods
IType*	Operator::matrix_operator(const IType &other) const
{
	(void)other;
	return (nullptr);
}

std::ostream	&Operator::print(std::ostream &os) const
{
	os << " ";
	switch (this->_op)
	{
		case E_ADD:
		{
			os << "+";
			break;
		}
		case E_SUBTRACT:
		{
			os << "-";
			break;
		}
		case E_MULTIPLY:
		{
			os << "*";
			break;
		}
		case E_DIVIDE:
		{
			os << "/";
			break;
		}
		case E_MODULO:
		{
			os << "%";
			break;
		}
		case E_POWER:
		{
			os << "^";
			break;
		}
		case E_MATRIX:
		{
			os << "**";
			break;
		}
		default:
			break;
	}
	os << " ";
	return (os);
}

IType*	Operator::clone(void) const
{
	return (new Operator(*this));
}
