/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 19:46:50 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 12:10:56 by qpupier          ###   ########lyon.fr   */
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
		throw std::runtime_error("Denominator cannot be zero");
	this->reduce();
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
	this->reduce();
}


// Operator overloads
Rational	&Rational::operator=(const Rational &other)
{
	if (this != &other)
	{
		this->_numerator = other._numerator;
		this->_denominator = other._denominator;
	}
	return (*this);
}

AST	*Rational::operator+(const Rational &other) const
{
	return (new AST(
		new Rational(this->_numerator * other._denominator + other._numerator * this->_denominator, this->_denominator * other._denominator)));
}

AST	*Rational::operator+(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	return (nullptr);
}

AST	*Rational::operator-(const Rational &other) const
{
	return (new AST(new Rational(this->_numerator * other._denominator - other._numerator * this->_denominator, this->_denominator * other._denominator)));
}

AST	*Rational::operator-(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	return (nullptr);
}

AST	*Rational::operator*(const Rational &other) const
{
	return (new AST(new Rational(this->_numerator * other._numerator, this->_denominator * other._denominator)));
}

AST	*Rational::operator*(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	return (nullptr);
}

AST	*Rational::operator/(const Rational &other) const
{
	return (*this * Rational(other.get_denominator(), other.get_numerator()));
}

AST	*Rational::operator/(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	return (nullptr);
}

AST	*Rational::operator%(const Rational &other) const
{
	AST			*operation_result;
	Rational	*operation_result_rational;

	operation_result = *this / other;
	operation_result_rational = dynamic_cast<Rational*>(operation_result->getNode()->clone());
	delete operation_result;
	if (operation_result_rational)
	{
		operation_result = other * Rational(operation_result_rational->integer_part());
		delete operation_result_rational;
		operation_result_rational = dynamic_cast<Rational*>(operation_result->getNode()->clone());
		delete operation_result;
		if (operation_result_rational)
		{
			operation_result = *this - *operation_result_rational;
			delete operation_result_rational;
			return (operation_result);
		}
	}
	throw std::logic_error("Modulo operation is only supported for rational numbers");
	return (nullptr);
}

AST	*Rational::operator%(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	return (nullptr);
}

AST	*Rational::operator^(const Rational &other) const
{
	int	exponent;

	exponent = other.get_numerator();
	if (other.get_denominator() != 1)
		throw std::logic_error("Exponentiation with non-integer base is not supported");
	return (new AST(new Rational(static_cast<int>(std::pow(this->get_numerator(), exponent)), static_cast<int>(std::pow(this->get_denominator(), exponent)))));
}

AST	*Rational::operator^(const IType &other) const
{
	const Rational	*other_rational;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	return (nullptr);
}


// Getters
int		Rational::get_numerator(void) const
{
	return (this->_numerator);
}

int		Rational::get_denominator(void) const
{
	return (this->_denominator);
}

double	Rational::get_number(void) const
{
	return (static_cast<double>(this->_numerator) / this->_denominator);
}


// Methods
AST*	Rational::matrix_operator(const IType &other) const
{
	throw std::logic_error("Matrix operator (**) cannot be applied to rational numbers");
	(void)other;
	return (nullptr);
}

int		Rational::integer_part(void) const
{
	return (this->_numerator / this->_denominator);
}

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

std::ostream&	Rational::print(std::ostream &os) const
{
	Rational	copy(*this);

	// copy.reduce();
	if (copy.get_denominator() == 1)
		return (os << copy.get_numerator());
	return (os << copy.get_numerator() << "/" << copy.get_denominator());
}

IType*	Rational::clone(void) const
{
	return (new Rational(*this));
}
