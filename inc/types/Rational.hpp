/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 18:08:44 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

// # include "AST.hpp"
# include "IType.hpp"
# include "Token.hpp"
# include "Complex.hpp"

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
		bool		operator!() const;
		Rational&	operator=(const Rational &other);
		Rational*	operator=(const Complex &other);
		Rational*	operator=(const Complex *other);
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
		Complex*	operator%(const Complex &other) const;
		IType*		operator^(const IType &other) const;
		Rational*	operator^(const Rational &other) const;
		Complex*	operator^(const Complex &other) const;

		// Getters
		int		get_numerator(void) const;
		int		get_denominator(void) const;
		double	get_number(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		int				integer_part(void) const;
		void			reduce(void);
		std::ostream&	print(std::ostream &os) const;
		IType*			clone(void) const;
		bool			is_integer(void) const;

	private:
		// Members
		int	_numerator;
		int	_denominator;
};

#endif
