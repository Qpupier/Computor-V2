/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Real.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 17:41:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/23 14:35:52 by qpupier          ###   ########lyon.fr   */
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
		Real*		operator+(const Real& other) const;
		Real*		operator+(const long long int value) const;
		IType*		operator-(void) const;
		IType*		operator-(const IType& other) const;
		Real*		operator-(const Real& other) const;
		Real*		operator-(const long long int value) const;
		IType*		operator*(const IType& other) const;
		Real*		operator*(const Real& other) const;
		Real*		operator*(const long long int value) const;
		IType*		operator/(const IType& other) const;
		Real*		operator/(const Real& other) const;
		Real*		operator/(const Rational& other) const;
		Complex*	operator/(const Complex& other) const;
		Matrix*		operator/(const Matrix& other) const;
		Polynomial*	operator/(const Polynomial& other) const;
		Vector*		operator/(const Vector& other) const;
		Real*		operator/(const long long int value) const;
		IType*		operator%(const IType& other) const;
		Real*		operator%(const Real& other) const;
		Real*		operator%(const Rational& other) const;
		Complex*	operator%(const Complex& other) const;
		Matrix*		operator%(const Matrix& other) const;
		Polynomial*	operator%(const Polynomial& other) const;
		Vector*		operator%(const Vector& other) const;
		Real*		operator%(const long long int value) const;
		IType*		operator^(const IType& other) const;
		Real*		operator^(const Rational& other) const;
		Real*		operator^(const long long int value) const;

		// Getters
		InfiniteFloat&	getValue(void) const;

		// Methods
		std::string		to_string(void) const;
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			abs(void) const;
		IType*			clone(void) const;
		IType*			cos(void) const;
		IType*			exp(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			norm(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Rational*		fact(void) const;
		Rational*		gcd(const IType &other) const;
		Real*			deg(void) const;
		Real*			rad(void) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		// Members
		InfiniteFloat	_value;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Real &num);

// Functions
Real*	get_pi(void);

#endif
