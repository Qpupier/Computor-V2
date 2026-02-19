/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 13:35:08 by qpupier          ###   ########lyon.fr   */
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

// Node	*Variable::operator*(const Variable &other) const
// {
// 	std::vector<IType*>	new_tokens(3);

// 	new_tokens[0] = this;
// 	new_tokens[1] = new Operator("*");
// 	new_tokens[2] = &other;
// 	return (new Node(new_tokens));
// }


// Getters
std::string	Variable::get_name(void) const
{
	return (this->_name);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Variable &var)
{
	os << "Variable: " << var.get_name() << std::endl;
	return (os);
}
