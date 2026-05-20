/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteDouble.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:46:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/20 22:21:08 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "InfiniteDouble.hpp"

// Utils
static void	remove_decimal_part_in_divisor(InfiniteDouble & dividend, 	\
		InfiniteDouble & divisor)
{
	while (divisor.getDecimalPart())
	{
		dividend *= InfiniteDouble(10);
		divisor *= InfiniteDouble(10);
	}
}

static bool	is_division_infinite(const InfiniteInt & divisor)
{
	InfiniteInt	divisor_copy(divisor);

	while (divisor_copy % 2 == 0)
		divisor_copy /= InfiniteInt(2);
	while (divisor_copy % 5 == 0)
		divisor_copy /= InfiniteInt(5);
	return (divisor_copy == InfiniteInt(1));
}

static bool	division_next_digit(const InfiniteDouble & dividend, 	\
		InfiniteInt & tmp_dividend, 								\
		std::vector<unsigned char>::size_type & nb_integer, 		\
		std::vector<unsigned char>::size_type & nb_decimal)
{
	if (nb_integer < dividend.getIntegerPart().size())
	{
		tmp_dividend.push_back(	\
				dividend.getIntegerPart().getDigits()[nb_integer]);
		return (true);
	}
	if (nb_decimal < dividend.getDecimalPart().size())
	{
		tmp_dividend.push_back(	\
				dividend.getDecimalPart().getDigits()[nb_decimal]);
		if (!nb_decimal++)
			return (true);
	}
	else
		tmp_dividend.push_back(0);
	return (false);
}

static void	division_loop(const InfiniteDouble & dividend, 	\
		const InfiniteInt & divisor, 						\
		std::vector<unsigned char> & result_integer, 		\
		std::vector<unsigned char> & result_decimal)
{
	InfiniteInt								tmp_dividend;
	InfiniteInt								tmp_result;
	std::vector<unsigned char>::size_type	nb_integer(0);
	std::vector<unsigned char>::size_type	nb_decimal(0);
	unsigned char							result_digit;

	while (result_decimal.size() < InfiniteDouble::MAX_PRECISION)
	{
		tmp_result = tmp_dividend / divisor;
		result_digit = tmp_result.getDigits().empty() 	\
				? 0 : tmp_result.getDigits()[0];
		tmp_dividend -= tmp_result * divisor;
		if (division_next_digit(dividend, tmp_dividend, nb_integer, nb_decimal))
			result_integer.push_back(result_digit);
		else
			result_decimal.push_back(result_digit);
		if (!tmp_dividend)
			break ;
		nb_integer++;
	}
}


// Constructors
InfiniteDouble::InfiniteDouble(const InfiniteInt &integer_part, 		\
		const InfiniteInt &decimal_part, bool is_negative, 				\
		bool is_decimal_infinite):										\
			_integer_part(integer_part), _decimal_part(decimal_part), 	\
			_isDecimalInfinite(is_decimal_infinite), 					\
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
		this->_isDecimalInfinite = other._isDecimalInfinite;
		this->_isNegative = other._isNegative;
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

InfiniteDouble	InfiniteDouble::operator/(const InfiniteDouble &other) const
{
	InfiniteDouble				dividend(*this);
	InfiniteDouble				divisor(other);
	std::vector<unsigned char>	result_integer;
	std::vector<unsigned char>	result_decimal;
	bool						infinite;

	remove_decimal_part_in_divisor(dividend, divisor);
	division_loop(dividend, divisor.getIntegerPart(), result_integer, 	\
			result_decimal);
	return (InfiniteDouble(InfiniteInt(result_integer), 				\
			InfiniteInt(result_decimal, false, false), 					\
			this->_isNegative != other._isNegative));
}

void			InfiniteDouble::operator/=(const InfiniteDouble &other)
{
	*this = *this / other;
}

InfiniteDouble	InfiniteDouble::operator%(const InfiniteDouble &other) const
{
	InfiniteDouble	division(*this / other);

	return (*this - InfiniteDouble(division.getIntegerPart()) * other);
}

void			InfiniteDouble::operator%=(const InfiniteDouble &other)
{
	*this = *this % other;
}

InfiniteDouble	InfiniteDouble::operator^(const InfiniteDouble &other) const
{
	InfiniteDouble	result(1);
	InfiniteDouble	power(other.getIntegerPart());

	if (other.getDecimalPart() || other.getIsNegative())
		throw std::runtime_error("Exponentiation with a non-integer exponent is not supported.");//TODO: remplacer avec la bonne
	while (power--)
		result *= *this;
	return (result);
}

void			InfiniteDouble::operator^=(const InfiniteDouble &other)
{
	*this = *this ^ other;
}


// Getters
InfiniteInt	InfiniteDouble::getDecimalPart(void) const
{
	return (this->_decimal_part);
}

InfiniteInt	InfiniteDouble::getIntegerPart(void) const
{
	return (this->_integer_part);
}

bool		InfiniteDouble::getIsDecimalInfinite(void) const
{
	return (this->_isDecimalInfinite);
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

void	InfiniteDouble::setIsDecimalInfinite(bool is_decimal_infinite)
{
	this->_isDecimalInfinite = is_decimal_infinite;
}

void	InfiniteDouble::setIsNegative(bool is_negative)
{
	this->_isNegative = is_negative;
}


// Methods
void	InfiniteDouble::push_back_decimal(unsigned char digit)
{
	this->_decimal_part.push_back(digit);
}

void	InfiniteDouble::push_back_integer(unsigned char digit)
{
	this->_integer_part.push_back(digit);
}

void	InfiniteDouble::reduce(void)
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
