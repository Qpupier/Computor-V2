/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 18:10:41 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils
// static AST	*complex_to_node(const Complex &complex)
// {
// 	AST	*result;
// 	AST	*imaginary;

// 	result = new AST("+", Token::E_OPERATOR);
// 	imaginary = new AST("*", Token::E_OPERATOR);
// 	imaginary->setLeft(new AST(std::to_string(complex.get_imaginary()), Token::E_NUMBER));
// 	imaginary->setRight(new AST("i", Token::E_IMAGINARY));
// 	result->setLeft(imaginary);
// 	result->setRight(new AST(std::to_string(complex.get_real()), Token::E_NUMBER));
// 	return (result);
// }

// static bool	number_in_z(const Rational &num)
// {
// 	return (num.get_value() == static_cast<int>(num.get_value()));
// }


// Operator overloads
bool	Complex::operator!() const
{
	return (!this->_real && !this->_imaginary);
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
	const Rational	*other_number;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_number = dynamic_cast<const Rational*>(&other);
	if (other_number)
		return (*this + *other_number);
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
	const Rational	*other_number;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_number = dynamic_cast<const Rational*>(&other);
	if (other_number)
		return (*this - *other_number);
	return (nullptr);
}

Complex*	Complex::operator*(const Complex &other) const
{
	Rational*	part1;
	Rational*	part2;
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	part1 = this->_imaginary * other._real;
	part2 = this->_real * other._imaginary;
	real = *part1 + *part2;
	delete part1;
	delete part2;
	part1 = this->_real * other._real;
	part2 = this->_imaginary * other._imaginary;
	imaginary = *part1 - *part2;
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
	const Rational	*other_number;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_number = dynamic_cast<const Rational*>(&other);
	if (other_number)
		return (*this * *other_number);
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
	const Rational	*other_number;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_number = dynamic_cast<const Rational*>(&other);
	if (other_number)
		return (*this / *other_number);
	return (nullptr);
}

IType*	Complex::operator%(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_number;
	Rational		*test;

	*test = this;
	if (test)
	{
		other_complex = dynamic_cast<const Complex*>(&other);
		if (other_complex)
		{
			*test = other_complex;
			if (test)
			{
				delete other_complex;
				return (*this ^ *test);
			}
		}
		other_number = dynamic_cast<const Rational*>(&other);
		if (other_number)
			return (*this ^ *other_number);
	}
	throw std::logic_error("Modulo operator (%) cannot be applied to complex numbers");
	return (nullptr);
}

Complex*	Complex::operator^(const Rational &other) const
{
	Rational	exponent;
	Complex*	result;

	exponent = other;
	exponent.reduce();
	if (!exponent.is_integer())
		throw std::logic_error("Only integers (ℤ) can be used as exponents");
	result = new Complex(*this);
	for (int i = 1; i < exponent.get_numerator(); i++)
	{
		Complex*	tmp;

		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

IType*	Complex::operator^(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_number;
	Rational		*test;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
	{
		*test = other_complex;
		if (!test)
			throw std::logic_error("A complex number cannot be a power");
		delete other_complex;
		return (*this ^ *test);
	}
	other_number = dynamic_cast<const Rational*>(&other);
	if (other_number)
		return (*this ^ *other_number);
	return (nullptr);
}


// Getters
Rational	Complex::get_imaginary(void) const
{
	return (this->_imaginary);
}

Rational	Complex::get_real(void) const
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

std::ostream	&Complex::print(std::ostream &os) const
{
	return (os << "Complex: " << this->_real << " + " << this->_imaginary << "i");
}

IType*	Complex::clone(void) const
{
	return (new Complex(*this));
}


// // Output stream operator overload
// std::ostream	&operator<<(std::ostream &os, const Complex &num)
// {
// 	return (num.print(os) << std::endl);
// }
