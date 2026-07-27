/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteFloat.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:46:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/27 17:21:33 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "InfiniteFloat.hpp"

// Utils

static void				align_sizes(						\
		std::vector<unsigned char> & this_decimal_digits, 	\
		std::vector<unsigned char> & other_decimal_digits, 	\
		const std::vector<unsigned char>::size_type decimal_size)
{
	while (this_decimal_digits.size() < decimal_size)
		this_decimal_digits.push_back(0);
	while (other_decimal_digits.size() < decimal_size)
		other_decimal_digits.push_back(0);
}

static void				concat_parts(std::vector<unsigned char> & a, 	\
		std::vector<unsigned char> & b, 								\
		const std::vector<unsigned char> & this_decimal_digits, 		\
		const std::vector<unsigned char> & other_decimal_digits)
{
	a.insert(a.end(), this_decimal_digits.begin(), this_decimal_digits.end());
	b.insert(b.end(), other_decimal_digits.begin(), other_decimal_digits.end());
}

static InfiniteInt		add_integer_parts(const InfiniteFloat & tmp_a, 	\
		const InfiniteFloat & tmp_b, 										\
		const std::vector<unsigned char>::size_type & decimal_size)
{
	std::vector<unsigned char>	a(tmp_a.getIntegerPart().getDigits());
	std::vector<unsigned char>	b(tmp_b.getIntegerPart().getDigits());
	std::vector<unsigned char>	this_decimal_digits		\
			(tmp_a.getDecimalPart().getDigits());
	std::vector<unsigned char>	other_decimal_digits	\
			(tmp_b.getDecimalPart().getDigits());

	align_sizes(this_decimal_digits, other_decimal_digits, decimal_size);
	concat_parts(a, b, this_decimal_digits, other_decimal_digits);
	return (InfiniteInt(a) + InfiniteInt(b));
}

static InfiniteInt		sub_integer_parts(const InfiniteFloat & tmp_a, 	\
		const InfiniteFloat & tmp_b, 										\
		const std::vector<unsigned char>::size_type & decimal_size)
{
	std::vector<unsigned char>	a(tmp_a.getIntegerPart().getDigits());
	std::vector<unsigned char>	b(tmp_b.getIntegerPart().getDigits());
	std::vector<unsigned char>	this_decimal_digits		\
			(tmp_a.getDecimalPart().getDigits());
	std::vector<unsigned char>	other_decimal_digits	\
			(tmp_b.getDecimalPart().getDigits());

	align_sizes(this_decimal_digits, other_decimal_digits, decimal_size);
	concat_parts(a, b, this_decimal_digits, other_decimal_digits);
	return (InfiniteInt(a) - InfiniteInt(b));
}

static InfiniteInt		mul_integer_parts(const InfiniteFloat & tmp_a, 	\
		const InfiniteFloat & tmp_b)
{
	std::vector<unsigned char>	a(tmp_a.getIntegerPart().getDigits());
	std::vector<unsigned char>	b(tmp_b.getIntegerPart().getDigits());
	std::vector<unsigned char>	this_decimal_digits		\
			(tmp_a.getDecimalPart().getDigits());
	std::vector<unsigned char>	other_decimal_digits	\
			(tmp_b.getDecimalPart().getDigits());

	concat_parts(a, b, this_decimal_digits, other_decimal_digits);
	return (InfiniteInt(a) * InfiniteInt(b));
}

static InfiniteFloat	place_floating_point(							\
		const InfiniteInt & int_result, 								\
		const std::vector<unsigned char>::size_type & decimal_size, 	\
		const bool is_negative)
{
	InfiniteFloat							result;
	std::vector<unsigned char>				integer_digits_sub;
	std::vector<unsigned char>				vector_integer;
	std::vector<unsigned char>				vector_decimal;
	std::vector<unsigned char>::iterator	floating_point_position;

	integer_digits_sub = int_result.getDigits();
	while (integer_digits_sub.size() <= decimal_size)
		integer_digits_sub.insert(integer_digits_sub.begin(), 0);
	floating_point_position = integer_digits_sub.end() 					\
			- static_cast<std::vector<unsigned char>::difference_type>(	\
				decimal_size);
	vector_integer = std::vector<unsigned char>(						\
			integer_digits_sub.begin(), floating_point_position);
	vector_decimal = std::vector<unsigned char>(						\
			floating_point_position, integer_digits_sub.end());
	result = InfiniteFloat(InfiniteInt(vector_integer), InfiniteInt(vector_decimal, false, false), is_negative);
	result.reduce();
	return (result);
}

