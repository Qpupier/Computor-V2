/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteInt.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:39:10 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/05 14:41:46 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "InfiniteInt.hpp"

// Utils

static std::vector<unsigned char>	add_infinite_int(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	std::vector<unsigned char>	rev_a(a.getDigits());
	std::vector<unsigned char>	rev_b(b.getDigits());
	std::vector<unsigned char>	result;
	unsigned char				digit_a;
	unsigned char				digit_b;
	unsigned char				sum;
	bool						hold(false);

	std::reverse(rev_a.begin(), rev_a.end());
	std::reverse(rev_b.begin(), rev_b.end());
	for (std::vector<unsigned char>::size_type i = 0; 	\
			i < rev_a.size() || i < rev_b.size() || hold; i++)
	{
		digit_a = (i < rev_a.size()) ? rev_a[i] : 0;
		digit_b = (i < rev_b.size()) ? rev_b[i] : 0;
		sum = digit_a + digit_b;
		if (hold)
			sum++;
		hold = sum >= 10;
		result.push_back((sum % 10));
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

static unsigned char	sub_infinite_int_digit(		\
		const std::vector<unsigned char>& rev_a, 	\
		const std::vector<unsigned char>& rev_b, 	\
		std::vector<unsigned char>::size_type i, bool& hold)
{
	unsigned char	digit_a((i < rev_a.size()) ? rev_a[i] : 0);
	unsigned char	digit_b((i < rev_b.size()) ? rev_b[i] : 0);
	short int		diff(digit_a - digit_b);

	if (hold)
		diff--;
	if (diff < 0)
	{
		diff += 10;
		hold = true;
	}
	else
		hold = false;
	return (static_cast<unsigned char>(diff));
}

static std::vector<unsigned char>	sub_infinite_int(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	std::vector<unsigned char>	rev_a(a.getDigits());
	std::vector<unsigned char>	rev_b(b.getDigits());
	std::vector<unsigned char>	result;
	bool						hold(false);

	std::reverse(rev_a.begin(), rev_a.end());
	std::reverse(rev_b.begin(), rev_b.end());
	for (std::vector<unsigned char>::size_type i = 0; 	\
			i < rev_a.size() || i < rev_b.size() || hold; i++)
		result.push_back(sub_infinite_int_digit(rev_a, rev_b, i, hold));
	std::reverse(result.begin(), result.end());
	return (result);
}

static std::vector<InfiniteInt>		multiplication_part(	\
		const std::vector<unsigned char> &a, 				\
		const std::vector<unsigned char> &b)
{
	std::vector<InfiniteInt>	result;

	for (std::vector<unsigned char>::const_reverse_iterator it_b 		\
			= b.rbegin(); it_b != b.rend(); it_b++)
	{
		InfiniteInt		intermediate_result;
		unsigned char	hold(0);

		for (std::vector<unsigned char>::const_reverse_iterator it_a 	\
				= a.rbegin(); it_a != a.rend(); it_a++)
		{
			unsigned char	product(*it_b * *it_a);

			if (hold)
				product += hold;
			hold = product / 10;
			intermediate_result.push_back(product % 10);
		}
		if (hold)
			intermediate_result.push_back(hold);
		intermediate_result.reverse();
		result.push_back(intermediate_result);
	}
	return (result);
}

static void							division(InfiniteInt &dividend, 	\
		InfiniteInt &tmp_dividend, const InfiniteInt &divisor, 			\
		InfiniteInt &result)
{
	InfiniteInt		factor;
	InfiniteInt		last_good_quotient;
	unsigned char	result_digit(0);
	unsigned char	last_good_factor(0);

	tmp_dividend.push_back(dividend[0]);
	dividend.pop_front();
	factor = InfiniteInt();
	while (factor <= tmp_dividend)
	{
		last_good_factor = result_digit;
		last_good_quotient = factor;
		factor += divisor;
		result_digit++;
	}
	result.push_back(last_good_factor);
	tmp_dividend -= last_good_quotient;
}


// Constructors

InfiniteInt::InfiniteInt(const std::vector<unsigned char> digits, 	\
		bool is_negative, bool is_integer_part): 					\
			_digits(digits), _isIntegerPart(is_integer_part), 		\
			_isNegative(is_negative)
{
	this->reduce();
}

InfiniteInt::InfiniteInt(const std::string str, bool is_negative, 	\
		bool is_integer_part): _isIntegerPart(false), _isNegative(false)
{
	std::string	str_copy(str);

	if (str.empty())
		return ;
	this->_isIntegerPart = is_integer_part;
	this->_isNegative = is_negative;
	if (str[0] == '-')
	{
		this->_isNegative = !this->_isNegative;
		str_copy.erase(0, 1);
	}
	for (std::string::size_type i = 0; i < str_copy.size(); i++)
		this->_digits.push_back(static_cast<unsigned char>(str_copy[i] - '0'));
	this->reduce();
}


// Operator overloads

InfiniteInt::operator bool() const
{
	return (!this->_digits.empty());
}

InfiniteInt&		InfiniteInt::operator=(const InfiniteInt &other)
{
	if (this != &other)
	{
		this->_digits = other._digits;
		this->_isNegative = other._isNegative;
	}
	return (*this);
}

InfiniteInt&		InfiniteInt::operator=(const long long int value)
{
	return (*this = InfiniteInt(value));
}

unsigned char		InfiniteInt::operator[](	\
		std::vector<unsigned char>::size_type index) const
{
	if (index >= this->_digits.size())
		throw std::out_of_range("Index out of range");
	return (this->_digits[index]);
}

unsigned char		InfiniteInt::operator[](	\
		std::vector<unsigned char>::size_type index)
{
	if (index >= this->_digits.size())
		throw std::out_of_range("Index out of range");
	return (this->_digits[index]);
}

bool				InfiniteInt::operator==(const InfiniteInt &other) const
{
	return ((!*this && !other) || (this->_digits == other._digits 	\
			&& this->_isNegative == other._isNegative));
}

bool				InfiniteInt::operator==(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) == other);
}

