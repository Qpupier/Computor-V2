/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Real.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 11:05:28 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/28 16:16:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Real.hpp"

// Utils

static Real	from_rational(const Rational& rational)
{
	return (Real(InfiniteFloat(rational.getNumerator()) 	\
			/ InfiniteFloat(rational.getDenominator())));
}

static Real	from_complex(const Complex& complex)
{
	if (*complex.getImaginary())
		throw ERROR_UNEXPECTED;
	return (Real(*complex.getReal()));
}

static Real	from_polynomial(const Polynomial& polynomial)
{
	if (polynomial.getDividers().size() != 1 					\
			|| *polynomial.getDividers()[0].coefficient != 1 	\
			|| polynomial.getDividers()[0].power 				\
			|| polynomial.getTerms().size() != 1 				\
			|| polynomial.getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	return (Real(*polynomial.getTerms()[0].coefficient));
}


// Constructor

Real::Real(const IType& other)
{
	const Real*			other_real;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	other_complex = dynamic_cast<const Complex*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		*this = *other_real;
	else if (other_rational)
		*this = from_rational(*other_rational);
	else if (other_complex)
		*this = from_complex(*other_complex);
	else if (other_polynomial)
		*this = from_polynomial(*other_polynomial);
	else
		throw ERROR_UNEXPECTED;
}


// Operator overloads

inline	Real::operator bool() const
{
	return (static_cast<bool>(this->_value));
}

Real&	Real::operator=(const Real& other)
{
	if (this != &other)
		this->_value = other.getValue();
	return (*this);
}

Real&	Real::operator=(const Real* other)
{
	if (this != other)
		this->_value = other->getValue();
	return (*this);
}

bool	Real::operator==(const IType &other) const
{
	try
	{
		return (this->_value == Real(other).getValue());
	}
	catch(...) {}
	return (false);
}

bool	Real::operator==(const long long int value) const
{
	return (*this == Real(value));
}

bool	Real::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool	Real::operator!=(const long long int value) const
{
	return (*this != Real(value));
}

bool	Real::operator<(const IType &other) const
{
	return (this->_value < Real(other).getValue());
}

bool	Real::operator<(const long long int value) const
{
	return (*this < Real(value));
}

bool	Real::operator<=(const IType &other) const
{
	return (*this < other || *this == other);
}

bool	Real::operator<=(const long long int value) const
{
	return (*this <= Real(value));
}

bool	Real::operator>(const IType &other) const
{
	return (!(*this <= other));
}

bool	Real::operator>(const long long int value) const
{
	return (*this > Real(value));
}

bool	Real::operator>=(const IType &other) const
{
	return (!(*this < other));
}

bool	Real::operator>=(const long long int value) const
{
	return (*this >= Real(value));
}

IType*	Real::operator+(const IType &other) const
{
	const Real*	other_real;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this + *other_real);
	return (other + *this);
}

IType*	Real::operator+(const Real& other) const
{
	return (new Real(this->_value + other.getValue()));
}

IType*	Real::operator+(const long long int value) const
{
	return (new Real(this->_value + InfiniteFloat(value)));
}

IType*	Real::operator-(void) const
{
	return (new Real(-this->_value));
}

IType*	Real::operator-(const IType& other) const
{
	const Real*	other_real;
	IType*		neg;
	IType*		result;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this - *other_real);
	neg = other - *this;
	result = -*neg;
	delete neg;
	return (result);
}

IType*	Real::operator-(const Real& other) const
{
	return (new Real(this->_value - other.getValue()));
}

IType*	Real::operator-(const long long int value) const
{
	return (new Real(this->_value - InfiniteFloat(value)));
}

IType*	Real::operator*(const IType& other) const
{
	const Real*	other_real;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this * *other_real);
	return (other * *this);
}

IType*	Real::operator*(const Real& other) const
{
	return (new Real(this->_value * other.getValue()));
}

IType*	Real::operator*(const long long int value) const
{
	return (new Real(this->_value * InfiniteFloat(value)));
}

IType*	Real::operator/(const IType& other) const
{
	const Real*			other_real;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this / *other_real);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this / *other_vector);
	return (nullptr);
}

IType*	Real::operator/(const Real& other) const
{
	return (new Real(this->_value / other.getValue()));
}

IType*	Real::operator/(const Rational& other) const
{
	return (Rational(*this) / other);
}

