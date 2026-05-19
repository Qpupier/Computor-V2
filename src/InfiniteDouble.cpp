/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteDouble.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:46:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/19 17:26:34 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "InfiniteDouble.hpp"

// Constructors
InfiniteDouble::InfiniteDouble(const InfiniteInt &integer_part, 		\
		const InfiniteInt &decimal_part, bool is_negative):				\
			_integer_part(integer_part), _decimal_part(decimal_part), 	\
			_isNegative(is_negative)
{
	this->_integer_part.setIsIntegerPart(true);
	this->_decimal_part.setIsIntegerPart(false);
	if (integer_part.getIsNegative())
	{
		this->_integer_part.setIsNegative(false);
		this->_isNegative = !this->_isNegative;
	}
	this->reduce();
}


// Operator overloads
InfiniteDouble::operator bool() const
{
	return (this->getIntegerPart() || this->getDecimalPart());
}

InfiniteDouble&	InfiniteDouble::operator=(const InfiniteDouble &other)
{
	if (this != &other)
	{
		this->_integer_part = other._integer_part;
		this->_decimal_part = other._decimal_part;
	}
	return (*this);
}

bool			InfiniteDouble::operator==(const InfiniteDouble &other) const
{
	return (this->getIntegerPart() == other.getIntegerPart() 	\
			&& this->getDecimalPart() == other.getDecimalPart());
}

bool			InfiniteDouble::operator!=(const InfiniteDouble&other) const
{
	return (!(*this == other));
}

bool			InfiniteDouble::operator<(const InfiniteDouble &other) const
{
	return (this->getIntegerPart() < other.getIntegerPart() 		\
			|| (this->getIntegerPart() == other.getIntegerPart() 	\
				&& this->getDecimalPart() < other.getDecimalPart()));
}

bool			InfiniteDouble::operator<=(const InfiniteDouble &other) const
{
	return (*this < other || *this == other);
}

bool			InfiniteDouble::operator>(const InfiniteDouble &other) const
{
	return (!(*this <= other));
}

bool			InfiniteDouble::operator>=(const InfiniteDouble &other) const
{
	return (!(*this < other));
}

InfiniteDouble	InfiniteDouble::operator+(const InfiniteDouble &other) const
{
	std::vector<unsigned char>				a						\
			(this->getIntegerPart().getDigits());
	std::vector<unsigned char>				b						\
			(other.getIntegerPart().getDigits());
	std::vector<unsigned char>				this_decimal_digits		\
			(this->getDecimalPart().getDigits());
	std::vector<unsigned char>				other_decimal_digits	\
			(other.getDecimalPart().getDigits());
	std::vector<unsigned char>				integer_digits_sub;
	std::vector<unsigned char>::size_type	decimal_size			\
			(std::max(this->getDecimalPart().size(), 				\
				other.getDecimalPart().size()));
	InfiniteInt								int_result;
	InfiniteDouble							result;

	if (this->_isNegative && !other._isNegative)
		return (other - (-*this));
	if (!this->_isNegative && other._isNegative)
		return (*this - (-other));
	while (this_decimal_digits.size() < decimal_size)
		this_decimal_digits.push_back(0);
	while (other_decimal_digits.size() < decimal_size)
		other_decimal_digits.push_back(0);
	a.insert(a.end(), this_decimal_digits.begin(), this_decimal_digits.end());
	b.insert(b.end(), other_decimal_digits.begin(), other_decimal_digits.end());
	int_result = InfiniteInt(a) + InfiniteInt(b);
	integer_digits_sub = int_result.getDigits();
	result.setIntegerPart(InfiniteInt(std::vector<unsigned char>	\
			(integer_digits_sub.begin(), 							\
				integer_digits_sub.end() - decimal_size)));
	std::vector<unsigned char> test(integer_digits_sub.end() - 		\
			decimal_size, integer_digits_sub.end());
	result.setDecimalPart(InfiniteInt(test, false, false));
	result.setIsNegative(false);
	result.reduce();
	return (result);
}

void			InfiniteDouble::operator+=(const InfiniteDouble &other)
{
	*this = *this + other;
}

InfiniteDouble&	InfiniteDouble::operator++(void)
{
	*this += InfiniteDouble(1);
	return (*this);
}

InfiniteDouble	InfiniteDouble::operator++(int)
{
	InfiniteDouble	result(*this);

	*this += InfiniteDouble(1);
	return (result);
}

InfiniteDouble	InfiniteDouble::operator-(void) const
{
	InfiniteDouble	result;

	result = *this;
	if (result)
		result._isNegative = !this->_isNegative;
	return (result);
}