bool				InfiniteInt::operator==(const long long int value) const
{
	return (*this == InfiniteInt(value));
}

bool				InfiniteInt::operator!=(const InfiniteInt&other) const
{
	return (!(*this == other));
}

bool				InfiniteInt::operator!=(const InfiniteFloat&other) const
{
	return (InfiniteFloat(*this) != other);
}

bool				InfiniteInt::operator!=(const long long int value) const
{
	return (*this != InfiniteInt(value));
}

bool				InfiniteInt::operator<(const InfiniteInt &other) const
{
	InfiniteInt	a(*this);
	InfiniteInt	b(other);
	bool		result;

	if (a == b)
		return (false);
	if (a._isNegative != b._isNegative)
		return (a._isNegative);
	if (!a._isIntegerPart)
	{
		while (a._digits.size() < b._digits.size())
			a._digits.push_back(0);
		while (b._digits.size() < a._digits.size())
			b._digits.push_back(0);
	}
	if (a._digits.size() != b._digits.size())
		result = a._digits.size() < b._digits.size();
	else
		result = a._digits < b._digits;
	if (a._isNegative)
		result = !result;
	return (result);
}

bool				InfiniteInt::operator<(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) < other);
}

bool				InfiniteInt::operator<(const long long int value) const
{
	return (*this < InfiniteInt(value));
}

bool				InfiniteInt::operator<=(const InfiniteInt &other) const
{
	return (*this < other || *this == other);
}

bool				InfiniteInt::operator<=(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) <= other);
}

bool				InfiniteInt::operator<=(const long long int value) const
{
	return (*this <= InfiniteInt(value));
}

bool				InfiniteInt::operator>(const InfiniteInt &other) const
{
	return (!(*this <= other));
}

bool				InfiniteInt::operator>(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) > other);
}

bool				InfiniteInt::operator>(const long long int value) const
{
	return (*this > InfiniteInt(value));
}

bool				InfiniteInt::operator>=(const InfiniteInt &other) const
{
	return (!(*this < other));
}

bool				InfiniteInt::operator>=(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) >= other);
}

bool				InfiniteInt::operator>=(const long long int value) const
{
	return (*this >= InfiniteInt(value));
}

InfiniteInt			InfiniteInt::operator+(const InfiniteInt &other) const
{
	InfiniteInt	result;

	if (this->_isNegative && !other._isNegative)
		return (other - (-*this));
	if (!this->_isNegative && other._isNegative)
		return (*this - (-other));
	result._digits = add_infinite_int(*this, other);
	result._isIntegerPart = this->_isIntegerPart;
	result._isNegative = this->_isNegative;
	result.reduce();
	return (result);
}

