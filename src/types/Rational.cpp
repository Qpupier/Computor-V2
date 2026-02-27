/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 19:46:50 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/27 17:18:02 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Rational.hpp"

// Utils
static int	compute_gcd(int a, int b)
{
	if (!b)
		return (std::abs(a));
	return (compute_gcd(b, a % b));
}


// Constructors
Rational::Rational(int numerator, int denominator): _numerator(numerator), _denominator(denominator)
{
	if (denominator == 0)
		throw std::logic_error("Denominator cannot be zero");
	this->reduce();//keep?
}

Rational::Rational(std::string str)
{
	std::size_t	slash_pos;

	slash_pos = str.find('.');
	if (slash_pos == std::string::npos)
	{
		_numerator = std::stoi(str);
		_denominator = 1;
	}
	else
	{
		_numerator = std::stoi(str.erase(slash_pos, 1));
		_denominator = static_cast<int>(std::pow(10, str.size() - slash_pos));
	}
	this->reduce();//keep?
}


// Operator overloads
Rational::operator bool() const
{
	return (this->_numerator);
}

bool	Rational::operator!() const
{
	return (!this->_numerator);
}

bool	Rational::operator==(const Rational &other) const
{
	return (this->_numerator * other._denominator == other._numerator * this->_denominator);
}

bool	Rational::operator!=(const Rational &other) const
{
	return (!(*this == other));
}

bool	Rational::operator<(const Rational &other) const
{
	return (this->_numerator * other._denominator < other._numerator * this->_denominator);
}

bool	Rational::operator<=(const Rational &other) const
{
	return (this->_numerator * other._denominator <= other._numerator * this->_denominator);
}

bool	Rational::operator>(const Rational &other) const
{
	return (this->_numerator * other._denominator > other._numerator * this->_denominator);
}

bool	Rational::operator>=(const Rational &other) const
{
	return (this->_numerator * other._denominator >= other._numerator * this->_denominator);
}

Rational&	Rational::operator=(const Rational &other)
{
	if (this != &other)
	{
		this->_numerator = other._numerator;
		this->_denominator = other._denominator;
	}
	return (*this);
}

Rational	Rational::operator=(const Complex &other)
{
	if (other.getImaginary())
		throw std::logic_error("Cannot assign a complex number with a non-zero imaginary part to a rational number");
	return (Rational(other.getReal()));
}

Rational*	Rational::operator+(const Rational &other) const
{
	return (new Rational(this->_numerator * other._denominator + other._numerator * this->_denominator, this->_denominator * other._denominator));
}

Complex*	Rational::operator+(const Complex &other) const
{
	return (Complex(*this, Rational()) + other);
}

IType*	Rational::operator+(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	return (nullptr);
}

Rational*	Rational::operator-(const Rational &other) const
{
	return (new Rational(this->_numerator * other._denominator - other._numerator * this->_denominator, this->_denominator * other._denominator));
}

Complex*	Rational::operator-(const Complex &other) const
{
	return (Complex(*this, Rational()) - other);
}

IType*	Rational::operator-(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	return (nullptr);
}

Rational*	Rational::operator*(const Rational &other) const
{
	return (new Rational(this->_numerator * other._numerator, this->_denominator * other._denominator));
}

Complex*	Rational::operator*(const Complex &other) const
{
	return (Complex(*this, Rational()) * other);
}

IType*	Rational::operator*(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	return (nullptr);
}

Rational*	Rational::operator/(const Rational &other) const
{
	return (*this * Rational(other.getDenominator(), other.getNumerator()));
}

Complex*	Rational::operator/(const Complex &other) const
{
	return (Complex(*this, Rational()) / other);
}

IType*	Rational::operator/(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	return (nullptr);
}

Rational*	Rational::operator%(const Rational &other) const
{
	Rational	*result;
	Rational	*tmp;

	result = *this / other;
	tmp = result;
	result = other * Rational(result->integer_part());
	delete tmp;
	tmp = result;
	result = *this - *result;
	delete tmp;
	return (result);
}

Rational*	Rational::operator%(const Complex &other) const
{
	return (Complex(*this, Rational()) % other);
}

IType*	Rational::operator%(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	return (nullptr);
}

Rational*	Rational::operator^(const Rational &other) const
{
	int	exponent;

	exponent = other.getNumerator();
	if (other.getDenominator() != 1)
		throw std::logic_error("Exponentiation with non-integer base is not supported");
	return (new Rational(static_cast<int>(std::pow(this->getNumerator(), exponent)), static_cast<int>(std::pow(this->getDenominator(), exponent))));
}

Complex*	Rational::operator^(const Complex &other) const
{
	return (Complex(*this, Rational()) ^ other);
}

IType*	Rational::operator^(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	return (nullptr);
}


// Getters
int		Rational::getNumerator(void) const
{
	return (this->_numerator);
}

int		Rational::getDenominator(void) const
{
	return (this->_denominator);
}

double	Rational::getNumber(void) const
{
	return (static_cast<double>(this->_numerator) / this->_denominator);
}


// Methods
void	Rational::reduce(void)
{
	int	gcd;

	gcd = compute_gcd(this->_numerator, this->_denominator);
	this->_numerator /= gcd;
	this->_denominator /= gcd;
	if (this->_denominator < 0)
	{
		this->_numerator = -this->_numerator;
		this->_denominator = -this->_denominator;
	}
}

IType*	Rational::matrix_operator(const IType &other) const
{
	throw std::logic_error("Matrix operator (**) cannot be applied to rational numbers");
	(void)other;
	return (nullptr);
}

IType*	Rational::clone(void) const
{
	return (new Rational(*this));
}

bool	Rational::is_integer(void) const
{
	Rational	copy(*this);

	copy.reduce();
	return (copy.getDenominator() == 1);
}

int		Rational::integer_part(void) const
{
	return (this->_numerator / this->_denominator);
}

std::ostream&	Rational::print(std::ostream &os) const
{
	Rational	copy(*this);

	copy.reduce();
	if (copy.is_integer())
		return (os << copy.getNumerator());
	return (os << copy.getNumerator() << "/" << copy.getDenominator());
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Rational &num)
{
	return (num.print(os));
}
