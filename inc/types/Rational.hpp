/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/04 14:37:59 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

# include "computor-v2.hpp"

class	Complex;
class	Matrix;
class	Polynomial;

class	Rational: public IType
{
	public:
		// Constructors and destructor
		Rational(void): _numerator(0), _denominator(1) {};
		Rational(int numerator): _numerator(numerator), _denominator(1) {};
		Rational(int numerator, int denominator);
		Rational(std::string str);
		Rational(const Token &token): Rational(token.getValue()) {};
		Rational(const Rational &other): _numerator(other._numerator), _denominator(other._denominator) {};
		Rational(const IType &other);
		~Rational(void) {};

		// Operator overloads
		Rational&	operator=(const Rational &other);
		Rational&	operator=(const Rational *other);
		Rational	operator=(const Complex &other);
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const Rational &other) const;
		bool		operator==(const Complex &other) const;
		bool		operator==(const Matrix &other) const;
		bool		operator==(const Polynomial &other) const;
		bool		operator!=(const IType&other) const;
		bool		operator<(const IType &other) const;
		bool		operator<(const Rational &other) const;
		bool		operator<(const Complex &other) const;
		bool		operator<(const Matrix &other) const;
		bool		operator<(const Polynomial &other) const;
		bool		operator<=(const IType &other) const;
		bool		operator<=(const Rational &other) const;
		bool		operator<=(const Complex &other) const;
		bool		operator<=(const Matrix &other) const;
		bool		operator<=(const Polynomial &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>(const Rational &other) const;
		bool		operator>(const Complex &other) const;
		bool		operator>(const Matrix &other) const;
		bool		operator>(const Polynomial &other) const;
		bool		operator>=(const IType &other) const;
		bool		operator>=(const Rational &other) const;
		bool		operator>=(const Complex &other) const;
		bool		operator>=(const Matrix &other) const;
		bool		operator>=(const Polynomial &other) const;
		IType*		operator+(const IType &other) const;
		Rational*	operator+(const Rational &other) const;
		Complex*	operator+(const Complex &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		IType*		operator-(const IType &other) const;
		Rational*	operator-(const Rational &other) const;
		Complex*	operator-(const Complex &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		IType*		operator*(const IType &other) const;
		Rational*	operator*(const Rational &other) const;
		Complex*	operator*(const Complex &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		IType*		operator/(const IType &other) const;
		Rational*	operator/(const Rational &other) const;
		Complex*	operator/(const Complex &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		IType*		operator^(const IType &other) const;
		// Rational*	operator^(const Rational &other) const;
		// Complex*	operator^(const Complex &other) const;
		// Matrix*		operator^(const Matrix &other) const;
		// IType*		operator^(const Polynomial &other) const;

		// Getters
		int		getNumerator(void) const;
		int		getDenominator(void) const;
		double	getValue(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Complex &other) const;
		Rational*		gcd(const Matrix &other) const;
		std::ostream&	print(std::ostream &os) const;
		std::ostream&	print_polynomial(std::ostream &os, 	\
				const std::string &var) const;
		void			reduce(void);
		bool			is_integer(void) const;
		int				integer_part(void) const;

	private:
		// Members
		int	_numerator;
		int	_denominator;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Rational &num);

// Functions
int	compute_gcd(int a, int b);

# include "Complex.hpp"
# include "Matrix.hpp"
# include "Polynomial.hpp"

#endif
