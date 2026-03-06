/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/06 18:31:15 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils
static std::string	print_imaginary(Rational imaginary)
{
	std::string	numerator;

	numerator = std::to_string(imaginary.getNumerator());
	if (imaginary.is_integer())
		return numerator + "i";
	return (numerator + "i/" + std::to_string(imaginary.getDenominator()));
}


// Operator overloads
Complex		&Complex::operator=(const Complex &other)
{
	if (this != &other)
	{
		this->_real = other._real;
		this->_imaginary = other._imaginary;
	}
	return (*this);
}

inline		Complex::operator bool() const
{
	return (this->_real || this->_imaginary);
}

bool		Complex::operator==(const Complex &other) const
{
	return (this->_real == other._real && this->_imaginary == other._imaginary);
}
	
bool		Complex::operator!=(const Complex &other) const
{
	return (!(*this == other));
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

Matrix*		Complex::operator+(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational + other);
}

IType*		Complex::operator+(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
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

Matrix*		Complex::operator-(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational - other);
}

IType*		Complex::operator-(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
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

Matrix*		Complex::operator*(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational * other);
}

IType*		Complex::operator*(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
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
	delete part1;
	delete part2;
	if (!*denominator)
	{
		delete denominator;
		throw ERROR_DIVISION_BY_ZERO;
	}
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

Matrix*		Complex::operator/(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational / other);
}

IType*		Complex::operator/(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
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
		throw ERROR_MODULO_COMPLEX;
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
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other);
}

Matrix*		Complex::operator%(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch(const std::logic_error &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other);
}

IType*		Complex::operator%(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	return (nullptr);
}

Complex*	Complex::operator^(const Rational &other) const
{
	Rational	exponent;
	Complex*	result;
	Complex*	tmp;
	int			numerator;

	try
	{
		exponent = other;
		if (!exponent.is_integer())
			throw ERROR_EXPONENT_INTEGER;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_EXPONENT_INTEGER;
	}
	result = new Complex(*this);
	numerator = exponent.getNumerator();
	for (int i = 1; i < numerator; i++)
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
		throw ERROR_EXPONENT_INTEGER;
	}
	return (*this ^ other_rational);
}

Matrix*		Complex::operator^(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_EXPONENT_INTEGER;
	}
	return (rational ^ other);
}

IType*		Complex::operator^(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this ^ *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this ^ *other_matrix);
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
IType*			Complex::matrix_operator(const IType &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	return (nullptr);
}

std::ostream&	Complex::print(std::ostream &os) const
{
	Rational	*operation;
	Rational	minus_one(-1);
	Rational	zero(0);
	Rational	one(1);

	if (!this->_real)
	{
		if (!this->_imaginary)
			os << "0";
		else if (this->_imaginary == one)
			os << "i";
		else if (this->_imaginary == minus_one)
			os << "-i";
		else
			os << this->_imaginary << "i";
	}
	else
	{
		os << this->_real;
		if (this->_imaginary)
		{
			if (this->_imaginary == one)
				os << " + i";
			else if (this->_imaginary == minus_one)
				os << " - i";
			else
			{
				if (this->_imaginary > zero)
					os << " + " << print_imaginary(this->_imaginary);
				else if (this->_imaginary < zero)
				{
					operation = minus_one * this->_imaginary;
					os << " - " << print_imaginary(*operation);
					delete operation;
				}
			}
		}
	}
	return (os);
}

IType*			Complex::clone(void) const
{
	return (new Complex(*this));
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num)
{
	return (num.print(os) << std::endl);
}
