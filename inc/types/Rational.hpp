/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:43:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 18:45:51 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RATIONAL_HPP
# define RATIONAL_HPP


class	Rational: public IType
{
	public:
		// Constructors and destructor
		~Rational(void) {};

		// Operator overloads
		Rational	&operator=(const Rational &other);
		// AST			*operator+(const IType &other) const;
		// AST			*operator+(const Rational &other) const;
		// AST			*operator+(const Rational &other) const;
		// AST			*operator-(const IType &other) const;
		// AST			*operator-(const Rational &other) const;
		// AST			*operator-(const Rational &other) const;
		// AST			*operator*(const IType &other) const;
		// AST			*operator*(const Rational &other) const;
		// AST			*operator*(const Rational &other) const;
		// AST			*operator/(const IType &other) const;
		// AST			*operator/(const Rational &other) const;
		// AST			*operator/(const Rational &other) const;
		// AST			*operator%(const IType &other) const;
		// AST			*operator^(const IType &other) const;
		// AST			*operator^(const Rational &other) const;
		// AST			*operator^(const Rational &other) const;

		// Getters
		// double	get_imaginary(void) const;
		// double	get_real(void) const;

		// Methods
		AST*	matrix_operator(const IType &other) const;

	private:
		// Members
		int	_numerator;
		int	_denominator;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Rational &num);

#endif
