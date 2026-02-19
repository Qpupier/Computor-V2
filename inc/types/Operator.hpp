/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:32:23 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/18 17:43:30 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

#include "IType.hpp"
#include "computor-v2.hpp"

class	Operator: public IType
{
	public:
		typedef enum	e_operator
		{
			E_ADD,
			E_SUBTRACT,
			E_MULTIPLY,
			E_DIVIDE,
			E_MODULO,
			E_POWER,
			E_MULTIPLY_MATRIX
		}		t_operator;

		// Constructors and destructor
		Operator(const std::string &op);
		Operator(const Operator &other): _operator(other._operator) {};
		~Operator(void) {};

		// Operator overloads
		Operator	&operator=(const Operator &other);

		// Getters
		t_operator	get_operator(void) const;

	private:
		t_operator	_operator;
};

#endif
