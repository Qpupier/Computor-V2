/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefinedFunction.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 15:35:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 15:55:56 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "DefinedFunction.hpp"
#include "computor-v2.hpp"

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
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
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
			os << "norm";
			break;
		case E_FUNCTION_ABS:
			os << "abs";
			break;
		default:
			throw UnexpectedError("Unknown function");
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
