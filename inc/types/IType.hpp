/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 14:39:37 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IType_HPP
# define IType_HPP

class	Node;
// class	Number;

class	IType
{
	public:
		// Operator overloads
		// virtual IType	&operator=(const IType &other) = 0;
		// virtual IType*	operator-(const IType &other) const = 0;
		// virtual IType*	operator+(const IType &other) const = 0;
		virtual Node*	operator*(const IType &other) const = 0;
		// virtual IType*	operator/(const IType &other) const = 0;
		// virtual IType*	operator%(const IType &other) const = 0;
		// virtual IType*	operator^(const IType &other) const = 0;
		// IType* operator**(const IType &other) const = 0;

	protected:
		// Constructor and destructor
		IType(void) {};
		virtual ~IType(void) {};
};

#endif
