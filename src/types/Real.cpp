/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Real.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 11:05:28 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/06 12:04:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Real.hpp"

// Utils

static Real			from_rational(const Rational& rational)
{
	return (Real(InfiniteFloat(rational.getNumerator()) 	\
			/ InfiniteFloat(rational.getDenominator())));
}

static Real			from_complex(const Complex& complex)
{
	if (*complex.getImaginary())
		throw ERROR_UNEXPECTED;
	return (Real(*complex.getReal()));
}

static Real			from_polynomial(const Polynomial& polynomial)
{
	if (polynomial.getDividers().size() != 1 					\
			|| *polynomial.getDividers()[0].coefficient != 1 	\
			|| polynomial.getDividers()[0].power 				\
			|| polynomial.getTerms().size() != 1 				\
			|| polynomial.getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	return (Real(*polynomial.getTerms()[0].coefficient));
}

static Rational*	get_real_new_term_coeff(long long int k, const Rational& k2)
{
	Rational*	coeff;
	Rational*	coeff_num;
	Rational*	factorial;

	coeff_num = Rational(-1) ^ k;
	factorial = k2.factorial();
	coeff = *coeff_num / *factorial;
	delete coeff_num;
	delete factorial;
	return (coeff);
}

static IType*		cos_get_new_term(const Real& number, long long int k)
{
	Real*		real_power;
	Real*		new_term;
	Rational*	coeff;
	Rational*	k2;

	k2 = Rational(k) * 2;
	coeff = get_real_new_term_coeff(k, *k2);
	real_power = number ^ *k2;
	delete k2;
	new_term = *coeff * *real_power;
	delete coeff;
	delete real_power;
	return (new_term);
}

static IType*		sin_get_new_term(const Real& number, long long int k)
{
	Real*		real_power;
	Real*		new_term;
	Rational*	coeff;
	Rational*	k2;
	Rational*	tmp;

	k2 = Rational(k) * 2;
	tmp = k2;
	k2 = *tmp + 1;
	delete tmp;
	coeff = get_real_new_term_coeff(k, *k2);
	real_power = number ^ *k2;
	delete k2;
	new_term = *coeff * *real_power;
	delete coeff;
	delete real_power;
	return (new_term);
}

static IType*		exp_get_new_term(const Real& number, long long int k)
{
	IType*		new_term;
	IType*		power;
	Rational*	factorial;

	power = number ^ k;
	factorial = Rational(k).factorial();
	new_term = *power / *factorial;
	delete power;
	delete factorial;
	return (new_term);
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

inline		Real::operator bool() const
{
	return (static_cast<bool>(this->_value));
}

Real&		Real::operator=(const Real& other)
{
	if (this != &other)
		this->_value = other.getValue();
	return (*this);
}

Real&		Real::operator=(const Real* other)
{
	if (this != other)
		this->_value = other->getValue();
	return (*this);
}

bool		Real::operator==(const IType &other) const
{
	try
	{
		return (this->_value == Real(other).getValue());
	}
	catch(...) {}
	return (false);
}

bool		Real::operator==(const long long int value) const
{
	return (*this == Real(value));
}

bool		Real::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Real::operator!=(const long long int value) const
{
	return (*this != Real(value));
}

bool		Real::operator<(const IType &other) const
{
	return (this->_value < Real(other).getValue());
}

bool		Real::operator<(const long long int value) const
{
	return (*this < Real(value));
}

bool		Real::operator<=(const IType &other) const
{
	return (*this < other || *this == other);
}

bool		Real::operator<=(const long long int value) const
{
	return (*this <= Real(value));
}

bool		Real::operator>(const IType &other) const
{
	return (!(*this <= other));
}

bool		Real::operator>(const long long int value) const
{
	return (*this > Real(value));
}

bool		Real::operator>=(const IType &other) const
{
	return (!(*this < other));
}

bool		Real::operator>=(const long long int value) const
{
	return (*this >= Real(value));
}

IType*		Real::operator+(const IType &other) const
{
	const Real*	other_real;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this + *other_real);
	return (other + *this);
}

Real*		Real::operator+(const Real& other) const
{
	return (new Real(this->_value + other.getValue()));
}

Real*		Real::operator+(const long long int value) const
{
	return (new Real(this->_value + InfiniteFloat(value)));
}

IType*		Real::operator-(void) const
{
	return (new Real(-this->_value));
}

IType*		Real::operator-(const IType& other) const
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

Real*		Real::operator-(const Real& other) const
{
	return (new Real(this->_value - other.getValue()));
}

Real*		Real::operator-(const long long int value) const
{
	return (new Real(this->_value - InfiniteFloat(value)));
}

IType*		Real::operator*(const IType& other) const
{
	const Real*	other_real;

	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this * *other_real);
	return (other * *this);
}

