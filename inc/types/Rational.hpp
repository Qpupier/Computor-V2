/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/05 18:07:28 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

# include "computor-v2.hpp"
// # include "Matrix.hpp"

class	Complex;
class	Matrix;

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
		~Rational(void) {};

		// Operator overloads
		Rational&	operator=(const Rational &other);
		Rational&	operator=(const Rational *other);
		Rational	operator=(const Complex &other);
		explicit 	operator bool() const;
		bool		operator==(const Rational &other) const;
		bool		operator!=(const Rational &other) const;
		bool		operator<(const Rational &other) const;
		bool		operator<=(const Rational &other) const;
		bool		operator>(const Rational &other) const;
		bool		operator>=(const Rational &other) const;
		IType*		operator+(const IType &other) const;
		Rational*	operator+(const Rational &other) const;
		Complex*	operator+(const Complex &other) const;
		Matrix*		operator+(const Matrix &other) const;
		IType*		operator-(const IType &other) const;
		Rational*	operator-(const Rational &other) const;
		Complex*	operator-(const Complex &other) const;
		Matrix*		operator-(const Matrix &other) const;
		IType*		operator*(const IType &other) const;
		Rational*	operator*(const Rational &other) const;
		Complex*	operator*(const Complex &other) const;
		Matrix*		operator*(const Matrix &other) const;
		IType*		operator/(const IType &other) const;
		Rational*	operator/(const Rational &other) const;
		Complex*	operator/(const Complex &other) const;
		Matrix*		operator/(const Matrix &other) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		IType*		operator^(const IType &other) const;
		Rational*	operator^(const Rational &other) const;
		Complex*	operator^(const Complex &other) const;
		Matrix*		operator^(const Matrix &other) const;

		// Getters
		int		getNumerator(void) const;
		int		getDenominator(void) const;
		double	getNumber(void) const;

		// Methods
		void			reduce(void);
		IType*			matrix_operator(const IType &other) const;
		IType*			clone(void) const;
		bool			is_integer(void) const;
		int				integer_part(void) const;
		std::ostream&	print(std::ostream &os) const;

	private:
		// Members
		int	_numerator;
		int	_denominator;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Rational &num);

# include "Complex.hpp"
# include "Matrix.hpp"

#endif
