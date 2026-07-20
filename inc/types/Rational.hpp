/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/20 13:51:23 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

# include "Token.hpp"
# include "IType.hpp"

class	Complex;
class	Matrix;
class	Polynomial;
class	Vector;
class	Real;

class	Rational: public IType
{
	public:
		// Constructors and destructor
		Rational(void): 							\
				_numerator(InfiniteInt()), _denominator(InfiniteInt(1)) {};
		Rational(InfiniteInt numerator): 			\
				_numerator(numerator), _denominator(InfiniteInt(1)) {};
		Rational(const long long int numerator): 	\
				Rational(InfiniteInt(numerator)) {};
		Rational(InfiniteInt numerator, InfiniteInt denominator);
		Rational(std::string str);
		Rational(const Token &token): Rational(token.getValue()) {};
		Rational(const Rational &other): 			\
				_numerator(other._numerator), 		\
				_denominator(other._denominator) {};
		Rational(const IType &other);
		~Rational(void) {};

		// Operator overloads
		explicit	operator bool() const;
		Rational&	operator=(const Rational* other);
		Rational&	operator=(const Rational& other);
		Rational	operator=(const IType& other);
		Rational	operator=(const Complex& other);
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
		Rational*	operator+(const Rational& other) const;
		Complex*	operator+(const Complex& other) const;
		Matrix*		operator+(const Matrix& other) const;
		IType*		operator+(const Polynomial& other) const;
		Vector*		operator+(const Vector& other) const;
		Real*		operator+(const Real& other) const;
		Rational*	operator+(const long long int value) const;
		Rational*	operator-(void) const;
		IType*		operator-(const IType& other) const;
		Rational*	operator-(const Rational& other) const;
		Complex*	operator-(const Complex& other) const;
		Matrix*		operator-(const Matrix& other) const;
		IType*		operator-(const Polynomial& other) const;
		Vector*		operator-(const Vector& other) const;
		Real*		operator-(const Real& other) const;
		Rational*	operator-(const long long int value) const;
		IType*		operator*(const IType& other) const;
		Rational*	operator*(const Rational& other) const;
		Complex*	operator*(const Complex& other) const;
		Matrix*		operator*(const Matrix& other) const;
		IType*		operator*(const Polynomial& other) const;
		Vector*		operator*(const Vector& other) const;
		Real*		operator*(const Real& other) const;
		Rational*	operator*(const long long int value) const;
		IType*		operator/(const IType& other) const;
		Rational*	operator/(const Rational& other) const;
		Complex*	operator/(const Complex& other) const;
		Matrix*		operator/(const Matrix& other) const;
		Polynomial*	operator/(const Polynomial& other) const;
		Vector*		operator/(const Vector& other) const;
		Real*		operator/(const Real& other) const;
		Rational*	operator/(const long long int value) const;
		IType*		operator%(const IType& other) const;
		Rational*	operator%(const Rational& other) const;
		Complex*	operator%(const Complex& other) const;
		Matrix*		operator%(const Matrix& other) const;
		Polynomial*	operator%(const Polynomial& other) const;
		Vector*		operator%(const Vector& other) const;
		IType*		operator%(const Real& other) const;
		Rational*	operator%(const long long int value) const;
		IType*		operator^(const IType& other) const;
		Rational*	operator^(const Rational& other) const;
		Rational*	operator^(const long long int value) const;

		// Getters
		InfiniteInt		getNumerator(void) const;
		InfiniteInt		getDenominator(void) const;
		InfiniteFloat	getValue(void) const;

		// Methods
		InfiniteInt		integer_part(void) const;
		std::ostream&	print(std::ostream &os) const;
		std::string		to_string(void) const;
		IType*			clone(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			sqrt(void) const;
		Rational*		gcd(const IType& other) const;
		Rational*		gcd(const Rational& other) const;
		Rational*		gcd(const Complex& other) const;
		Rational*		gcd(const Matrix& other) const;
		Rational*		gcd(const Vector& other) const;
		bool			in_D(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		void			print_rounded	\
				(const std::string var = std::string()) const;
		void			reduce(void);

	private:
		// Members
		InfiniteInt	_numerator;
		InfiniteInt	_denominator;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Rational &num);

// Functions
InfiniteInt	compute_gcd(InfiniteInt a, InfiniteInt b);
Rational*	get_gcd_terms(const Rational* term1, const Rational* term2, 	\
		const Rational* term3, const Rational* term4);

# include "Complex.hpp"
# include "Matrix.hpp"
# include "Polynomial.hpp"
# include "Vector.hpp"
# include "Real.hpp"

#endif