InfiniteDouble	InfiniteDouble::operator-(const InfiniteDouble &other) const
{
	std::vector<unsigned char>				a						\
			(this->getIntegerPart().getDigits());
	std::vector<unsigned char>				b						\
			(other.getIntegerPart().getDigits());
	std::vector<unsigned char>				this_decimal_digits		\
			(this->getDecimalPart().getDigits());
	std::vector<unsigned char>				other_decimal_digits	\
			(other.getDecimalPart().getDigits());
	std::vector<unsigned char>				integer_digits_sub;
	std::vector<unsigned char>::size_type	decimal_size			\
			(std::max(this->getDecimalPart().size(), 				\
				other.getDecimalPart().size()));
	InfiniteInt								int_result;
	InfiniteDouble							result;

	if (this->_isNegative && other._isNegative)
		return (-other - (-*this));
	if (this->_isNegative != other._isNegative)
		return (*this + (-other));
	if (*this < other)
		return (-(other - *this));
	while (this_decimal_digits.size() < decimal_size)
		this_decimal_digits.push_back(0);
	while (other_decimal_digits.size() < decimal_size)
		other_decimal_digits.push_back(0);
	a.insert(a.end(), this_decimal_digits.begin(), this_decimal_digits.end());
	b.insert(b.end(), other_decimal_digits.begin(), other_decimal_digits.end());
	int_result = InfiniteInt(a) - InfiniteInt(b);
	integer_digits_sub = int_result.getDigits();
	result.setIntegerPart(InfiniteInt(std::vector<unsigned char>	\
			(integer_digits_sub.begin(), 							\
				integer_digits_sub.end() - decimal_size)));
	std::vector<unsigned char> test(integer_digits_sub.end() - 		\
			decimal_size, integer_digits_sub.end());
	result.setDecimalPart(InfiniteInt(test, false, false));
	result.setIsNegative(false);
	result.reduce();
	return (result);
}

void			InfiniteDouble::operator-=(const InfiniteDouble &other)
{
	*this = *this - other;
}

InfiniteDouble&	InfiniteDouble::operator--(void)
{
	*this -= InfiniteDouble(1);
	return (*this);
}

InfiniteDouble	InfiniteDouble::operator--(int)
{
	InfiniteDouble	result(*this);

	*this -= InfiniteDouble(1);
	return (result);
}

InfiniteDouble	InfiniteDouble::operator*(const InfiniteDouble &other) const
{
	std::vector<unsigned char>				a						\
			(this->getIntegerPart().getDigits());
	std::vector<unsigned char>				b						\
			(other.getIntegerPart().getDigits());
	std::vector<unsigned char>				this_decimal_digits		\
			(this->getDecimalPart().getDigits());
	std::vector<unsigned char>				other_decimal_digits	\
			(other.getDecimalPart().getDigits());
	std::vector<unsigned char>::size_type	decimal_size			\
			(this->getDecimalPart().size() + other.getDecimalPart().size());
	std::vector<unsigned char>				integer_digits_sum;
	InfiniteInt								int_result;
	InfiniteDouble							result;

	a.insert(a.end(), this_decimal_digits.begin(), this_decimal_digits.end());
	b.insert(b.end(), other_decimal_digits.begin(), other_decimal_digits.end());
	int_result = InfiniteInt(a) * InfiniteInt(b);
	integer_digits_sum = int_result.getDigits();
	result.setIntegerPart(InfiniteInt(std::vector<unsigned char>	\
			(integer_digits_sum.begin(), integer_digits_sum.end() - decimal_size)));
	result.setDecimalPart(InfiniteInt(std::vector<unsigned char>(integer_digits_sum.end() - decimal_size, integer_digits_sum.end()), false, false));
	result.setIsNegative(this->_isNegative != other._isNegative);
	result.reduce();
	return (result);
}

void			InfiniteDouble::operator*=(const InfiniteDouble &other)
{
	*this = *this * other;
}

// InfiniteDouble	InfiniteDouble::operator/(const InfiniteDouble &other) const
// {
// 	InfiniteDouble	dividend(*this);
// 	InfiniteDouble	result;

// 	while (!dividend.getDigits().empty())
// 		if (!division(dividend, other, result))
// 			break ;
// 	result.setIsNegative(this->_isNegative != other._isNegative);
// 	result.reduce();
// 	return (result);
// }

// void			InfiniteDouble::operator/=(const InfiniteDouble &other)
// {
// 	*this = *this / other;
// }

// InfiniteDouble	InfiniteDouble::operator%(const InfiniteDouble &other) const
// {
// 	return (*this - *this / other * other);
// }

// void			InfiniteDouble::operator%=(const InfiniteDouble &other)
// {
// 	*this = *this % other;
// }

// InfiniteDouble	InfiniteDouble::operator^(const InfiniteDouble &other) const
// {
// 	InfiniteDouble	result(std::vector<unsigned char>(1, 1));
// 	InfiniteDouble	exp(other);

// 	while (exp > InfiniteDouble())
// 	{
// 		result *= *this;
// 		exp -= InfiniteDouble(1);
// 	}
// 	return (result);
// }

// void			InfiniteDouble::operator^=(const InfiniteDouble &other)
// {
// 	*this = *this ^ other;
// }


// Getters
InfiniteInt	InfiniteDouble::getDecimalPart(void) const
{
	return (this->_decimal_part);
}

InfiniteInt	InfiniteDouble::getIntegerPart(void) const
{
	return (this->_integer_part);
}

bool		InfiniteDouble::getIsNegative(void) const
{
	return (this->_isNegative);
}


// Setters
void	InfiniteDouble::setDecimalPart(const InfiniteInt &decimal_part)
{
	this->_decimal_part = decimal_part;
}

void	InfiniteDouble::setIntegerPart(const InfiniteInt &integer_part)
{
	this->_integer_part = integer_part;
}

void	InfiniteDouble::setIsNegative(bool is_negative)
{
	this->_isNegative = is_negative;
}


// Methods
void		InfiniteDouble::reduce(void)
{
	this->_integer_part.reduce();
	this->_decimal_part.reduce();
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteDouble &num)
{
	if (num.getIsNegative())
		os << "-";
	os << num.getIntegerPart();
	if (num.getDecimalPart())
		os << "." << num.getDecimalPart();
	return (os);
}
