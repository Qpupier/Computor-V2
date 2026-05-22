/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/22 14:04:13 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IType_HPP
# define IType_HPP

# include <iostream>

class	Rational;

class	IType
{
	public:
		// Destructor
		virtual ~IType(void) {};

		// Operator overloads
		virtual explicit	operator bool() const = 0;
		virtual bool		operator==(const IType &other) const = 0;
		virtual bool		operator!=(const IType &other) const = 0;
		virtual bool		operator<(const IType &other) const = 0;
		virtual bool		operator<=(const IType &other) const = 0;
		virtual bool		operator>(const IType &other) const = 0;
		virtual bool		operator>=(const IType &other) const = 0;
		virtual IType*		operator+(const IType &other) const = 0;
		virtual IType*		operator-(const IType &other) const = 0;
		virtual IType*		operator*(const IType &other) const = 0;
		virtual IType*		operator/(const IType &other) const = 0;
		virtual IType*		operator%(const IType &other) const = 0;
		virtual IType*		operator^(const IType &other) const = 0;

		// Methods
		virtual IType*			matrix_operator(const IType &other) const = 0;
		virtual IType*			function_operator(const IType &other) const = 0;
		virtual IType*			clone(void) const = 0;
		virtual Rational*		gcd(const IType &other) const = 0;
		virtual std::ostream	&print(std::ostream &os) const = 0;
		virtual void			print_rounded(const std::string var = std::string()) const = 0;
		virtual bool			finite_decimals(void) const = 0;
		virtual bool			in_Z(void) const = 0;

	protected:
		// Constructor
		IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
