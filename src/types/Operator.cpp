/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:00:31 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/03 12:59:25 by qpupier          ###   ########lyon.fr   */
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
	else if (op == TOKEN_OPERATOR_INVERSE)
		_op = E_INVERSE;
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

bool		Operator::operator<(const long long int value) const
{
	(void)value;
	return (false);
}

bool		Operator::operator<=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator<=(const long long int value) const
{
	(void)value;
	return (false);
}

bool		Operator::operator>(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator>=(const long long int value) const
{
	(void)value;
	return (false);
}

bool		Operator::operator>=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Operator::operator>(const long long int value) const
{
	(void)value;
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

std::string		Operator::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			Operator::in_C(void) const
{
	return (false);
}

bool			Operator::in_D(void) const
{
	return (false);
}

bool			Operator::in_M(void) const
{
	return (false);
}

bool			Operator::in_Q(void) const
{
	return (false);
}

bool			Operator::in_Z(void) const
{
	return (false);
}

std::ostream&	Operator::print(std::ostream &os) const
{
	switch (this->_op)
	{
		case E_ADD:
			return (os << "+");
		case E_SUBTRACT:
			return (os << "-");
		case E_MULTIPLY:
			return (os << "*");
		case E_DIVIDE:
			return (os << "/");
		case E_MODULO:
			return (os << "%");
		case E_MATRIX:
			return (os << "**");
		case E_POWER:
			return (os << "^");
		case E_FUNCTION:
			return (os << "<>");
		case E_INVERSE:
			return (os << TOKEN_OPERATOR_INVERSE);
		default:
			break;
	}
	return (os);
}

IType*			Operator::clone(void) const
{
	return (new Operator(*this));
}

IType*			Operator::cos(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Operator::e(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Operator::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Operator::matrix_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Operator::matrix_inversion(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Operator::sin(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Operator::sqrt(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Operator::tan(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*		Operator::gcd(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

void			Operator::print_rounded(const std::string var) const
{
	throw ERROR_UNEXPECTED;
	(void)var;
}


// Output stream operator overload

std::ostream &operator<<(std::ostream &os, const Operator &op)
{
	return (op.print(os));
}
