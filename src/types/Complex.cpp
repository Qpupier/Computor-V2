/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/27 19:14:36 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils
static std::ostream&	print_value(std::ostream &os, Rational value, 	\
		const std::string &i, bool is_first)
{
	Rational	copy(value);

	if (!value)
		return (os);
	if (!is_first)
	{
		if (value < Rational(0))
		{
			os << " - ";
			copy = value * Rational(-1);
		}
		else
			os << " + ";
	}
	else if (value < Rational(0) && i != std::string())
	{
		os << "-";
		copy = value * Rational(-1);
	}
	if (i == std::string() || copy != Rational(1))
		os << copy.getNumerator();
	if (i != std::string())
		os << i;
	if (copy.getDenominator() != 1)
		os << "/" << copy.getDenominator();
	return (os);
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

bool		Complex::operator==(const Rational &other) const
{
	return (!this->_imaginary && this->_real == other);
}

bool		Complex::operator==(const Matrix &other) const
{
	(void)other;
	return (false);
}

bool		Complex::operator==(const Polynomial &other) const
{
	return (!other.getPower2() && !other.getPower1() && *this == *other.getPower0());
}

bool		Complex::operator==(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this == *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this == *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this == *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this == *other_polynomial);
	return (false);
}

bool		Complex::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Complex::operator<(const IType &other) const
{
	return (!this->_imaginary && this->_real < other);
}

bool		Complex::operator<=(const IType &other) const
{
	return (!this->_imaginary && this->_real <= other);
}

bool		Complex::operator>(const IType &other) const
{
	return (!this->_imaginary && this->_real > other);
}

bool		Complex::operator>=(const IType &other) const
{
	return (!this->_imaginary && this->_real >= other);
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
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational + other);
}

Polynomial*	Complex::operator+(const Polynomial &other) const
{
	return (new Polynomial(other.getName(), other.getPower2()->clone(), other.getPower1()->clone(), *this + *other.getPower0()));
}

IType*		Complex::operator+(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	throw ERROR_UNEXPECTED;
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
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational - other);
}

Polynomial*	Complex::operator-(const Polynomial &other) const
{
	return (new Polynomial(other.getName(), other.getPower2()->clone(), other.getPower1()->clone(), *this - *other.getPower0()));
}

IType*		Complex::operator-(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	throw ERROR_UNEXPECTED;
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
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational * other);
}

Polynomial*	Complex::operator*(const Polynomial &other) const
{
	return (new Polynomial(other.getName(), *this * *other.getPower2(), *this * *other.getPower1(), *this * *other.getPower0()));
}

IType*		Complex::operator*(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	throw ERROR_UNEXPECTED;
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
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational / other);
}

Polynomial*	Complex::operator/(const Polynomial &other) const
{
	return (Polynomial(other.getName(), new Rational(0), new Rational(0), this->clone()) / other);
}

IType*		Complex::operator/(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	throw ERROR_UNEXPECTED;
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
	catch (const LogicError &e)
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
	catch(const LogicError &e)
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
	catch(const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other);
}

Polynomial*	Complex::operator%(const Polynomial &other) const
{
	return (Polynomial(other.getName(), new Rational(0), new Rational(0), this->clone()) % other);
}

IType*		Complex::operator%(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	throw ERROR_UNEXPECTED;
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
	catch (const LogicError &e)
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
	catch (const LogicError &e)
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
	catch (const LogicError &e)
	{
		throw ERROR_EXPONENT_INTEGER;
	}
	return (rational ^ other);
}

Polynomial*	Complex::operator^(const Polynomial &other) const
{
	return (Polynomial(other.getName(), new Rational(0), new Rational(0), this->clone()) ^ other);
}

IType*		Complex::operator^(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_rational;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this ^ *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this ^ *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this ^ *other_polynomial);
	throw ERROR_UNEXPECTED;
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

IType*			Complex::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Complex::clone(void) const
{
	return (new Complex(*this));
}

std::ostream&	Complex::print(std::ostream &os) const
{
	if (!this->_real && !this->_imaginary)
		return (os << "0");
	else if (this->_real < Rational(0) && this->_imaginary > Rational(0))
	{
		print_value(os, this->_imaginary, "i", true);
		print_value(os, this->_real, "", false);
	}
	else
	{
		print_value(os, this->_real, "", true);
		print_value(os, this->_imaginary, "i", !this->_real);
	}
	return (os);
}

std::ostream&	Complex::print_polynomial(std::ostream &os, const std::string &var) const
{
	if (!this->_imaginary)
		return (this->_real.print_polynomial(os, var));
	if (!this->_real)
		return (os << *this << " * " << var);
	return (os << "(" << *this << ")" << var);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num)
{
	return (num.print(os));
}
