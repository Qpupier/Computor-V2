/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 18:42:26 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IType_HPP
# define IType_HPP

class	AST;

class	IType
{
	public:
		// Operator overloads
		virtual AST*	operator+(const IType &other) const = 0;
		virtual AST*	operator-(const IType &other) const = 0;
		virtual AST*	operator*(const IType &other) const = 0;
		virtual AST*	operator/(const IType &other) const = 0;
		virtual AST*	operator%(const IType &other) const = 0;
		virtual AST*	operator^(const IType &other) const = 0;

		// Methods
		virtual AST*	matrix_operator(const IType &other) const = 0;

	protected:
		// Constructor and destructor
		IType(void) {};
		virtual ~IType(void) {};
};

#endif
