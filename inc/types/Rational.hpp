/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/25 17:48:34 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "AST.hpp"
# include "computor-v2.hpp"

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
		Rational	&operator=(const Rational &other);
		AST			*operator+(const IType &other) const;
		AST			*operator+(const Rational &other) const;
		// AST			*operator+(const Rational &other) const;
		AST			*operator-(const IType &other) const;
		AST			*operator-(const Rational &other) const;
		// AST			*operator-(const Rational &other) const;
		AST			*operator*(const IType &other) const;
		AST			*operator*(const Rational &other) const;
		// AST			*operator*(const Rational &other) const;
		AST			*operator/(const IType &other) const;
		AST			*operator/(const Rational &other) const;
		// AST			*operator/(const Rational &other) const;
		AST			*operator%(const IType &other) const;
		AST			*operator%(const Rational &other) const;
		AST			*operator^(const IType &other) const;
		AST			*operator^(const Rational &other) const;
		// AST			*operator^(const Rational &other) const;

		// Getters
		int		get_numerator(void) const;
		int		get_denominator(void) const;
		double	get_number(void) const;

		// Methods
		AST*			matrix_operator(const IType &other) const;
		int				integer_part(void) const;
		void			reduce(void);
		std::ostream	&print(std::ostream &os) const;

	private:
		// Members
		int	_numerator;
		int	_denominator;
};

#endif
