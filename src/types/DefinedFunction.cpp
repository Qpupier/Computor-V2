/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefinedFunction.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 15:35:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/31 15:55:10 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "DefinedFunction.hpp"
#include "computor-v2.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Polynomial.hpp"
#include "Vector.hpp"

// Operator overloads

DefinedFunction::operator bool(void) const
{
	return (this->_function != E_FUNCTION_NOTHING);
}

bool	DefinedFunction::operator==(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (false);
}

bool	DefinedFunction::operator==(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (false);
}

bool	DefinedFunction::operator!=(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (false);
}

bool	DefinedFunction::operator!=(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (false);
}

bool	DefinedFunction::operator<(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (false);
}

bool	DefinedFunction::operator<(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (false);
}

bool	DefinedFunction::operator<=(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (false);
}

bool	DefinedFunction::operator<=(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (false);
}

bool	DefinedFunction::operator>(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (false);
}

bool	DefinedFunction::operator>(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (false);
}

bool	DefinedFunction::operator>=(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (false);
}

bool	DefinedFunction::operator>=(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (false);
}

IType*	DefinedFunction::operator+(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*	DefinedFunction::operator+(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*	DefinedFunction::operator-(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*	DefinedFunction::operator-(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*	DefinedFunction::operator-(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*	DefinedFunction::operator*(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*	DefinedFunction::operator*(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*	DefinedFunction::operator/(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*	DefinedFunction::operator/(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*	DefinedFunction::operator%(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*	DefinedFunction::operator%(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}

IType*	DefinedFunction::operator^(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*	DefinedFunction::operator^(const long long int value) const
{
	throw ERROR_UNEXPECTED;
	(void)value;
	return (nullptr);
}


// Getters

IType::t_type	DefinedFunction::getType(void) const
{
	return (IType::t_type::E_TYPE_FUNCTION);
}


// Methods

std::string		DefinedFunction::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			DefinedFunction::in_C(void) const
{
	return (false);
}

bool			DefinedFunction::in_D(void) const
{
	return (false);
}

bool			DefinedFunction::in_M(void) const
{
	return (false);
}

bool			DefinedFunction::in_Q(void) const
{
	return (false);
}

bool			DefinedFunction::in_Z(void) const
{
	return (false);
}

std::ostream&	DefinedFunction::print(std::ostream &os) const
{
	switch (this->_function)
	{
		case E_FUNCTION_NORM:
			return (os << "norm");
		case E_FUNCTION_ABSOLUTE:
			return (os << "abs");
		case E_FUNCTION_SQRT:
			return (os << "sqrt");
		case E_FUNCTION_EXPONENTIAL:
			return (os << "exp");
		case E_FUNCTION_COSINE:
			return (os << "cos");
		case E_FUNCTION_SINE:
			return (os << "sin");
		case E_FUNCTION_TANGENT:
			return (os << "tan");
		case E_FUNCTION_RADIAN:
			return (os << "rad");
		case E_FUNCTION_DEGREE:
			return (os << "deg");
		case E_FUNCTION_FACTORIAL:
			return (os << "fact");
		default:
			throw ERROR_UNKNOWN_FUNCTION;
	}
	return (os);
}

IType*			DefinedFunction::abs(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::clone(void) const
{
	return (new DefinedFunction(*this));
}

IType*			DefinedFunction::cos(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::exp(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::function_operator(const IType &other) const
{
	switch (this->_function)
	{
		case E_FUNCTION_NORM:
			return (other.norm());
		case E_FUNCTION_ABSOLUTE:
			return (other.abs());
		case E_FUNCTION_SQRT:
			return (other.sqrt());
		case E_FUNCTION_EXPONENTIAL:
			return (other.exp());
		case E_FUNCTION_COSINE:
			return (other.cos());
		case E_FUNCTION_SINE:
			return (other.sin());
		case E_FUNCTION_TANGENT:
			return (other.tan());
		case E_FUNCTION_RADIAN:
			return (other.rad());
		case E_FUNCTION_DEGREE:
			return (other.deg());
		case E_FUNCTION_FACTORIAL:
			return (other.fact());
		default:
			throw ERROR_UNKNOWN_FUNCTION;
	}
}

IType*			DefinedFunction::matrix_inversion(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::matrix_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			DefinedFunction::norm(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::sin(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::sqrt(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			DefinedFunction::tan(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*		DefinedFunction::fact(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*		DefinedFunction::gcd(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

Real*			DefinedFunction::deg(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Real*			DefinedFunction::rad(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

void			DefinedFunction::print_rounded(const std::string var) const
{
	std::cout << var << " = " << *this << std::endl;
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream& os, const DefinedFunction& function)
{
	return (function.print(os));
}
