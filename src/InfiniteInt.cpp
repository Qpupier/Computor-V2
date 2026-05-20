/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteInt.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:39:10 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/20 20:39:01 by qpupier          ###   ########lyon.fr   */
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
		sum = digit_a + digit_b + hold;
		hold = sum >= 10;
		result.push_back((sum % 10));
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

static std::vector<unsigned char>	sub_infinite_int(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	std::vector<unsigned char>	rev_a(a.getDigits());
	std::vector<unsigned char>	rev_b(b.getDigits());
	std::vector<unsigned char>	result;
	bool							hold(false);

	std::reverse(rev_a.begin(), rev_a.end());
	std::reverse(rev_b.begin(), rev_b.end());
	for (std::vector<unsigned char>::size_type i = 0; 	\
			i < rev_a.size() || i < rev_b.size() || hold; i++)
	{
		unsigned char	digit_a((i < rev_a.size()) ? rev_a[i] : 0);
		unsigned char	digit_b((i < rev_b.size()) ? rev_b[i] : 0);
		short int			diff(digit_a - digit_b - hold);

		if (digit_a < digit_b + hold)
		{
			diff += 10;
			hold = true;
		}
		else
			hold = false;
		result.push_back(diff);
	}
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
		InfiniteInt						intermediate_result;
		unsigned char				hold(0);

		for (std::vector<unsigned char>::const_reverse_iterator it_a 	\
				= a.rbegin(); it_a != a.rend(); it_a++)
		{
			unsigned char	product(*it_b * *it_a + hold);

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

static void							division_sub(			\
		InfiniteInt tmp_remainder, InfiniteInt &dividend, 	\
		std::size_t nb, InfiniteInt &result)
{
	for (std::size_t i = nb; i < dividend.getDigits().size(); i++)
		if (!tmp_remainder && !dividend.getDigits()[i])
			result.push_back(0);
		else
			tmp_remainder.push_back(dividend.getDigits()[i]);
	dividend = tmp_remainder;
}

static bool							division(InfiniteInt &dividend, 	\
		const InfiniteInt &divisor, InfiniteInt &result)
{
	InfiniteInt		tmp_dividend;
	InfiniteInt		factor;
	InfiniteInt		last_good_quotient;
	std::size_t		nb(0);
	unsigned char	result_digit(0);
	unsigned char	last_good_factor(0);

	// std::cout << "dividend: " << dividend << " divisor: " << divisor << std::endl;
	while (tmp_dividend < divisor && nb < dividend.getDigits().size())
	{
		tmp_dividend.push_back(dividend.getDigits()[nb]);
		nb++;
		// std::cout << "tmp_dividend: " << tmp_dividend << std::endl;
	}
	if (tmp_dividend < divisor)
		return (false);
	factor = InfiniteInt();
	while (factor <= tmp_dividend)
	{
		last_good_factor = result_digit;
		last_good_quotient = factor;
		factor += divisor;
		result_digit++;
		// std::cout << "factor: " << factor << " tmp_dividend: " << tmp_dividend << std::endl;
	}
	result.push_back(last_good_factor);
	division_sub(tmp_dividend - last_good_quotient, dividend, nb, result);
	return (true);
}


// Constructors
InfiniteInt::InfiniteInt(const std::vector<unsigned char> &digits, \
		bool is_negative, bool is_integer_part): _digits(digits), _isIntegerPart(is_integer_part), _isNegative(is_negative)
{
	this->reduce();
}

InfiniteInt::InfiniteInt(const std::string &str, bool is_negative, bool is_integer_part)
{
	if (str.empty())
		return ;
	this->_isIntegerPart = is_integer_part;
	this->_isNegative = is_negative;
	for (std::string::size_type i = 0; i < str.size(); i++)
		this->_digits.push_back(str[i] - '0');
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

bool				InfiniteInt::operator!=(const InfiniteInt&other) const
{
	return (!(*this == other));
}

bool				InfiniteInt::operator<(const InfiniteInt &other) const
{
	bool	result;

	if (*this == other)
		return (false);
	if (this->_isNegative != other._isNegative)
		return (this->_isNegative);
	if (this->_digits.size() != other._digits.size())
		result = this->_digits.size() < other._digits.size();
	else
		result = this->_digits < other._digits;
	if (this->_isNegative)
		result = !result;
	return (result);
}

bool				InfiniteInt::operator<=(const InfiniteInt &other) const
{
	return (*this < other || *this == other);
}

bool				InfiniteInt::operator>(const InfiniteInt &other) const
{
	return (!(*this <= other));
}

bool				InfiniteInt::operator>=(const InfiniteInt &other) const
{
	return (!(*this < other));
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

void				InfiniteInt::operator+=(const InfiniteInt &other)
{
	*this = *this + other;
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

	result = this->clone();
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

void				InfiniteInt::operator-=(const InfiniteInt &other)
{
	*this = *this - other;
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

	for (std::vector<InfiniteInt>::iterator it 	\
				= intermediate_results.begin(); 	\
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

void				InfiniteInt::operator*=(const InfiniteInt &other)
{
	*this = *this * other;
}

InfiniteInt			InfiniteInt::operator/(const InfiniteInt &other) const
{
	InfiniteInt	dividend(*this);
	InfiniteInt	result;

	while (!dividend.getDigits().empty())
		if (!division(dividend, other, result))
			break ;
	result.setIsNegative(this->_isNegative != other._isNegative);
	result.reduce();
	return (result);
}

void				InfiniteInt::operator/=(const InfiniteInt &other)
{
	*this = *this / other;
}

InfiniteInt			InfiniteInt::operator%(const InfiniteInt &other) const
{
	return (*this - *this / other * other);
}

void				InfiniteInt::operator%=(const InfiniteInt &other)
{
	*this = *this % other;
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

void				InfiniteInt::operator^=(const InfiniteInt &other)
{
	*this = *this ^ other;
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
InfiniteInt	InfiniteInt::clone(void) const
{
	return (InfiniteInt(this->_digits, this->_isNegative));
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
	while (!this->_digits.empty() && this->_digits[0] == 0)
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
