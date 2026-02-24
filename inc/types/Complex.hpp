/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 19:59:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_HPP
# define COMPLEX_HPP

// # include "IType.hpp"
# include "AST.hpp"
# include "Rational.hpp"

class	Complex: public IType
{
	public:
		// Constructors and destructor
		Complex(void): _real(Rational(0)), _imaginary(Rational(1)) {};
		Complex(Rational real, Rational imaginary): _real(real), _imaginary(imaginary) {};
		// Complex(const std::string &real, const std::string &imaginary): _real(real), _imaginary(imaginary) {};
		Complex(const Token &token): Complex(token.getValue(), Rational(0)) {};
		// Complex(const AST *node): Complex(node->getToken()) {};
		Complex(const Complex &other): _real(other._real), _imaginary(other._imaginary) {};
		~Complex(void) {};

		// Operator overloads
		Complex	&operator=(const Complex &other);
		AST		*operator+(const IType &other) const;
		AST		*operator+(const Complex &other) const;
		AST		*operator+(const Rational &other) const;
		AST		*operator-(const IType &other) const;
		AST		*operator-(const Complex &other) const;
		AST		*operator-(const Rational &other) const;
		AST		*operator*(const IType &other) const;
		AST		*operator*(const Complex &other) const;
		AST		*operator*(const Rational &other) const;
		AST		*operator/(const IType &other) const;
		AST		*operator/(const Complex &other) const;
		AST		*operator/(const Rational &other) const;
		AST		*operator%(const IType &other) const;
		AST		*operator^(const IType &other) const;
		AST		*operator^(const Complex &other) const;
		AST		*operator^(const Rational &other) const;

		// Getters
		Rational	get_imaginary(void) const;
		Rational	get_real(void) const;

		// Methods
		AST*	matrix_operator(const IType &other) const;

	private:
		// Members
		Rational	_real;
		Rational	_imaginary;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num);

#endif
