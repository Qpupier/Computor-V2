/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/27 17:18:02 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils
static std::string	print_imaginary(Rational imaginary)
{
	if (imaginary.is_integer())
		return std::to_string(imaginary.getNumerator()) + "i";
	return (std::to_string(imaginary.getNumerator()) + "i/" + std::to_string(imaginary.getDenominator()));
}


// Operator overloads
inline	Complex::operator bool() const
{
	return (this->_real || this->_imaginary);
}

bool	Complex::operator!() const
{
	return (!this->_real && !this->_imaginary);
}

bool	Complex::operator==(const Complex &other) const
{
	return (this->_real == other._real && this->_imaginary == other._imaginary);
}

bool	Complex::operator!=(const Complex &other) const
{
	return (!(*this == other));
}

Complex	&Complex::operator=(const Complex &other)
{
	if (this != &other)
	{
		this->_real = other._real;
		this->_imaginary = other._imaginary;
	}
	return (*this);
}

Complex*	Complex::operator+(const Complex &other) const
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	real = this->_real + other._real;
	imaginary = this->_imaginary + other._imaginary;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator+(const Rational &other) const
{
	return (*this + Complex(other, Rational()));
}

IType*	Complex::operator+(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	return (nullptr);
}

Complex*	Complex::operator-(const Complex &other) const
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	real = this->_real - other._real;
	imaginary = this->_imaginary - other._imaginary;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator-(const Rational &other) const
{
	return (*this - Complex(other, Rational()));
}

IType*	Complex::operator-(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	return (nullptr);
}

Complex*	Complex::operator*(const Complex &other) const
{
	Rational*	part1;
	Rational*	part2;
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	part1 = this->_real * other._real;
	part2 = this->_imaginary * other._imaginary;
	real = *part1 - *part2;
	delete part1;
	delete part2;
	part1 = this->_real * other._imaginary;
	part2 = this->_imaginary * other._real;
	imaginary = *part1 + *part2;
	delete part1;
	delete part2;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator*(const Rational &other) const
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	real = this->_real * other;
	imaginary = this->_imaginary * other;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

IType*	Complex::operator*(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	return (nullptr);
}

Complex*	Complex::operator/(const Complex &other) const
{
	Rational*	part1;
	Rational*	part2;
	Rational*	numerator;
	Rational*	denominator;
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	part1 = other._real * other._real;
	part2 = other._imaginary * other._imaginary;
	denominator = *part1 + *part2;
	if (!denominator)
		throw std::runtime_error("Division by zero");
	delete part1;
	delete part2;
	part1 = this->_imaginary * other._real;
	part2 = this->_real * other._imaginary;
	numerator = *part1 - *part2;
	delete part1;
	delete part2;
	real = *numerator / *denominator;
	delete numerator;
	part1 = this->_real * other._real;
	part2 = this->_imaginary * other._imaginary;
	numerator = *part1 + *part2;
	delete part1;
	delete part2;
	imaginary = *numerator / *denominator;
	delete numerator;
	delete denominator;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator/(const Rational &other) const
{
	return (*this / Complex(other, Rational()));
}

IType*	Complex::operator/(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	return (nullptr);
}

Rational*	Complex::operator%(const Complex &other) const
{
	Rational	rational;
	Rational	other_rational;

	try
	{
		rational = *this;
		other_rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw std::logic_error("Modulo operator (%) cannot be applied to complex numbers");
	}
	return (rational % other_rational);
}

Rational*	Complex::operator%(const Rational &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch(const std::logic_error &e)
	{
		throw std::logic_error("Modulo operator (%) cannot be applied to complex numbers");
	}
	return (rational % other);
}

IType*	Complex::operator%(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	return (nullptr);
}

Complex*	Complex::operator^(const Rational &other) const
{
	Rational	exponent;
	Complex*	result;
	Complex*	tmp;

	try
	{
		exponent = other;
		if (!exponent.is_integer())
			throw std::logic_error("Exponent must be an integer");
	}
	catch (const std::logic_error &e)
	{
		throw std::logic_error("Only integers (ℤ) can be used as exponents");
	}
	result = new Complex(*this);
	for (int i = 1; i < exponent.getNumerator(); i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

Complex*	Complex::operator^(const Complex &other) const
{
	Rational	other_rational;

	try
	{
		other_rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw std::logic_error("Only integers (ℤ) can be used as exponents");
	}
	return (*this ^ other_rational);
}

IType*	Complex::operator^(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this ^ *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	return (nullptr);
}


// Getters
Rational	Complex::getImaginary(void) const
{
	return (this->_imaginary);
}

Rational	Complex::getReal(void) const
{
	return (this->_real);
}


// Methods
IType*	Complex::matrix_operator(const IType &other) const
{
	throw std::logic_error("Matrix operator (**) cannot be applied to complex numbers");
	(void)other;
	return (nullptr);
}

Rational	Complex::to_rational(void) const
{
	if (this->_imaginary)
		throw std::logic_error("Cannot convert a complex number with a non-zero imaginary part to a rational number");
	return (this->_real);
}

std::ostream	&Complex::print(std::ostream &os) const
{
	Rational	*operation;

	if (!this->_real)
	{
		if (!this->_imaginary)
			os << "0";
		else if (this->_imaginary == Rational(1))
			os << "i";
		else if (this->_imaginary == Rational(-1))
			os << "-i";
		else
			os << this->_imaginary << "i";
	}
	else
	{
		os << this->_real;
		if (this->_imaginary)
		{
			if (this->_imaginary == Rational(1))
				os << " + i";
			else if (this->_imaginary == Rational(-1))
				os << " - i";
			else
			{
				if (this->_imaginary > Rational(0))
					os << " + " << print_imaginary(this->_imaginary);
				else if (this->_imaginary < Rational(0))
				{
					operation = Rational(-1) * this->_imaginary;
					os << " - " << print_imaginary(*operation);
					delete operation;
				}
			}
		}
	}
	return (os);
}

IType*	Complex::clone(void) const
{
	return (new Complex(*this));
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num)
{
	return (num.print(os) << std::endl);
}
