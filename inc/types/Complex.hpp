/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 18:43:02 by qpupier          ###   ########lyon.fr   */
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
		Complex(void): _real(0), _imaginary(1) {};
		Complex(double real, double imaginary): _real(real), _imaginary(imaginary) {};
		Complex(Rational real, Rational imaginary): _real(real.get_value()), _imaginary(imaginary.get_value()) {};
		Complex(const std::string &real, const std::string &imaginary): _real(std::stod(real)), _imaginary(std::stod(imaginary)) {};
		Complex(const Token &token): Complex(token.getValue(), "0") {};
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
		double	get_imaginary(void) const;
		double	get_real(void) const;

		// Methods
		AST*	matrix_operator(const IType &other) const;

	private:
		// Members
		double	_real;
		double	_imaginary;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num);

#endif
