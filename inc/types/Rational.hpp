/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 15:15:20 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

# include "AST.hpp"

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
		IType*		operator+(const IType &other) const;
		Rational*	operator+(const Rational &other) const;
		IType*		operator-(const IType &other) const;
		Rational*	operator-(const Rational &other) const;
		IType*		operator*(const IType &other) const;
		Rational*	operator*(const Rational &other) const;
		IType*		operator/(const IType &other) const;
		Rational*	operator/(const Rational &other) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		IType*		operator^(const IType &other) const;
		Rational*	operator^(const Rational &other) const;

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

	private:
		// Members
		int	_numerator;
		int	_denominator;
};

#endif
