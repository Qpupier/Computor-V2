/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Boolean.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:26:22 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 16:49:23 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOOLEAN_HPP
# define BOOLEAN_HPP

# include "IType.hpp"

class	Boolean : public IType
{
	public:
		// Constructors and destructor
		Boolean(void): _value(false) {};
		Boolean(bool value): _value(value) {};
		Boolean(const Boolean& other): _value(other._value) {};
		~Boolean(void) {};

		// Operator overloads
		explicit	operator bool() const;
		Boolean&	operator=(const Boolean& other);
		Boolean		operator=(const IType& other);
		bool		operator==(const IType& other) const;
		bool		operator==(const long long int value) const;
		bool		operator!=(const IType& other) const;
		bool		operator!=(const long long int value) const;
		bool		operator<(const IType& other) const;
		bool		operator<(const long long int value) const;
		bool		operator<=(const IType& other) const;
		bool		operator<=(const long long int value) const;
		bool		operator>(const IType& other) const;
		bool		operator>(const long long int value) const;
		bool		operator>=(const IType& other) const;
		bool		operator>=(const long long int value) const;
		IType*		operator+(const IType& other) const;
		IType*		operator+(const long long int value) const;
		IType*		operator-(void) const;
		IType*		operator-(const IType& other) const;
		IType*		operator-(const long long int value) const;
		IType*		operator*(const IType& other) const;
		IType*		operator*(const long long int value) const;
		IType*		operator/(const IType& other) const;
		IType*		operator/(const long long int value) const;
		IType*		operator%(const IType& other) const;
		IType*		operator%(const long long int value) const;
		IType*		operator^(const IType& other) const;
		IType*		operator^(const long long int value) const;

		// Getters
		t_type	getType(void) const;
		bool	getValue(void) const;

		// Setters
		void	setValue(bool value);

		// Methods
		std::string		to_string(void) const;
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream& os) const;
		IType*			abs(void) const;
		IType*			clone(void) const;
		IType*			cos(void) const;
		IType*			exp(void) const;
		IType*			function_operator(const IType& other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType& other) const;
		IType*			norm(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Rational*		fact(void) const;
		Rational*		gcd(const IType& other) const;
		Real*			deg(void) const;
		Real*			rad(void) const;
		void			print_rounded(const std::string var 	\
				= std::string()) const;

	private:
		// Members
		bool	_value;
};

// Output stream operator overload
std::ostream& operator<<(std::ostream& os, const IType& type);

#endif
