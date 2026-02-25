/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Number.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:38:37 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/25 14:45:32 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Number.hpp"
// #include "Imaginary.hpp"

// Operator overloads
Number	&Number::operator=(const Number &other)
{
	if (this != &other)
		this->_value = other._value;
	return (*this);
}

Node	*Number::operator+(const Number &other) const
{
	return (new Node(std::to_string(this->_value + other._value), Token::E_NUMBER));
}

Node	*Number::operator+(const IType &other) const
{
	const Number *other_number = dynamic_cast<const Number*>(&other);

	if (other_number)
		return (*this + *other_number);
	return (nullptr);
}

Node	*Number::operator-(const Number &other) const
{
	return (new Node(std::to_string(this->_value - other._value), Token::E_NUMBER));
}

Node	*Number::operator-(const IType &other) const
{
	const Number *other_number = dynamic_cast<const Number*>(&other);

	if (other_number)
		return (*this - *other_number);
	return (nullptr);
}

Node	*Number::operator*(const Number &other) const
{
	return (new Node(std::to_string(this->_value * other._value), Token::E_NUMBER));
}

Node	*Number::operator*(const Imaginary &other) const
{
	return (other * *this);
}

Node	*Number::operator*(const IType &other) const
{
	const Number	*other_number;
	const Imaginary	*other_Imaginary;

	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this * *other_number);
	other_Imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_Imaginary)
		return (*this * *other_Imaginary);
	return (nullptr);
}

Node	*Number::operator/(const Number &other) const
{
	if (other._value == 0)
		throw std::runtime_error("Division by zero");
	return (new Node(std::to_string(this->_value / other._value), Token::E_NUMBER));
}

Node	*Number::operator/(const Imaginary &other) const
{
	Node	*result;

	result = new Node("/", Token::E_OPERATOR);
	result->setLeft(new Node(std::to_string(this->_value), Token::E_NUMBER));
	result->setRight(new Node("i", Token::E_IMAGINARY));
	(void)other;
	return (result);
}

Node	*Number::operator/(const IType &other) const
{
	const Number	*other_number;
	const Imaginary	*other_Imaginary;

	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this / *other_number);
	other_Imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_Imaginary)
		return (*this / *other_Imaginary);
	return (nullptr);
}

Node	*Number::operator%(const Number &other) const
{
	if (other._value == 0)
		throw std::runtime_error("Modulo by zero");
	return (new Node(std::to_string(std::fmod(this->_value, other._value)), Token::E_NUMBER));
}

Node	*Number::operator%(const IType &other) const
{
	const Number	*other_number;
	const Imaginary	*other_Imaginary;

	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this % *other_number);
	other_Imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_Imaginary)
		throw std::logic_error("Modulo operator (%) cannot be applied to Imaginary numbers");
	return (nullptr);
}

Node	*Number::operator^(const Number &other) const
{
	return (new Node(std::to_string(std::pow(this->_value, other._value)), Token::E_NUMBER));
}

Node	*Number::operator^(const IType &other) const
{
	const Number	*other_number;
	const Imaginary	*other_Imaginary;

	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this ^ *other_number);
	other_Imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_Imaginary)
		throw std::logic_error("A Imaginary number cannot be a power");
	return (nullptr);
}


// Getters
double	Number::get_value(void) const
{
	return (this->_value);
}


// Methods
Node*	Number::matrix_operator(const IType &other) const
{
	throw std::logic_error("Matrix operator (**) cannot be applied to numbers");
	(void)other;
	return (nullptr);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Number &num)
{
	os << "Number: " << num.get_value() << std::endl;
	return (os);
}
