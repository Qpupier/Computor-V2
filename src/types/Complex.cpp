/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 20:00:17 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils
static AST	*complex_to_node(const Complex &complex)
{
	AST	*result;
	AST	*imaginary;

	result = new AST("+", Token::E_OPERATOR);
	imaginary = new AST("*", Token::E_OPERATOR);
	imaginary->setLeft(new AST(std::to_string(complex.get_imaginary()), Token::E_NUMBER));
	imaginary->setRight(new AST("i", Token::E_IMAGINARY));
	result->setLeft(imaginary);
	result->setRight(new AST(std::to_string(complex.get_real()), Token::E_NUMBER));
	return (result);
}

static bool	number_in_z(const Rational &num)
{
	return (num.get_value() == static_cast<int>(num.get_value()));
}


// Operator overloads
Complex	&Complex::operator=(const Complex &other)
{
	if (this != &other)
	{
		this->_imaginary = other._imaginary;
		this->_real = other._real;
	}
	return (*this);
}

AST	*Complex::operator+(const Complex &other) const
{
	return (complex_to_node(Complex(this->_imaginary + other._imaginary, this->_real + other._real)));
}

AST	*Complex::operator+(const Rational &other) const
{
	return (complex_to_node(Complex(this->_imaginary, this->_real + other.get_value())));
}

AST	*Complex::operator+(const IType &other) const
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

AST	*Complex::operator-(const Complex &other) const
{
	return (complex_to_node(Complex(this->_imaginary - other._imaginary, this->_real - other._real)));
}

AST	*Complex::operator-(const Rational &other) const
{
	return (complex_to_node(Complex(this->_imaginary, this->_real - other.get_value())));
}

AST	*Complex::operator-(const IType &other) const
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

AST	*Complex::operator*(const Complex &other) const
{
	return (complex_to_node(Complex(this->_imaginary * other._real + this->_real * other._imaginary, this->_real * other._real - this->_imaginary * other._imaginary)));
}

AST	*Complex::operator*(const Rational &other) const
{
	return (complex_to_node(Complex(this->_imaginary * other.get_value(), this->_real * other.get_value())));
}

AST	*Complex::operator*(const IType &other) const
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

AST	*Complex::operator/(const Complex &other) const
{
	double	denominator = other._real * other._real + other._imaginary * other._imaginary;

	if (denominator == 0)
		throw std::runtime_error("Division by zero");
	return (complex_to_node(Complex((this->_imaginary * other._real - this->_real * other._imaginary) / denominator, (this->_real * other._real + this->_imaginary * other._imaginary) / denominator)));
}

AST	*Complex::operator/(const Rational &other) const
{
	if (other.get_value() == 0)
		throw std::runtime_error("Division by zero");
	return (complex_to_node(Complex(this->_imaginary / other.get_value(), this->_real / other.get_value())));
}

AST	*Complex::operator/(const IType &other) const
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

AST	*Complex::operator%(const IType &other) const
{
	throw std::logic_error("Modulo operator (%) cannot be applied to complex numbers");
	(void)other;
	return (nullptr);
}

AST	*Complex::operator^(const Rational &other) const
{
	if (!number_in_z(other))
		throw std::logic_error("Only integers (ℤ) can be used as exponents");
	// TODO: Implement exponentiation for complex numbers
	return (nullptr);
}

AST	*Complex::operator^(const IType &other) const
{
	const Complex	*other_complex;
	const Rational	*other_number;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		throw std::logic_error("A complex number cannot be a power");
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
AST*	Complex::matrix_operator(const IType &other) const
{
	throw std::logic_error("Matrix operator (**) cannot be applied to complex numbers");
	(void)other;
	return (nullptr);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num)
{
	os << "Complex: " << num.get_imaginary() << "i" << " + " << num.get_real() << std::endl;
	return (os);
}
