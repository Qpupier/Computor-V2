/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/27 17:19:05 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

# include "IType.hpp"
# include "Token.hpp"

class	Complex;

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
		explicit 	operator bool() const;
		bool		operator!() const;
		bool		operator==(const Rational &other) const;
		bool		operator!=(const Rational &other) const;
		bool		operator<(const Rational &other) const;
		bool		operator<=(const Rational &other) const;
		bool		operator>(const Rational &other) const;
		bool		operator>=(const Rational &other) const;
		Rational&	operator=(const Rational &other);
		Rational	operator=(const Complex &other);
		IType*		operator+(const IType &other) const;
		Rational*	operator+(const Rational &other) const;
		Complex*	operator+(const Complex &other) const;
		IType*		operator-(const IType &other) const;
		Rational*	operator-(const Rational &other) const;
		Complex*	operator-(const Complex &other) const;
		IType*		operator*(const IType &other) const;
		Rational*	operator*(const Rational &other) const;
		Complex*	operator*(const Complex &other) const;
		IType*		operator/(const IType &other) const;
		Rational*	operator/(const Rational &other) const;
		Complex*	operator/(const Complex &other) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		IType*		operator^(const IType &other) const;
		Rational*	operator^(const Rational &other) const;
		Complex*	operator^(const Complex &other) const;

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

#endif
