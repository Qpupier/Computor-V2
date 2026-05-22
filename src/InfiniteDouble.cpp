/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteDouble.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:46:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/22 21:51:51 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "InfiniteDouble.hpp"

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

static InfiniteInt		add_integer_parts(const InfiniteDouble & tmp_a, 	\
		const InfiniteDouble & tmp_b, 										\
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

static InfiniteInt		sub_integer_parts(const InfiniteDouble & tmp_a, 	\
		const InfiniteDouble & tmp_b, 										\
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

static InfiniteInt		mul_integer_parts(const InfiniteDouble & tmp_a, 	\
		const InfiniteDouble & tmp_b)
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

static InfiniteDouble	place_floating_point(							\
		const InfiniteInt & int_result, 								\
		const std::vector<unsigned char>::size_type & decimal_size, 	\
		const bool is_negative)
{
	InfiniteDouble							result;
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
	result.setIntegerPart(InfiniteInt(vector_integer));
	result.setDecimalPart(InfiniteInt(vector_decimal, false, false));
	result.setIsNegative(is_negative);
	result.reduce();
	return (result);
}

static void				remove_decimal_part_in_divisor(	\
		InfiniteDouble & dividend, InfiniteDouble & divisor)
{
	while (divisor.getDecimalPart())
	{
		dividend *= InfiniteDouble(10);
		divisor *= InfiniteDouble(10);
	}
}

static bool				division_next_digit(							\
		const InfiniteDouble & dividend, InfiniteInt & tmp_dividend, 	\
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

static void				division_loop(const InfiniteDouble & dividend, 	\
		const InfiniteInt & divisor, 									\
		std::vector<unsigned char> & result_integer, 					\
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

static void				insert_new_digit(const InfiniteDouble num, 	\
		InfiniteDouble & padding, 						\
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
		if (((InfiniteDouble(test) * padding) ^ InfiniteDouble(2)) > num)
			break ;
		last_good_digit = test_digit;
	}
	if (padding >= InfiniteDouble(1))
		result_integer.push_back(last_good_digit);
	else
		result_decimal.push_back(last_good_digit);
	padding /= InfiniteDouble(10);
}


// Constructors
InfiniteDouble::InfiniteDouble(const InfiniteInt integer_part, 			\
		const InfiniteInt decimal_part, bool is_negative, 				\
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

InfiniteDouble&	InfiniteDouble::operator=(const InfiniteInt &other)
{
	return (*this = InfiniteDouble(other));
}

InfiniteDouble&	InfiniteDouble::operator=(const long long int value)
{
	return (*this = InfiniteDouble(value));
}

bool			InfiniteDouble::operator==(const InfiniteDouble &other) const
{
	return (this->_isNegative == other._isNegative 					\
			&& this->getIntegerPart() == other.getIntegerPart() 	\
			&& this->getDecimalPart() == other.getDecimalPart());
}

bool			InfiniteDouble::operator==(const InfiniteInt &other) const
{
	return (*this == InfiniteDouble(other));
}

bool			InfiniteDouble::operator==(const long long int value) const
{
	return (*this == InfiniteDouble(value));
}

bool			InfiniteDouble::operator!=(const InfiniteDouble&other) const
{
	return (!(*this == other));
}

bool			InfiniteDouble::operator!=(const InfiniteInt&other) const
{
	return (!(*this == other));
}

bool			InfiniteDouble::operator!=(const long long int value) const
{
	return (*this != InfiniteDouble(value));
}

bool			InfiniteDouble::operator<(const InfiniteDouble &other) const
{
	if (this->_isNegative != other._isNegative)
		return (this->_isNegative);
	return (this->getIntegerPart() < other.getIntegerPart() 		\
			|| (this->getIntegerPart() == other.getIntegerPart() 	\
				&& this->getDecimalPart() < other.getDecimalPart()));
}

bool			InfiniteDouble::operator<(const InfiniteInt &other) const
{
	return (*this < InfiniteDouble(other));
}

bool			InfiniteDouble::operator<(const long long int value) const
{
	return (*this < InfiniteDouble(value));
}

bool			InfiniteDouble::operator<=(const InfiniteDouble &other) const
{
	return (*this < other || *this == other);
}

bool			InfiniteDouble::operator<=(const InfiniteInt &other) const
{
	return (*this <= InfiniteDouble(other));
}

bool			InfiniteDouble::operator<=(const long long int value) const
{
	return (*this <= InfiniteDouble(value));
}

bool			InfiniteDouble::operator>(const InfiniteDouble &other) const
{
	return (!(*this <= other));
}

bool			InfiniteDouble::operator>(const InfiniteInt &other) const
{
	return (*this > InfiniteDouble(other));
}

bool			InfiniteDouble::operator>(const long long int value) const
{
	return (*this > InfiniteDouble(value));
}

bool			InfiniteDouble::operator>=(const InfiniteDouble &other) const
{
	return (!(*this < other));
}

bool			InfiniteDouble::operator>=(const InfiniteInt &other) const
{
	return (*this >= InfiniteDouble(other));
}

bool			InfiniteDouble::operator>=(const long long int value) const
{
	return (*this >= InfiniteDouble(value));
}

InfiniteDouble	InfiniteDouble::operator+(const InfiniteDouble &other) const
{
	std::vector<unsigned char>::size_type	decimal_size	\
			(std::max(this->getDecimalPart().size(), 		\
				other.getDecimalPart().size()));
	InfiniteInt								int_result;

	if (this->_isNegative && !other._isNegative)
		return (other - (-*this));
	if (!this->_isNegative && other._isNegative)
		return (*this - (-other));
	int_result = add_integer_parts(*this, other, decimal_size);
	return (place_floating_point(int_result, decimal_size, this->_isNegative));
}

InfiniteDouble	InfiniteDouble::operator+(const InfiniteInt &other) const
{
	return (*this + InfiniteDouble(other));
}

InfiniteDouble	InfiniteDouble::operator+(const long long int value) const
{
	return (*this + InfiniteDouble(value));
}

void			InfiniteDouble::operator+=(const InfiniteDouble &other)
{
	*this = *this + other;
}

void			InfiniteDouble::operator+=(const InfiniteInt &other)
{
	*this += InfiniteDouble(other);
}

void			InfiniteDouble::operator+=(const long long int value)
{
	*this += InfiniteDouble(value);
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
	std::vector<unsigned char>::size_type	decimal_size	\
			(std::max(this->getDecimalPart().size(), 		\
				other.getDecimalPart().size()));
	InfiniteInt								int_result;

	if (this->_isNegative && other._isNegative)
		return (-other - (-*this));
	if (this->_isNegative != other._isNegative)
		return (*this + (-other));
	if (*this < other)
		return (-(other - *this));
	int_result = sub_integer_parts(*this, other, decimal_size);
	return (place_floating_point(int_result, decimal_size, false));
}

InfiniteDouble	InfiniteDouble::operator-(const InfiniteInt &other) const
{
	return (*this - InfiniteDouble(other));
}

InfiniteDouble	InfiniteDouble::operator-(const long long int value) const
{
	return (*this - InfiniteDouble(value));
}

void			InfiniteDouble::operator-=(const InfiniteDouble &other)
{
	*this = *this - other;
}

void			InfiniteDouble::operator-=(const InfiniteInt &other)
{
	*this -= InfiniteDouble(other);
}

void			InfiniteDouble::operator-=(const long long int value)
{
	*this -= InfiniteDouble(value);
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
	std::vector<unsigned char>::size_type	decimal_size	\
			(this->getDecimalPart().size() + other.getDecimalPart().size());
	InfiniteInt								int_result;

	int_result = mul_integer_parts(*this, other);
	return (place_floating_point(int_result, decimal_size, 	\
			this->_isNegative != other._isNegative));
}

InfiniteDouble	InfiniteDouble::operator*(const InfiniteInt &other) const
{
	return (*this * InfiniteDouble(other));
}

InfiniteDouble	InfiniteDouble::operator*(const long long int value) const
{
	return (*this * InfiniteDouble(value));
}

void			InfiniteDouble::operator*=(const InfiniteDouble &other)
{
	*this = *this * other;
}

void			InfiniteDouble::operator*=(const InfiniteInt &other)
{
	*this *= InfiniteDouble(other);
}

void			InfiniteDouble::operator*=(const long long int value)
{
	*this *= InfiniteDouble(value);
}

InfiniteDouble	InfiniteDouble::operator/(const InfiniteDouble &other) const
{
	InfiniteDouble				dividend(*this);
	InfiniteDouble				divisor(other);
	std::vector<unsigned char>	result_integer;
	std::vector<unsigned char>	result_decimal;

	remove_decimal_part_in_divisor(dividend, divisor);
	division_loop(dividend, divisor.getIntegerPart(), result_integer, 	\
			result_decimal);
	return (InfiniteDouble(InfiniteInt(result_integer), 				\
			InfiniteInt(result_decimal, false, false), 					\
			this->_isNegative != other._isNegative));
}

InfiniteDouble	InfiniteDouble::operator/(const InfiniteInt &other) const
{
	return (*this / InfiniteDouble(other));
}

InfiniteDouble	InfiniteDouble::operator/(const long long int value) const
{
	return (*this / InfiniteDouble(value));
}

void			InfiniteDouble::operator/=(const InfiniteDouble &other)
{
	*this = *this / other;
}

void			InfiniteDouble::operator/=(const InfiniteInt &other)
{
	*this /= InfiniteDouble(other);
}

void			InfiniteDouble::operator/=(const long long int value)
{
	*this /= InfiniteDouble(value);
}

InfiniteDouble	InfiniteDouble::operator%(const InfiniteDouble &other) const
{
	InfiniteDouble	division(*this / other);

	return (*this - InfiniteDouble(division.getIntegerPart()) * other);
}

InfiniteDouble	InfiniteDouble::operator%(const InfiniteInt &other) const
{
	return (*this % InfiniteDouble(other));
}

InfiniteDouble	InfiniteDouble::operator%(const long long int value) const
{
	return (*this % InfiniteDouble(value));
}

void			InfiniteDouble::operator%=(const InfiniteDouble &other)
{
	*this = *this % other;
}

void			InfiniteDouble::operator%=(const InfiniteInt &other)
{
	*this %= InfiniteDouble(other);
}

void			InfiniteDouble::operator%=(const long long int value)
{
	*this %= InfiniteDouble(value);
}

InfiniteDouble	InfiniteDouble::operator^(const InfiniteDouble &other) const
{
	InfiniteDouble	result(1);
	InfiniteDouble	power(other.getIntegerPart());

	if (other.getDecimalPart() || other.getIsNegative())
		throw ERROR_EXPONENT_INTEGER;
	while (power--)
		result *= *this;
	return (result);
}

InfiniteDouble	InfiniteDouble::operator^(const InfiniteInt &other) const
{
	return (*this ^ InfiniteDouble(other));
}

InfiniteDouble	InfiniteDouble::operator^(const long long int value) const
{
	return (*this ^ InfiniteDouble(value));
}

void			InfiniteDouble::operator^=(const InfiniteDouble &other)
{
	*this = *this ^ other;
}

void			InfiniteDouble::operator^=(const InfiniteInt &other)
{
	*this ^= InfiniteDouble(other);
}

void			InfiniteDouble::operator^=(const long long int value)
{
	*this ^= InfiniteDouble(value);
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
InfiniteDouble	InfiniteDouble::sqrt(void) const
{
	InfiniteDouble							padding(1);
	InfiniteDouble							result;
	std::vector<unsigned char>				result_integer;
	std::vector<unsigned char>				result_decimal;
	std::vector<unsigned char>::size_type	padding_size	\
			(this->_integer_part 							\
				? (this->getIntegerPart().size() - 1) / 2 + 1 : 0);

	if (this->_isNegative)
		throw ERROR_SQRT_NEGATIVE;
	if (!*this)
		return (InfiniteInt());
	for (std::vector<unsigned char>::size_type i = 0; i < padding_size; i++)
		padding.push_back_integer(0);
	while (padding)
		insert_new_digit(*this, padding, result_integer, result_decimal);
	return (InfiniteDouble(result_integer, result_decimal));
}

void			InfiniteDouble::push_back_decimal(unsigned char digit)
{
	this->_decimal_part.push_back(digit);
}

void			InfiniteDouble::push_back_integer(unsigned char digit)
{
	this->_integer_part.push_back(digit);
}

void			InfiniteDouble::reduce(void)
{
	this->_integer_part.reduce();
	this->_decimal_part.reduce();
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteDouble &num)
{
	InfiniteDouble				num_copy(num);
	InfiniteDouble				factor(1);
	std::vector<unsigned char>	decimal_digits;
	std::string					rounded;

	if (num_copy.getIsNegative())
		os << "-";
	if (num_copy.getDecimalPart().size() > InfiniteDouble::PRECISION)
	{
		if (num_copy.getDecimalPart()[InfiniteDouble::PRECISION] >= 5)
		{
			for (unsigned char i(0); i < InfiniteDouble::PRECISION; i++)
				factor /= 10;
			num_copy += factor;
		}
		decimal_digits = num_copy.getDecimalPart().getDigits();
		decimal_digits.resize(InfiniteDouble::PRECISION);
		num_copy.setDecimalPart(InfiniteInt(decimal_digits, false, false));
		num_copy.reduce();
		rounded = "...";
	}
	os << num_copy.getIntegerPart();
	if (num_copy.getDecimalPart())
	{
		os << ".";
		for (unsigned char i(0); i < InfiniteDouble::PRECISION && i < num_copy.getDecimalPart().size(); i++)
			os << static_cast<char>(num_copy.getDecimalPart()[i] + '0');
	}
	os << rounded;
	return (os);
}
