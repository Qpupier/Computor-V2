/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 16:25:31 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITYPE_HPP
# define ITYPE_HPP

# include <iostream>

# include "InfiniteDecimal.hpp"

class	Rational;
class	Real;

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

		// Enums
		typedef enum	e_type
		{
			E_TYPE_RATIONAL,
			E_TYPE_COMPLEX,
			E_TYPE_MATRIX,
			E_TYPE_POLYNOMIAL,
			E_TYPE_VECTOR,
			E_TYPE_REAL,
			E_TYPE_BOOLEAN,
			E_TYPE_OPERATOR,
			E_TYPE_FUNCTION
		}				t_type;

		// Getters
		virtual t_type	getType(void) const = 0;

		// Methods
		virtual std::string		to_string(void) const = 0;
		virtual bool			in_C(void) const = 0;
		virtual bool			in_D(void) const = 0;
		virtual bool			in_M(void) const = 0;
		virtual bool			in_Q(void) const = 0;
		virtual bool			in_Z(void) const = 0;
		virtual std::ostream&	print(std::ostream &os) const = 0;
		virtual IType*			abs(void) const = 0;
		virtual IType*			clone(void) const = 0;
		virtual IType*			cos(void) const = 0;
		virtual IType*			exp(void) const = 0;
		virtual IType*			function_operator(const IType &other) const = 0;
		virtual IType*			matrix_inversion(void) const = 0;
		virtual IType*			matrix_operator(const IType &other) const = 0;
		virtual IType*			norm(void) const = 0;
		virtual IType*			sin(void) const = 0;
		virtual IType*			sqrt(void) const = 0;
		virtual IType*			tan(void) const = 0;
		virtual Rational*		fact(void) const = 0;
		virtual Rational*		gcd(const IType &other) const = 0;
		virtual Real*			deg(void) const = 0;
		virtual Real*			rad(void) const = 0;
		virtual void			print_rounded(const std::string var 	\
				= std::string()) const = 0;


		protected:
			// Constructor
			IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