Real*		Real::operator*(const Real& other) const
{
	return (new Real(this->_value * other.getValue()));
}

Real*		Real::operator*(const long long int value) const
{
	return (new Real(this->_value * InfiniteFloat(value)));
}

IType*		Real::operator/(const IType& other) const
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

Real*		Real::operator/(const Real& other) const
{
	return (new Real(this->_value / other.getValue()));
}

Real*		Real::operator/(const Rational& other) const
{
	return (*this / Real(other));
}

Complex*	Real::operator/(const Complex& other) const
{
	return (Complex(*this) / other);
}

Matrix*		Real::operator/(const Matrix& other) const
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

Polynomial*	Real::operator/(const Polynomial& other) const
{
	return (Polynomial(*this) / other);
}

Vector*		Real::operator/(const Vector& other) const
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

Real*		Real::operator/(const long long int value) const
{
	return (new Real(this->_value / value));
}

IType*		Real::operator%(const IType& other) const
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

Real*		Real::operator%(const Real& other) const
{
	return (new Real(this->_value % other.getValue()));
}

Real*		Real::operator%(const Rational& other) const
{
	return (*this % Real(other));
}

Complex*	Real::operator%(const Complex& other) const
{
	return (Complex(*this) % other);
}

Matrix*		Real::operator%(const Matrix& other) const
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

Polynomial*	Real::operator%(const Polynomial& other) const
{
	return (Polynomial(*this) % other);
}

Vector*		Real::operator%(const Vector& other) const
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

Real*		Real::operator%(const long long int value) const
{
	return (new Real(this->_value % value));
}

Real*		Real::operator^(const Rational& other) const
{
	Real*	result;
	Real*	tmp;

	if (!other.in_N())
		throw UNSUPPORTED_EXPONENT;
	result = new Real(Rational(1));
	for (InfiniteInt i(0); i < other.getNumerator(); i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

IType*		Real::operator^(const IType& other) const
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

Real*		Real::operator^(const long long int value) const
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

bool			Real::in_C(void) const
{
	return (false);
}

bool			Real::in_D(void) const
{
	return (false);
}

bool			Real::in_M(void) const
{
	return (false);
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

IType*			Real::abs(void) const
{
	return (new Real(this->_value.abs()));
}

IType*			Real::clone(void) const
{
	return (new Real(*this));
}

IType*			Real::cos(void) const
{
	long long int	k(0);
	IType*			new_term;
	IType*			result;
	IType*			tmp;

	result = new Real(0);
	while (true)
	{
		new_term = cos_get_new_term(*this, k);
		tmp = result;
		result = *result + *new_term;
		delete new_term;
		if (*result == *tmp)
		{
			delete tmp;
			return (result);
		}
		delete tmp;
		k++;
	}
	return (nullptr);
}

IType*			Real::exp(void) const
{
	long long int	k(0);
	IType*			new_term;
	IType*			result;
	IType*			tmp;

	result = new Real(0);
	while (true)
	{
		new_term = exp_get_new_term(*this, k);
		tmp = result;
		result = *result + *new_term;
		delete new_term;
		if (*result == *tmp)
		{
			delete tmp;
			return (result);
		}
		delete tmp;
		k++;
	}
	return (nullptr);
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

IType*			Real::norm(void) const
{
	throw LogicError("Norm is not defined for real numbers, "
			"use the absolute function instead");
	return (nullptr);
}

IType*			Real::sin(void) const
{
	long long int	k(0);
	IType*			new_term;
	IType*			result;
	IType*			tmp;

	result = new Real(0);
	while (true)
	{
		new_term = sin_get_new_term(*this, k);
		tmp = result;
		result = *result + *new_term;
		delete new_term;
		if (*result == *tmp)
		{
			delete tmp;
			return (result);
		}
		delete tmp;
		k++;
	}
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

IType*			Real::tan(void) const
{
	IType*	cosine;
	IType*	result;
	IType*	sine;

	cosine = this->cos();
	if (!*cosine)
		throw LogicError("Tangent is undefined for this value");
	sine = this->sin();
	result = *sine / *cosine;
	delete sine;
	delete cosine;
	return (result);
}

Rational*		Real::gcd(const IType &other) const
{
	return (new Rational(1));
	(void)other;
}

Real*			Real::rad(void) const
{
	InfiniteInt	pi_denominator(1);
	double		pi_integer_part(M_PI);
	Real*		product;
	Real*		result;

	while (std::floor(pi_integer_part) != pi_integer_part)
	{
		pi_integer_part *= 10;
		pi_denominator *= 10;
	}
	product = *this 													\
			* Rational(static_cast<long long int>(pi_integer_part), 	\
				pi_denominator).getValue();
	result = *product / Real(180);
	delete product;
	return (result);
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
