/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Boolean.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:33:23 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 16:57:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Boolean.hpp"

// Operator overloads

inline		Boolean::operator bool() const
{
	return (this->_value);
}

Boolean&	Boolean::operator=(const Boolean& other)
{
	this->_value = other._value;
	return (*this);
}

Boolean		Boolean::operator=(const IType& other)
{
	if (other.getType() != IType::E_TYPE_BOOLEAN)
		throw ERROR_UNEXPECTED;
	this->_value = dynamic_cast<const Boolean&>(other).getValue();
	return (*this);
}

bool		Boolean::operator==(const IType& other) const
{
	if (other.getType() != IType::E_TYPE_BOOLEAN)
		throw ERROR_UNEXPECTED;
	return (this->_value == dynamic_cast<const Boolean&>(other).getValue());
}

bool		Boolean::operator==(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator!=(const IType& other) const
{
	if (other.getType() != IType::E_TYPE_BOOLEAN)
		throw ERROR_UNEXPECTED;
	return (this->_value != dynamic_cast<const Boolean&>(other).getValue());
}

bool		Boolean::operator!=(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator<(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator<(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator<=(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator<=(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator>(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator>(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator>=(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
}

bool		Boolean::operator>=(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
}

IType*		Boolean::operator+(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator+(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator-(void) const
{
	return (new Boolean(!this->_value));
}

IType*		Boolean::operator-(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator-(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator*(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator*(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator/(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator/(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator%(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator%(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator^(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Boolean::operator^(const long long int value) const
{
	(void)value;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}


// Getters

IType::t_type	Boolean::getType(void) const
{
	return (IType::E_TYPE_BOOLEAN);
}

bool			Boolean::getValue(void) const
{
	return (this->_value);
}


// Setters

void	Boolean::setValue(bool value)
{
	this->_value = value;
}


// Methods

std::string		Boolean::to_string(void) const
{
	return (this->_value ? "True" : "False");
}

bool			Boolean::in_C(void) const
{
	throw ERROR_UNEXPECTED;
}

bool			Boolean::in_D(void) const
{
	throw ERROR_UNEXPECTED;
}

bool			Boolean::in_M(void) const
{
	throw ERROR_UNEXPECTED;
}

bool			Boolean::in_Q(void) const
{
	throw ERROR_UNEXPECTED;
}

bool			Boolean::in_Z(void) const
{
	throw ERROR_UNEXPECTED;
}

std::ostream&	Boolean::print(std::ostream& os) const
{
	os << this->to_string();
	return (os);
}

IType*			Boolean::abs(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::clone(void) const
{
	return (new Boolean(*this));
}

IType*			Boolean::cos(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::exp(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::function_operator(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::matrix_inversion(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::matrix_operator(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::norm(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::sin(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::sqrt(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Boolean::tan(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*		Boolean::fact(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*		Boolean::gcd(const IType& other) const
{
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Real*			Boolean::deg(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Real*			Boolean::rad(void) const
{
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

void			Boolean::print_rounded(const std::string var) const
{
	(void)var;
	throw ERROR_UNEXPECTED;
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream& os, const Boolean& boolean)
{
	return (boolean.print(os));
}
