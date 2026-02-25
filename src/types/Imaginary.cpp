/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Imaginary.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:25:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/25 14:46:07 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// #include "Number.hpp"
// #include "Imaginary.hpp"

// Utils
static bool	number_in_z(const Number &num)
{
	return (num.get_value() == static_cast<int>(num.get_value()));
}


// Operator overloads
Imaginary	&Imaginary::operator=(const Imaginary &other)
{
	(void)other;
	return (*this);
}

Node	*Imaginary::operator+(const Imaginary &other) const
{
	Node	*result;

	result = new Node("*", Token::E_OPERATOR);
	result->setLeft(new Node("2", Token::E_NUMBER));
	result->setRight(new Node("i", Token::E_IMAGINARY));
	(void)other;
	return (result);
}

Node	*Imaginary::operator+(const Number &other) const
{
	Node	*result;

	result = new Node("+", Token::E_OPERATOR);
	result->setLeft(new Node("i", Token::E_IMAGINARY));
	result->setRight(new Node(std::to_string(other.get_value()), Token::E_NUMBER));
	return (result);
}

Node	*Imaginary::operator+(const IType &other) const
{
	const Imaginary	*other_imaginary;
	const Number	*other_number;

	other_imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_imaginary)
		return (*this + *other_imaginary);
	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this + *other_number);
	return (nullptr);
}

Node	*Imaginary::operator-(const Imaginary &other) const
{
	(void)other;
	return (new Node("0", Token::E_NUMBER));
}

Node	*Imaginary::operator-(const Number &other) const
{
	Node	*result;

	result = new Node("-", Token::E_OPERATOR);
	result->setLeft(new Node("i", Token::E_IMAGINARY));
	result->setRight(new Node(std::to_string(other.get_value()), Token::E_NUMBER));
	return (result);
}

Node	*Imaginary::operator-(const IType &other) const
{
	const Imaginary	*other_imaginary;
	const Number	*other_number;

	other_imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_imaginary)
		return (*this - *other_imaginary);
	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this - *other_number);
	return (nullptr);
}

Node	*Imaginary::operator*(const Imaginary &other) const
{
	(void)other;
	return (new Node("-1", Token::E_NUMBER));
}

Node	*Imaginary::operator*(const Number &other) const
{
	Node	*result;

	result = new Node("*", Token::E_OPERATOR);
	result->setLeft(new Node(std::to_string(other.get_value()), Token::E_NUMBER));
	result->setRight(new Node("i", Token::E_IMAGINARY));
	return (result);
}

Node	*Imaginary::operator*(const IType &other) const
{
	const Imaginary	*other_imaginary;
	const Number	*other_number;

	other_imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_imaginary)
		return (*this * *other_imaginary);
	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this * *other_number);
	return (nullptr);
}

Node	*Imaginary::operator/(const Imaginary &other) const
{
	(void)other;
	return (new Node("1", Token::E_NUMBER));
}

Node	*Imaginary::operator/(const Number &other) const
{
	Node	*result;

	if (other.get_value() == 0)
		throw std::runtime_error("Division by zero");
	result = new Node("*", Token::E_OPERATOR);
	result->setLeft(new Node("i", Token::E_IMAGINARY));
	result->setRight(new Node(std::to_string(other.get_value()), Token::E_NUMBER));
	return (result);
}

Node	*Imaginary::operator/(const IType &other) const
{
	const Imaginary	*other_Imaginary;
	const Number	*other_number;

	other_Imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_Imaginary)
		return (*this / *other_Imaginary);
	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this / *other_number);
	return (nullptr);
}

Node	*Imaginary::operator%(const IType &other) const
{
	throw std::logic_error("Modulo operator (%) cannot be applied to Complex numbers");
	(void)other;
	return (nullptr);
}

Node	*Imaginary::operator^(const Number &other) const
{
	int		exponent;
	int		modulo;
	Node	*node;

	if (!number_in_z(other))
		throw std::logic_error("Only integers (∈ ℤ) can be used as exponents");
	exponent = static_cast<int>(other.get_value());
	if (!exponent)
		return (new Node("1", Token::E_NUMBER));
	modulo = exponent % 4;
	if (!modulo)
		return (new Node("1", Token::E_NUMBER));
	if (modulo == 1)
		return (new Node("i", Token::E_IMAGINARY));
	if (modulo == 2 || modulo == -2)
		return (new Node("-1", Token::E_NUMBER));
	if (modulo == 3)
	{
		node = new Node("*", Token::E_OPERATOR);
		node->setLeft(new Node("-1", Token::E_NUMBER));
		node->setRight(new Node("i", Token::E_IMAGINARY));
		return (node);
	}
	if (modulo == -1)
	{
		node = new Node("/", Token::E_OPERATOR);
		node->setLeft(new Node("1", Token::E_NUMBER));
		node->setRight(new Node("i", Token::E_IMAGINARY));
		return (node);
	}
	if (modulo == -3)
	{
		node = new Node("/", Token::E_OPERATOR);
		node->setLeft(new Node("-1", Token::E_NUMBER));
		node->setRight(new Node("i", Token::E_IMAGINARY));
		return (node);
	}
	return (nullptr);
}

Node	*Imaginary::operator^(const IType &other) const
{
	const Imaginary	*other_Imaginary;
	const Number	*other_number;

	other_Imaginary = dynamic_cast<const Imaginary*>(&other);
	if (other_Imaginary)
		throw std::logic_error("A Complex number cannot be a power");
	other_number = dynamic_cast<const Number*>(&other);
	if (other_number)
		return (*this ^ *other_number);
	return (nullptr);
}


// Methods
Node*	Imaginary::matrix_operator(const IType &other) const
{
	throw std::logic_error("Matrix operator (**) cannot be applied to Complex numbers");
	(void)other;
	return (nullptr);
}