IType*	Real::operator/(const Complex& other) const
{
	return (Complex(*this) / other);
}

IType*	Real::operator/(const Matrix& other) const
{
	Matrix*	result;

	result = new Matrix(other);
	for (unsigned long int j(0); j < other.getHeight(); j++)
		for (unsigned long int i(0); i < other.getWidth(); i++)
			if (*other[j][i])
				result->setValue(i, j, *this / *other[j][i]);
			else
				throw ERROR_DIVISION_BY_ZERO;
	return (result);
}

IType*	Real::operator/(const Polynomial& other) const
{
	return (Polynomial(*this) / other);
}

IType*	Real::operator/(const Vector& other) const
{
	Vector*	result;

	result = new Vector(other);
	for (unsigned long int i(0); i < other.size(); i++)
		if (*other[i])
			result->setValue(i, *this / *other[i]);
		else
			throw ERROR_DIVISION_BY_ZERO;
	return (result);
}

IType*	Real::operator/(const long long int value) const
{
	return (new Real(this->_value / value));
}

IType*	Real::operator%(const IType& other) const
{
	const Real*			other_real;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this % *other_real);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this % *other_vector);
	return (nullptr);
}

IType*	Real::operator%(const Real& other) const
{
	return (new Real(this->_value % other.getValue()));
}

IType*	Real::operator%(const Rational& other) const
{
	return (Rational(*this) % other);
}

IType*	Real::operator%(const Complex& other) const
{
	return (Complex(*this) % other);
}

IType*	Real::operator%(const Matrix& other) const
{
	Matrix*	result;

	result = new Matrix(other);
	for (unsigned long int j(0); j < other.getHeight(); j++)
		for (unsigned long int i(0); i < other.getWidth(); i++)
			if (*other[j][i])
				result->setValue(i, j, *this % *other[j][i]);
			else
				throw ERROR_DIVISION_BY_ZERO;
	return (result);
}

IType*	Real::operator%(const Polynomial& other) const
{
	return (Polynomial(*this) % other);
}

IType*	Real::operator%(const Vector& other) const
{
	Vector*	result;

	result = new Vector(other);
	for (unsigned long int i(0); i < other.size(); i++)
		if (*other[i])
			result->setValue(i, *this / *other[i]);
		else
			throw ERROR_DIVISION_BY_ZERO;
	return (result);
}

IType*	Real::operator%(const long long int value) const
{
	return (new Real(this->_value % value));
}

IType*	Real::operator^(const IType& other) const
{
	try
	{
		return (*this ^ Rational(other));
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (nullptr);
}

IType*	Real::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
}


// Getters

InfiniteFloat&	Real::getValue(void) const
{
	return (const_cast<InfiniteFloat&>(this->_value));
}


// Methods

std::string		Real::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			Real::in_D(void) const
{
	return (false);// [ ]: pas sur de ca dans mon contexte
}

bool			Real::in_Q(void) const
{
	return (false);
}

bool			Real::in_Z(void) const
{
	return (false);
}

std::ostream&	Real::print(std::ostream &os) const
{
	os << this->_value;
	return (os);
}

IType*			Real::clone(void) const
{
	return (new Real(*this));
}

IType*			Real::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Real::matrix_inversion(void) const
{
	throw ERROR_MATRIX_INVERSION_SQUARE;
	return (nullptr);
}

IType*			Real::matrix_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Real::sqrt(void) const
{
	InfiniteFloat	sqrt_value(1);
	InfiniteFloat	value(this->getValue());
	InfiniteFloat	epsilon(1);
	InfiniteFloat	delta(1);

	for (int i(0); i < InfiniteFloat::CALCULATION_PRECISION; i++)
		epsilon /= InfiniteFloat(10);
	while (delta >= epsilon)
	{
		sqrt_value = (sqrt_value + value / sqrt_value) / 2;
		delta = sqrt_value * sqrt_value - value;
		if (delta < 0)
			delta *= -1;
	}
	return (new Real(sqrt_value));
}

Rational*		Real::gcd(const IType &other) const
{
	return (new Rational(1));// [ ]: pas sur de ca
	(void)other;
}

Real*			Real::abs(void) const
{
	return (new Real(this->_value.abs()));
}

void			Real::print_rounded(const std::string var) const
{
	(void)var;
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream &os, const Real &num)
{
	return (num.print(os));
}
