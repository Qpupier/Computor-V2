/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:37:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/18 17:43:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Operator.hpp"

// Constructors and destructor
Operator::Operator(const std::string &op)
{
	if (op == "+")
		this->_operator = E_ADD;
	else if (op == "-")
		this->_operator = E_SUBTRACT;
	else if (op == "*")
		this->_operator = E_MULTIPLY;
	else if (op == "/")
		this->_operator = E_DIVIDE;
	else if (op == "%")
		this->_operator = E_MODULO;
	else if (op == "^")
		this->_operator = E_POWER;
	else if (op == "***")
		this->_operator = E_MULTIPLY_MATRIX;
	else
		throw std::invalid_argument("Invalid operator: " + op);
}

// Operator overloads
Operator	&Operator::operator=(const Operator &other)
{
	if (this != &other)
		this->_operator = other._operator;
	return (*this);
}


// Getters
Operator::t_operator	Operator::get_operator(void) const
{
	return (this->_operator);
}
