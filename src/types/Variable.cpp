/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 17:29:16 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Variable.hpp"

// Operator overloads
Variable	&Variable::operator=(const Variable &other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_value = other._value;
	}
	return (*this);
}


// Getters
std::string	Variable::get_name(void) const
{
	return (this->_name);
}


// Methods
Node*	Variable::matrix_operator(const IType &other) const
{
	(void)other;
	return (nullptr);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Variable &var)
{
	os << "Variable: " << var.get_name() << std::endl;
	return (os);
}
