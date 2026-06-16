/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/16 17:48:50 by qpupier          ###   ########lyon.fr   */
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

		// Methods
		virtual IType*			clone(void) const = 0;
		virtual IType*			function_operator(const IType &other) const = 0;
		virtual IType*			matrix_inversion(void) const = 0;
		virtual IType*			matrix_operator(const IType &other) const = 0;
		virtual Rational*		gcd(const IType &other) const = 0;
		virtual std::ostream	&print(std::ostream &os) const = 0;
		virtual std::string		to_string(void) const = 0;
		virtual void			print_rounded(const std::string var = std::string()) const = 0;

	protected:
		// Constructor
		IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
