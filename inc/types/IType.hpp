/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/21 11:49:32 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITYPE_HPP
# define ITYPE_HPP

# include <iostream>

# include "InfiniteFloat.hpp"

class	Rational;

class	IType
{
	public:
		// Destructor
		virtual ~IType(void) {};

		// Operator overloads
		virtual explicit	operator bool() const = 0;
		virtual bool		operator==(const IType &other) const = 0;
		virtual bool		operator==(const long long int value) const = 0;
		virtual bool		operator!=(const IType &other) const = 0;
		virtual bool		operator!=(const long long int value) const = 0;
		virtual bool		operator<(const IType &other) const = 0;
		virtual bool		operator<(const long long int value) const = 0;
		virtual bool		operator<=(const IType &other) const = 0;
		virtual bool		operator<=(const long long int value) const = 0;
		virtual bool		operator>(const IType &other) const = 0;
		virtual bool		operator>(const long long int value) const = 0;
		virtual bool		operator>=(const IType &other) const = 0;
		virtual bool		operator>=(const long long int value) const = 0;
		virtual IType*		operator+(const IType &other) const = 0;
		virtual IType*		operator+(const long long int value) const = 0;
		virtual IType*		operator-(void) const = 0;
		virtual IType*		operator-(const IType &other) const = 0;
		virtual IType*		operator-(const long long int value) const = 0;
		virtual IType*		operator*(const IType &other) const = 0;
		virtual IType*		operator*(const long long int value) const = 0;
		virtual IType*		operator/(const IType &other) const = 0;
		virtual IType*		operator/(const long long int value) const = 0;
		virtual IType*		operator%(const IType &other) const = 0;
		virtual IType*		operator%(const long long int value) const = 0;
		virtual IType*		operator^(const IType &other) const = 0;
		virtual IType*		operator^(const long long int value) const = 0;

		// Getters
		// virtual InfiniteFloat	getRoundedValue(void) const = 0;

		// Methods
		virtual IType*			clone(void) const = 0;
		virtual IType*			function_operator(const IType &other) const = 0;
		virtual IType*			matrix_inversion(void) const = 0;
		virtual IType*			matrix_operator(const IType &other) const = 0;
		virtual IType*			sqrt(void) const = 0;
		virtual Rational*		gcd(const IType &other) const = 0;
		virtual std::ostream	&print(std::ostream &os) const = 0;
		virtual std::string		to_string(void) const = 0;
		virtual bool			in_D(void) const = 0;
		virtual bool			in_Q(void) const = 0;
		virtual bool			in_Z(void) const = 0;
		virtual void			print_rounded(const std::string var = std::string()) const = 0;

	protected:
		// Constructor
		IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