InfiniteFloat		InfiniteInt::operator+(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) + other);
}

InfiniteInt			InfiniteInt::operator+(const long long int value) const
{
	return (*this + InfiniteInt(value));
}

void				InfiniteInt::operator+=(const InfiniteInt &other)
{
	*this = *this + other;
}

void				InfiniteInt::operator+=(const long long int value)
{
	*this += InfiniteInt(value);
}

InfiniteInt&		InfiniteInt::operator++(void)
{
	*this += InfiniteInt(1);
	return (*this);
}

InfiniteInt			InfiniteInt::operator++(int)
{
	InfiniteInt	result(*this);

	*this += InfiniteInt(1);
	return (result);
}

InfiniteInt			InfiniteInt::operator-(void) const
{
	InfiniteInt	result;

	result = *this;
	if (result)
		result._isNegative = !this->_isNegative;
	return (result);
}

InfiniteInt			InfiniteInt::operator-(const InfiniteInt &other) const
{
	InfiniteInt	result;

	if (this->_isNegative && other._isNegative)
		return (-other - (-*this));
	if (this->_isNegative != other._isNegative)
		return (*this + (-other));
	if (*this < other)
		return (-(other - *this));
	result._digits = sub_infinite_int(*this, other);
	result._isIntegerPart = this->_isIntegerPart;
	result._isNegative = false;
	result.reduce();
	return (result);
}

InfiniteFloat		InfiniteInt::operator-(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) - other);
}

InfiniteInt			InfiniteInt::operator-(const long long int value) const
{
	return (*this - InfiniteInt(value));
}

void				InfiniteInt::operator-=(const InfiniteInt &other)
{
	*this = *this - other;
}

void				InfiniteInt::operator-=(const long long int value)
{
	*this -= InfiniteInt(value);
}

InfiniteInt&		InfiniteInt::operator--(void)
{
	*this -= InfiniteInt(1);
	return (*this);
}

InfiniteInt			InfiniteInt::operator--(int)
{
	InfiniteInt	result(*this);

	*this -= InfiniteInt(1);
	return (result);
}

InfiniteInt			InfiniteInt::operator*(const InfiniteInt &other) const
{
	InfiniteInt					result;
	std::vector<InfiniteInt>	intermediate_results(	\
			multiplication_part(this->_digits, other._digits));
	std::size_t					offset(0);

	for (std::vector<InfiniteInt>::iterator it 			\
				= intermediate_results.begin(); 		\
			it != intermediate_results.end(); it++)
	{
		for (std::size_t i = 0; i < offset; i++)
			it->push_back(0);
		result += *it;
		offset++;
	}
	result._isNegative = this->_isNegative != other._isNegative;
	result.reduce();
	return (result);
}

InfiniteFloat		InfiniteInt::operator*(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) * other);
}

InfiniteInt			InfiniteInt::operator*(const long long int value) const
{
	return (*this * InfiniteInt(value));
}

void				InfiniteInt::operator*=(const InfiniteInt &other)
{
	*this = *this * other;
}

void				InfiniteInt::operator*=(const long long int value)
{
	*this *= InfiniteInt(value);
}

InfiniteInt			InfiniteInt::operator/(const InfiniteInt &other) const
{
	InfiniteInt	dividend(*this);
	InfiniteInt	tmp_dividend;
	InfiniteInt	divisor(other);
	InfiniteInt	result;

	dividend.setIsNegative(false);
	divisor.setIsNegative(false);
	while (!dividend.getDigits().empty())
		division(dividend, tmp_dividend, divisor, result);
	result.setIsNegative(this->_isNegative != other._isNegative);
	result.reduce();
	return (result);
}

InfiniteFloat		InfiniteInt::operator/(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) / other);
}

InfiniteInt			InfiniteInt::operator/(const long long int value) const
{
	return (*this / InfiniteInt(value));
}

void				InfiniteInt::operator/=(const InfiniteInt &other)
{
	*this = *this / other;
}

void				InfiniteInt::operator/=(const long long int value)
{
	*this /= InfiniteInt(value);
}

InfiniteInt			InfiniteInt::operator%(const InfiniteInt &other) const
{
	return (*this - *this / other * other);
}

InfiniteFloat		InfiniteInt::operator%(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) % other);
}

InfiniteInt			InfiniteInt::operator%(const long long int value) const
{
	return (*this % InfiniteInt(value));
}

