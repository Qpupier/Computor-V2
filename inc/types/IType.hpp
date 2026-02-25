/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/25 18:09:14 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IType_HPP
# define IType_HPP

# include "computor-v2.hpp"

class	AST;

class	IType
{
	public:
		// Destructor
		virtual ~IType(void) {};

		// Operator overloads
		virtual AST*	operator+(const IType &other) const = 0;
		virtual AST*	operator-(const IType &other) const = 0;
		virtual AST*	operator*(const IType &other) const = 0;
		virtual AST*	operator/(const IType &other) const = 0;
		virtual AST*	operator%(const IType &other) const = 0;
		virtual AST*	operator^(const IType &other) const = 0;

		// Methods
		virtual AST*	matrix_operator(const IType &other) const = 0;
		virtual std::ostream	&print(std::ostream &os) const = 0;

	protected:
		// Constructor
		IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