static void				remove_decimal_part_in_divisor(	\
		InfiniteFloat & dividend, InfiniteFloat & divisor)
{
	while (divisor.getDecimalPart())
	{
		dividend *= InfiniteFloat(10);
		divisor *= InfiniteFloat(10);
	}
}

static bool				division_next_digit(							\
		const InfiniteFloat & dividend, InfiniteInt & tmp_dividend, 	\
		std::vector<unsigned char>::size_type & nb_integer, 			\
		std::vector<unsigned char>::size_type & nb_decimal)
{
	if (nb_integer < dividend.getIntegerPart().size())
	{
		tmp_dividend.push_back(	\
				dividend.getIntegerPart().getDigits()[nb_integer]);
		return (true);
	}
	if (nb_decimal < dividend.getDecimalPart().size())
		tmp_dividend.push_back(	\
				dividend.getDecimalPart().getDigits()[nb_decimal]);
	else
		tmp_dividend.push_back(0);
	if (!nb_decimal++)
			return (true);
	return (false);
}

static void				division_loop(const InfiniteFloat & dividend, 	\
		const InfiniteInt & divisor, 									\
		std::vector<unsigned char> & result_integer, 					\
		std::vector<unsigned char> & result_decimal)
{
	InfiniteInt								tmp_dividend;
	InfiniteInt								tmp_result;
	std::vector<unsigned char>::size_type	nb_integer(0);
	std::vector<unsigned char>::size_type	nb_decimal(0);
	unsigned char							result_digit;

	while (result_decimal.size() < InfiniteFloat::MAX_PRECISION)
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

static void				insert_new_digit(const InfiniteFloat num, 	\
		InfiniteFloat & padding, 						\
		std::vector<unsigned char> & result_integer, 	\
		std::vector<unsigned char> & result_decimal)
{
	unsigned char				last_good_digit(0);
	std::vector<unsigned char>	test;

	for (unsigned char test_digit = 1; test_digit < 10; test_digit++)
	{
		test = result_integer;
		test.insert(test.end(), result_decimal.begin(), result_decimal.end());
		test.push_back(test_digit);
		if (((InfiniteFloat(test) * padding) ^ InfiniteFloat(2)) > num)
			break ;
		last_good_digit = test_digit;
	}
	if (padding >= InfiniteFloat(1))
		result_integer.push_back(last_good_digit);
	else
		result_decimal.push_back(last_good_digit);
	padding /= InfiniteFloat(10);
}


// Constructors

InfiniteFloat::InfiniteFloat(const InfiniteInt integer_part, 			\
		const InfiniteInt decimal_part, const bool is_negative): 		\
			_integer_part(integer_part), _decimal_part(decimal_part), 	\
			_isNegative(is_negative)
{
	this->_integer_part.setIsIntegerPart(true);
	this->_integer_part.setIsNegative(false);
	this->_decimal_part.setIsIntegerPart(false);
	this->_decimal_part.setIsNegative(false);
	this->reduce();
}


// Operator overloads

InfiniteFloat::operator bool() const
{
	return (this->getIntegerPart() || this->getDecimalPart());
}

InfiniteFloat&	InfiniteFloat::operator=(const InfiniteFloat &other)
{
	if (this != &other)
	{
		this->_integer_part = other._integer_part;
		this->_decimal_part = other._decimal_part;
		this->_isNegative = other._isNegative;
	}
	return (*this);
}

InfiniteFloat&	InfiniteFloat::operator=(const InfiniteInt &other)
{
	return (*this = InfiniteFloat(other));
}

InfiniteFloat&	InfiniteFloat::operator=(const long long int value)
{
	return (*this = InfiniteFloat(value));
}

bool			InfiniteFloat::operator==(const InfiniteFloat &other) const
{
	return (this->getIntegerPart() == other.getIntegerPart() 		\
			&& this->getDecimalPart() == other.getDecimalPart() 	\
			&& this->getIsNegative() == other.getIsNegative());
}

bool			InfiniteFloat::operator==(const InfiniteInt &other) const
{
	return (*this == InfiniteFloat(other));
}

bool			InfiniteFloat::operator==(const long long int value) const
{
	return (*this == InfiniteFloat(value));
}

bool			InfiniteFloat::operator!=(const InfiniteFloat&other) const
{
	return (!(*this == other));
}

bool			InfiniteFloat::operator!=(const InfiniteInt&other) const
{
	return (!(*this == other));
}

bool			InfiniteFloat::operator!=(const long long int value) const
{
	return (*this != InfiniteFloat(value));
}

bool			InfiniteFloat::operator<(const InfiniteFloat &other) const
{
	if (this->getIsNegative() != other.getIsNegative())
		return (this->getIsNegative());
	return (this->getIntegerPart() < other.getIntegerPart() 		\
			|| (this->getIntegerPart() == other.getIntegerPart() 	\
				&& this->getDecimalPart() < other.getDecimalPart()));
}

bool			InfiniteFloat::operator<(const InfiniteInt &other) const
{
	return (*this < InfiniteFloat(other));
}

bool			InfiniteFloat::operator<(const long long int value) const
{
	return (*this < InfiniteFloat(value));
}

bool			InfiniteFloat::operator<=(const InfiniteFloat &other) const
{
	return (*this < other || *this == other);
}

bool			InfiniteFloat::operator<=(const InfiniteInt &other) const
{
	return (*this <= InfiniteFloat(other));
}

bool			InfiniteFloat::operator<=(const long long int value) const
{
	return (*this <= InfiniteFloat(value));
}

bool			InfiniteFloat::operator>(const InfiniteFloat &other) const
{
	return (!(*this <= other));
}

bool			InfiniteFloat::operator>(const InfiniteInt &other) const
{
	return (*this > InfiniteFloat(other));
}

bool			InfiniteFloat::operator>(const long long int value) const
{
	return (*this > InfiniteFloat(value));
}

bool			InfiniteFloat::operator>=(const InfiniteFloat &other) const
{
	return (!(*this < other));
}

bool			InfiniteFloat::operator>=(const InfiniteInt &other) const
{
	return (*this >= InfiniteFloat(other));
}

bool			InfiniteFloat::operator>=(const long long int value) const
{
	return (*this >= InfiniteFloat(value));
}

InfiniteFloat	InfiniteFloat::operator+(const InfiniteFloat &other) const
{
	std::vector<unsigned char>::size_type	decimal_size	\
			(std::max(this->getDecimalPart().size(), 		\
				other.getDecimalPart().size()));
	InfiniteInt								int_result;

	if (this->getIsNegative() && !other.getIsNegative())
		return (other - (-*this));
	if (!this->getIsNegative() && other.getIsNegative())
		return (*this - (-other));
	int_result = add_integer_parts(*this, other, decimal_size);
	return (place_floating_point(int_result, decimal_size, 	\
			this->getIsNegative()));
}

InfiniteFloat	InfiniteFloat::operator+(const InfiniteInt &other) const
{
	return (*this + InfiniteFloat(other));
}

InfiniteFloat	InfiniteFloat::operator+(const long long int value) const
{
	return (*this + InfiniteFloat(value));
}

void			InfiniteFloat::operator+=(const InfiniteFloat &other)
{
	*this = *this + other;
}

void			InfiniteFloat::operator+=(const InfiniteInt &other)
{
	*this += InfiniteFloat(other);
}

void			InfiniteFloat::operator+=(const long long int value)
{
	*this += InfiniteFloat(value);
}

InfiniteFloat&	InfiniteFloat::operator++(void)
{
	*this += InfiniteFloat(1);
	return (*this);
}

InfiniteFloat	InfiniteFloat::operator++(int)
{
	InfiniteFloat	result(*this);

	*this += InfiniteFloat(1);
	return (result);
}

InfiniteFloat	InfiniteFloat::operator-(void) const
{
	InfiniteFloat	result;

	result = *this;
	result.setIsNegative(result ? !this->getIsNegative() : false);
	return (result);
}

InfiniteFloat	InfiniteFloat::operator-(const InfiniteFloat &other) const
{
	std::vector<unsigned char>::size_type	decimal_size	\
			(std::max(this->getDecimalPart().size(), 		\
				other.getDecimalPart().size()));
	InfiniteInt								int_result;

	if (this->getIsNegative() && other.getIsNegative())
		return (-other - (-*this));
	if (this->getIsNegative() != other.getIsNegative())
		return (*this + (-other));
	if (*this < other)
		return (-(other - *this));
	int_result = sub_integer_parts(*this, other, decimal_size);
	return (place_floating_point(int_result, decimal_size, false));
}

InfiniteFloat	InfiniteFloat::operator-(const InfiniteInt &other) const
{
	return (*this - InfiniteFloat(other));
}

InfiniteFloat	InfiniteFloat::operator-(const long long int value) const
{
	return (*this - InfiniteFloat(value));
}

void			InfiniteFloat::operator-=(const InfiniteFloat &other)
{
	*this = *this - other;
}

void			InfiniteFloat::operator-=(const InfiniteInt &other)
{
	*this -= InfiniteFloat(other);
}

void			InfiniteFloat::operator-=(const long long int value)
{
	*this -= InfiniteFloat(value);
}

InfiniteFloat&	InfiniteFloat::operator--(void)
{
	*this -= InfiniteFloat(1);
	return (*this);
}

InfiniteFloat	InfiniteFloat::operator--(int)
{
	InfiniteFloat	result(*this);

	*this -= InfiniteFloat(1);
	return (result);
}

InfiniteFloat	InfiniteFloat::operator*(const InfiniteFloat &other) const
{
	std::vector<unsigned char>::size_type	decimal_size	\
			(this->getDecimalPart().size() + other.getDecimalPart().size());
	InfiniteInt								int_result;

	int_result = mul_integer_parts(*this, other);
	return (place_floating_point(int_result, decimal_size, 	\
			this->getIsNegative() != other.getIsNegative()));
}

InfiniteFloat	InfiniteFloat::operator*(const InfiniteInt &other) const
{
	return (*this * InfiniteFloat(other));
}

InfiniteFloat	InfiniteFloat::operator*(const long long int value) const
{
	return (*this * InfiniteFloat(value));
}

void			InfiniteFloat::operator*=(const InfiniteFloat &other)
{
	*this = *this * other;
}

void			InfiniteFloat::operator*=(const InfiniteInt &other)
{
	*this *= InfiniteFloat(other);
}

void			InfiniteFloat::operator*=(const long long int value)
{
	*this *= InfiniteFloat(value);
}

InfiniteFloat	InfiniteFloat::operator/(const InfiniteFloat &other) const
{
	InfiniteFloat				dividend(*this);
	InfiniteFloat				divisor(other);
	std::vector<unsigned char>	result_integer;
	std::vector<unsigned char>	result_decimal;

	remove_decimal_part_in_divisor(dividend, divisor);
	division_loop(dividend.abs(), divisor.getIntegerPart(), 	\
			result_integer, result_decimal);
	return (InfiniteFloat(InfiniteInt(result_integer), 			\
			InfiniteInt(result_decimal, false, false), 			\
			this->getIsNegative() != other.getIsNegative()));
}

InfiniteFloat	InfiniteFloat::operator/(const InfiniteInt &other) const
{
	return (*this / InfiniteFloat(other));
}

InfiniteFloat	InfiniteFloat::operator/(const long long int value) const
{
	return (*this / InfiniteFloat(value));
}

void			InfiniteFloat::operator/=(const InfiniteFloat &other)
{
	*this = *this / other;
}

void			InfiniteFloat::operator/=(const InfiniteInt &other)
{
	*this /= InfiniteFloat(other);
}

void			InfiniteFloat::operator/=(const long long int value)
{
	*this /= InfiniteFloat(value);
}

InfiniteFloat	InfiniteFloat::operator%(const InfiniteFloat &other) const
{
	InfiniteFloat	division(*this / other);

	return (*this - InfiniteFloat(division.getIntegerPart()) * other);
}

InfiniteFloat	InfiniteFloat::operator%(const InfiniteInt &other) const
{
	return (*this % InfiniteFloat(other));
}

InfiniteFloat	InfiniteFloat::operator%(const long long int value) const
{
	return (*this % InfiniteFloat(value));
}

void			InfiniteFloat::operator%=(const InfiniteFloat &other)
{
	*this = *this % other;
}

void			InfiniteFloat::operator%=(const InfiniteInt &other)
{
	*this %= InfiniteFloat(other);
}

void			InfiniteFloat::operator%=(const long long int value)
{
	*this %= InfiniteFloat(value);
}

InfiniteFloat	InfiniteFloat::operator^(const InfiniteFloat &other) const
{
	InfiniteFloat	result(1);
	InfiniteFloat	power(other.getIntegerPart());

	if (other.getDecimalPart() || other.getIsNegative())
		throw ERROR_EXPONENT_INTEGER;
	while (power--)
		result *= *this;
	return (result);
}

InfiniteFloat	InfiniteFloat::operator^(const InfiniteInt &other) const
{
	return (*this ^ InfiniteFloat(other));
}

InfiniteFloat	InfiniteFloat::operator^(const long long int value) const
{
	return (*this ^ InfiniteFloat(value));
}

void			InfiniteFloat::operator^=(const InfiniteFloat &other)
{
	*this = *this ^ other;
}

void			InfiniteFloat::operator^=(const InfiniteInt &other)
{
	*this ^= InfiniteFloat(other);
}

void			InfiniteFloat::operator^=(const long long int value)
{
	*this ^= InfiniteFloat(value);
}


// Getters

InfiniteInt	InfiniteFloat::getDecimalPart(void) const
{
	return (this->_decimal_part);
}

InfiniteInt	InfiniteFloat::getIntegerPart(void) const
{
	return (this->_integer_part);
}

bool		InfiniteFloat::getIsNegative(void) const
{
	return (this->_isNegative);
}


// Setters

void	InfiniteFloat::setDecimalPart(const InfiniteInt &decimal_part)
{
	this->_decimal_part = decimal_part;
}

void	InfiniteFloat::setIntegerPart(const InfiniteInt &integer_part)
{
	this->_integer_part = integer_part;
}

void	InfiniteFloat::setIsNegative(bool is_negative)
{
	this->_isNegative = is_negative;
}


// Methods

InfiniteFloat	InfiniteFloat::sqrt(void) const
{
	InfiniteFloat							padding(1);
	InfiniteFloat							result;
	std::vector<unsigned char>				result_integer;
	std::vector<unsigned char>				result_decimal;
	std::vector<unsigned char>::size_type	padding_size	\
			(this->_integer_part 							\
				? (this->getIntegerPart().size() - 1) / 2 + 1 : 0);

	if (this->getIsNegative())
		throw ERROR_SQRT_NEGATIVE;
	if (!*this)
		return (InfiniteInt());
	for (std::vector<unsigned char>::size_type i = 0; i < padding_size; i++)
		padding.push_back_integer(0);
	while (padding)
		insert_new_digit(*this, padding, result_integer, result_decimal);
	return (InfiniteFloat(result_integer, result_decimal));
}

InfiniteFloat	InfiniteFloat::abs(void) const
{
	InfiniteFloat	result;

	result = *this;
	result.setIsNegative(false);
	return (result);
}

bool			InfiniteFloat::in_D(void) const
{
	return (this->_decimal_part.size() < InfiniteFloat::PRINT_PRECISION);
}

void			InfiniteFloat::push_back_decimal(unsigned char digit)
{
	this->_decimal_part.push_back(digit);
}

void			InfiniteFloat::push_back_integer(unsigned char digit)
{
	this->_integer_part.push_back(digit);
}

void			InfiniteFloat::reduce(void)
{
	this->_integer_part.reduce();
	this->_decimal_part.reduce();
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream &os, const InfiniteFloat &num)// TODO
{
	InfiniteFloat				num_copy(num);
	InfiniteFloat				factor(1);
	std::vector<unsigned char>	decimal_digits;
	std::string					rounded;

	if (num_copy.getIsNegative())
		os << "-";
	if (num_copy.getDecimalPart().size() > InfiniteFloat::PRINT_PRECISION)
	{
		if (num_copy.getDecimalPart()[InfiniteFloat::PRINT_PRECISION] >= 5)
		{
			for (unsigned char i(0); i < InfiniteFloat::PRINT_PRECISION; i++)
				factor /= 10;
			num_copy += factor;
		}
		decimal_digits = num_copy.getDecimalPart().getDigits();
		decimal_digits.resize(InfiniteFloat::PRINT_PRECISION);
		num_copy.setDecimalPart(InfiniteInt(decimal_digits, false, false));
		num_copy.reduce();
		rounded = "...";
	}
	os << num_copy.getIntegerPart();
	if (num_copy.getDecimalPart())
	{
		os << ".";
		for (unsigned char i(0); i < InfiniteFloat::PRINT_PRECISION && i < num_copy.getDecimalPart().size(); i++)
			os << static_cast<char>(num_copy.getDecimalPart()[i] + '0');
	}
	os << rounded;
	return (os);
}
