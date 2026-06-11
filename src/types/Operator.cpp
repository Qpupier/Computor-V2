/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:00:31 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/11 15:29:08 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Operator.hpp"
#include "computor-v2.hpp"

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
	else if (op == "<>")
		_op = E_FUNCTION;
	else
		throw UnexpectedError("Invalid operator: " + op);
}


// Operator overloads

Operator::operator bool() const
{
	return (false);
}

Operator&	Operator::operator=(const Operator &other)
{
	if (this != &other)
		this->_op = other._op;
	return (*this);
}

bool		Operator::operator==(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator==(const long long int value) const
{
	(void)value;
	return (false);
}

bool		Operator::operator!=(const IType &other) const
{
	(void)other;
	return (true);
}

bool		Operator::operator!=(const long long int value) const
{
	(void)value;
	return (true);
}

bool		Operator::operator<(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator<=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator>(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator>=(const IType &other) const
{
	(void)other;
	return (false);
}

IType*		Operator::operator+(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Operator::operator+(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*		Operator::operator-(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Operator::operator-(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Operator::operator-(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*		Operator::operator*(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Operator::operator*(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*		Operator::operator/(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Operator::operator/(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*		Operator::operator%(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Operator::operator%(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*		Operator::operator^(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Operator::operator^(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}


// Getters
Operator::t_operator	Operator::getOperator(void) const
{
	return (this->_op);
}


// Methods
IType*			Operator::matrix_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Operator::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Operator::clone(void) const
{
	return (new Operator(*this));
}

Rational*		Operator::gcd(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

std::ostream&	Operator::print(std::ostream &os) const
{
	return (os);
}

void			Operator::print_rounded(const std::string var) const
{
	throw ERROR_UNEXPECTED;
	(void)var;
}

bool			Operator::finite_decimals(void) const
{
	throw ERROR_UNEXPECTED;
	return (false);
}
