/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/29 11:30:57 by qpupier          ###   ########lyon.fr   */
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
		virtual bool		operator<(const IType &other) const = 0;// [ ] Garder ces 4-la ?
		virtual bool		operator<=(const IType &other) const = 0;
		virtual bool		operator>(const IType &other) const = 0;
		virtual bool		operator>=(const IType &other) const = 0;
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
		virtual IType*			matrix_operator(const IType &other) const = 0;
		virtual IType*			function_operator(const IType &other) const = 0;
		virtual IType*			clone(void) const = 0;
		virtual Rational*		gcd(const IType &other) const = 0;
		virtual std::ostream	&print(std::ostream &os) const = 0;
		virtual void			print_rounded(const std::string var = std::string()) const = 0;
		virtual bool			finite_decimals(void) const = 0;// [ ]: A retirer ?

	protected:
		// Constructor
		IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
