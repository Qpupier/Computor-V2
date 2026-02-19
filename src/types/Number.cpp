/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Number.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:38:37 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 14:45:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Number.hpp"

// Operator overloads
Number	&Number::operator=(const Number &other)
{
	if (this != &other)
		this->_value = other._value;
	return (*this);
}

Node	*Number::operator*(const Number &other) const
{
	return (new Node(std::to_string(this->_value * other._value), Token::E_NUMBER));
}

Node	*Number::operator*(const IType &other) const
{
	const Number *other_number = dynamic_cast<const Number*>(&other);

	if (other_number)
		return (*this * *other_number);
	return (nullptr);
}


// Getters
double	Number::get_value(void) const
{
	return (this->_value);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Number &num)
{
	os << "Number: " << num.get_value() << std::endl;
	return (os);
}
