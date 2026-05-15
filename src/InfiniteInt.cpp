/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteInt.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:39:10 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/15 19:39:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "InfiniteInt.hpp"

// Utils
static std::vector<unsigned short int>	add_infinite_int(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	std::vector<unsigned short int>	rev_a(a.getDigits());
	std::vector<unsigned short int>	rev_b(b.getDigits());
	std::vector<unsigned short int>	result;
	unsigned short int				digit_a;
	unsigned short int				digit_b;
	unsigned short int				sum;
	bool							hold(false);

	for (std::vector<unsigned short int>::size_type i = 0; 	\
			i < rev_a.size() || i < rev_b.size() || hold; i++)
	{
		digit_a = (i < rev_a.size()) ? rev_a[i] - '0' : 0;
		digit_b = (i < rev_b.size()) ? rev_b[i] - '0' : 0;
		sum = digit_a + digit_b + hold;
		hold = sum >= 10;
		result.push_back((sum % 10) + '0');
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

static InfiniteInt*						sub_operator_different_signs(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	InfiniteInt*	negative;
	InfiniteInt*	result;

	if (a.getIsNegative() && !b.getIsNegative())
	{
		negative = -b;
		result = a + *negative;
		delete negative;
		return (result);
	}
	if (!a.getIsNegative() && b.getIsNegative())
	{
		negative = -b;
		result = a + *negative;
		delete negative;
		return (result);
	}
	return (nullptr);
}

static InfiniteInt*						sub_operator_edge_cases(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	InfiniteInt*	negative_this;
	InfiniteInt*	negative_other;
	InfiniteInt*	result;

	result = sub_operator_different_signs(a, b);
	if (result)
		return (result);
	if (a.getIsNegative())
	{
		negative_this = -a;
		negative_other = -b;
		result = *negative_other - *negative_this;
		delete negative_this;
		delete negative_other;
		return (result);
	}
	if (a < b)
	{
		negative_this = b - a;
		result = -*negative_this;
		delete negative_this;
		return (result);
	}
	return (nullptr);
}

static std::vector<unsigned short int>	sub_infinite_int(	\
		const InfiniteInt &a, const InfiniteInt &b)
{
	std::vector<unsigned short int>	rev_a(a.getDigits());
	std::vector<unsigned short int>	rev_b(b.getDigits());
	std::vector<unsigned short int>	result;
	unsigned short int				digit_a;
	unsigned short int				digit_b;
	short int						diff;
	bool							hold(false);

	for (std::vector<unsigned short int>::size_type i = 0; 	\
			i < rev_a.size() || i < rev_b.size() || hold; i++)
	{
		digit_a = (i < rev_a.size()) ? rev_a[i] - '0' : 0;
		digit_b = (i < rev_b.size()) ? rev_b[i] - '0' : 0;
		diff = digit_a - digit_b - hold;
		hold = diff < 0;
		if (hold)
			diff += 10;
		result.push_back(diff + '0');
	}
	while (result.size() > 1 && result.back() == '0')
		result.pop_back();
	std::reverse(result.begin(), result.end());
	return (result);
}

static std::vector<InfiniteInt>			multiplication_part(	\
		const std::vector<unsigned short int> &a, 		\
		const std::vector<unsigned short int> &b)
{
	std::vector<InfiniteInt>	result;
	unsigned long long int		offset(0);

	for (std::vector<unsigned short int>::reverse_iterator it_b 		\
			= b.rbegin(); it_b != b.rend(); it_b++)
	{
		std::vector<unsigned short int>	offset_digits(offset);
		InfiniteInt						intermediate_result(offset_digits);
		unsigned short int				hold(0);

		for (std::vector<unsigned short int>::reverse_iterator it_a 	\
				= a.rbegin(); it_a != a.rend(); it_a++)
		{
			unsigned short int	product(*it_b * *it_a + hold);

			hold = product / 10;
			intermediate_result.push_back(product % 10);
		}
		if (hold)
			intermediate_result.push_back(hold);
		intermediate_result.reverse();
		result.push_back(intermediate_result);
		offset++;
	}
	return (result);
}


// Operator overloads
InfiniteInt&	InfiniteInt::operator=(const InfiniteInt &other)
{
	if (this != &other)
	{
		this->_digits = other._digits;
		this->_is_negative = other._is_negative;
	}
	return (*this);
}

explicit		InfiniteInt::operator bool() const
{
	return (this->_digits != std::vector<unsigned short int>(1));
}

bool			InfiniteInt::operator==(const InfiniteInt &other) const
{
	return ((!*this && !other) || (this->_digits == other._digits 	\
			&& this->_is_negative == other._is_negative));
}

bool			InfiniteInt::operator!=(const InfiniteInt&other) const
{
	return (!(*this == other));
}

bool			InfiniteInt::operator<(const InfiniteInt &other) const
{
	bool	result;

	if (*this == other)
		return (false);
	if (this->_is_negative != other._is_negative)
		return (this->_is_negative);
	if (this->_digits.size() != other._digits.size())
		result = this->_digits.size() < other._digits.size();
	else
		result = this->_digits < other._digits;
	if (this->_is_negative)
		result = !result;
	return (result);
}

bool			InfiniteInt::operator<=(const InfiniteInt &other) const
{
	return (*this < other || *this == other);
}

bool			InfiniteInt::operator>(const InfiniteInt &other) const
{
	return (!(*this <= other));
}

bool			InfiniteInt::operator>=(const InfiniteInt &other) const
{
	return (!(*this < other));
}

InfiniteInt*	InfiniteInt::operator+(const InfiniteInt &other) const
{
	InfiniteInt*	negative;
	InfiniteInt*	result;

	if (this->_is_negative && !other._is_negative)
	{
		negative = -*this;
		result = other - *negative;
		delete negative;
		return (result);
	}
	if (!this->_is_negative && other._is_negative)
	{
		negative = -other;
		result = *this - *negative;
		delete negative;
		return (result);
	}
	result = this->clone();
	result->_digits = add_infinite_int(*result, other);
	return (result);
}

InfiniteInt*	InfiniteInt::operator-(void) const
{
	InfiniteInt*	result;

	result = new InfiniteInt(*this);
	if (*result)
		result->_is_negative = !this->_is_negative;
	return (result);
}

InfiniteInt*	InfiniteInt::operator-(const InfiniteInt &other) const
{
	InfiniteInt*	result;

	result = sub_operator_edge_cases(*this, other);
	if (result)
		return (result);
	result = this->clone();
	result->_digits = sub_infinite_int(*this, other);
	return (result);
}

InfiniteInt*	InfiniteInt::operator*(const InfiniteInt &other) const
{
	InfiniteInt*				result;
	std::vector<InfiniteInt>	intermediate_results(multiplication_part(this->_digits, other._digits));

	result = new InfiniteInt(std::vector<unsigned short int>(1, 1));
	for (std::vector<InfiniteInt>::iterator it = intermediate_results.begin(); it != intermediate_results.end(); it++)
	{
		InfiniteInt*	sum(*result + *it);

		delete result;
		result = sum;
	}
	result->_is_negative = this->_is_negative != other._is_negative;
	return (result);
}

InfiniteInt*	InfiniteInt::operator/(const InfiniteInt &other) const
{
}
