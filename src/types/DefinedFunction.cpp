/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefinedFunction.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 15:35:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 16:49:19 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "DefinedFunction.hpp"
#include "computor-v2.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Polynomial.hpp"
#include "Vector.hpp"

// Utils

static IType*	function_absolute(const IType &other)
{
	const Rational*	other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (other < 0 ? -other : other.clone());
	throw LogicError("Absolute function only exists for Rational types");
	return (nullptr);
}


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


// Methods

IType*			DefinedFunction::clone(void) const
{
	return (new DefinedFunction(*this));
}

IType*			DefinedFunction::function_operator(const IType &other) const
{
	switch (this->_function)
	{
		case E_FUNCTION_NORM:
			return (other.clone());// TODO
		case E_FUNCTION_ABSOLUTE:
			return (function_absolute(other));
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

Rational*		DefinedFunction::gcd(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

std::ostream&	DefinedFunction::print(std::ostream &os) const
{
	switch (this->_function)
	{
		case E_FUNCTION_NORM:
			return (os << "norm");
		case E_FUNCTION_ABSOLUTE:
			return (os << "abs");
		default:
			throw ERROR_UNKNOWN_FUNCTION;
	}
	return (os);
}

std::string		DefinedFunction::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
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