void				InfiniteInt::operator%=(const InfiniteInt &other)
{
	*this = *this % other;
}

void				InfiniteInt::operator%=(const long long int value)
{
	*this %= InfiniteInt(value);
}

InfiniteInt			InfiniteInt::operator^(const InfiniteInt &other) const
{
	InfiniteInt	result(std::vector<unsigned char>(1, 1));
	InfiniteInt	exp(other);

	while (exp > InfiniteInt())
	{
		result *= *this;
		exp -= InfiniteInt(1);
	}
	return (result);
}

InfiniteFloat		InfiniteInt::operator^(const InfiniteFloat &other) const
{
	return (InfiniteFloat(*this) ^ other);
}

InfiniteInt			InfiniteInt::operator^(const long long int value) const
{
	return (*this ^ InfiniteInt(value));
}

void				InfiniteInt::operator^=(const InfiniteInt &other)
{
	*this = *this ^ other;
}

void				InfiniteInt::operator^=(const long long int value)
{
	*this ^= InfiniteInt(value);
}


// Getters

std::vector<unsigned char>	InfiniteInt::getDigits(void) const
{
	return (this->_digits);
}

bool						InfiniteInt::getIsIntegerPart(void) const
{
	return (this->_isIntegerPart);
}

bool						InfiniteInt::getIsNegative(void) const
{
	return (this->_isNegative);
}


// Setters

void	InfiniteInt::setDigits(const std::vector<unsigned char> &digits)
{
	this->_digits = digits;
	this->reduce();
}

void	InfiniteInt::setIsIntegerPart(bool is_integer_part)
{
	this->_isIntegerPart = is_integer_part;
}

void	InfiniteInt::setIsNegative(bool is_negative)
{
	this->_isNegative = is_negative;
}


// Methods

InfiniteInt	InfiniteInt::abs(void) const
{
	InfiniteInt	result(*this);

	result.setIsNegative(false);
	return (result);
}

InfiniteInt	InfiniteInt::sqrt(void) const
{
	std::vector<unsigned char>::size_type	size((this->size() - 1) / 2 + 1);
	InfiniteInt								padding(1);
	InfiniteInt								result;

	if (this->_isNegative)
		throw ERROR_SQRT_NEGATIVE;
	if (!*this)
		return (InfiniteInt());
	for (std::vector<unsigned char>::size_type i = 0; i < size; i++)
		padding.push_back(0);
	while (padding)
	{
		for (unsigned char test_digit = 1; test_digit <= 10; test_digit++)
		{
			std::vector<unsigned char> test(result.getDigits());
			test.push_back(test_digit);
			if (((InfiniteInt(test) * padding) ^ InfiniteInt(2)) > *this)
			{
				result.push_back(test_digit - 1);
				break ;
			}
		}
		padding /= InfiniteInt(10);
	}
	return (result);
}

void	InfiniteInt::pop_front(void)
{
	if (this->_digits.empty())
		throw std::out_of_range("Cannot pop from an empty InfiniteInt");
	this->_digits.erase(this->_digits.begin());
}

void		InfiniteInt::push_back(unsigned char digit)
{
	this->_digits.push_back(digit);
}

std::string	InfiniteInt::to_string(void) const
{
	std::string	str;

	if (this->_isNegative)
		str += '-';
	for (std::vector<unsigned char>::const_iterator it 	\
			= this->_digits.begin(); it != this->_digits.end(); it++)
		str += static_cast<char>(*it + '0');
	return (str);
}

std::size_t	InfiniteInt::size(void) const
{
	return (this->_digits.size());
}

void		InfiniteInt::reverse(void)
{
	std::reverse(this->_digits.begin(), this->_digits.end());
}

void		InfiniteInt::reduce(void)
{
	if (!this->_isIntegerPart)
		this->reverse();
	while (!this->_digits.empty() && !this->_digits[0])
		this->_digits.erase(this->_digits.begin());
	if (this->_digits.empty())
		this->_isNegative = false;
	else if (!this->_isIntegerPart)
		this->reverse();
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream &os, const InfiniteInt &num)
{
	std::vector<unsigned char>	digits(num.getDigits());

	if (num.getDigits().empty())
		os << '0';
	else if (num.getIsNegative())
		os << '-';
	for (std::vector<unsigned char>::const_iterator it = digits.begin(); 	\
			it != digits.end(); it++)
		os << static_cast<char>(*it + '0');
	return (os);
}
