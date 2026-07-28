/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Real.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 17:41:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/28 16:13:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REAL_HPP
# define REAL_HPP

# include "IType.hpp"
# include "InfiniteFloat.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Polynomial.hpp"

class	Real: public IType
{
	// Constructors and destructor
	public:
		Real(void): _value(InfiniteFloat()) {};
		Real(const Real& other): _value(other._value) {};
		Real(const InfiniteFloat& value): _value(value) {};
		Real(const long long int value): _value(InfiniteFloat(value)) {};
		Real(const IType& other);
		~Real(void) {};

		// Operator overloads
		explicit	operator bool() const;
		Real&		operator=(const Real& other);
		Real&		operator=(const Real* other);
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
		IType*		operator+(const Real& other) const;
		IType*		operator+(const long long int value) const;
		IType*		operator-(void) const;
		IType*		operator-(const IType& other) const;
		IType*		operator-(const Real& other) const;
		IType*		operator-(const long long int value) const;
		IType*		operator*(const IType& other) const;
		IType*		operator*(const Real& other) const;
		IType*		operator*(const long long int value) const;
		IType*		operator/(const IType& other) const;
		IType*		operator/(const Real& other) const;
		IType*		operator/(const Rational& other) const;
		IType*		operator/(const Complex& other) const;
		IType*		operator/(const Matrix& other) const;
		IType*		operator/(const Polynomial& other) const;
		IType*		operator/(const Vector& other) const;
		IType*		operator/(const long long int value) const;
		IType*		operator%(const IType& other) const;
		IType*		operator%(const Real& other) const;
		IType*		operator%(const Rational& other) const;
		IType*		operator%(const Complex& other) const;
		IType*		operator%(const Matrix& other) const;
		IType*		operator%(const Polynomial& other) const;
		IType*		operator%(const Vector& other) const;
		IType*		operator%(const long long int value) const;
		IType*		operator^(const IType& other) const;
		IType*		operator^(const long long int value) const;

		// Getters
		InfiniteFloat&	getValue(void) const;

		// Methods
		std::string		to_string(void) const;
		bool			in_D(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			clone(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			sqrt(void) const;
		Rational*		gcd(const IType &other) const;
		Real*			abs(void) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		// Members
		InfiniteFloat	_value;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Real &num);

#endif
