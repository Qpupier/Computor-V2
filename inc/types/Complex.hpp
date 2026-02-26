/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 18:10:12 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_HPP
# define COMPLEX_HPP

// # include "AST.hpp"
# include "IType.hpp"
# include "Rational.hpp"

// class	Rational;

class	Complex: public IType
{
	public:
		// Constructors and destructor
		Complex(void): _real(Rational(0)), _imaginary(Rational(1)) {};
		Complex(Rational real, Rational imaginary): _real(real), _imaginary(imaginary) {};
		// Complex(const std::string &real, const std::string &imaginary): _real(real), _imaginary(imaginary) {};
		// Complex(const Token &token): Complex(token.getValue(), Rational(0)) {};
		// Complex(const AST *node): Complex(node->getToken()) {};
		Complex(const Complex &other): _real(other._real), _imaginary(other._imaginary) {};
		~Complex(void) {};

		// Operator overloads
		bool		operator!() const;
		Complex&	operator=(const Complex &other);
		IType*		operator+(const IType &other) const;
		Complex*	operator+(const Complex &other) const;
		Complex*	operator+(const Rational &other) const;
		IType*		operator-(const IType &other) const;
		Complex*	operator-(const Complex &other) const;
		Complex*	operator-(const Rational &other) const;
		IType*		operator*(const IType &other) const;
		Complex*	operator*(const Complex &other) const;
		Complex*	operator*(const Rational &other) const;
		IType*		operator/(const IType &other) const;
		Complex*	operator/(const Complex &other) const;
		Complex*	operator/(const Rational &other) const;
		IType*		operator%(const IType &other) const;
		Complex*	operator%(const Rational &other) const;
		Complex*	operator%(const Complex &other) const;
		IType*		operator^(const IType &other) const;
		Complex*	operator^(const Rational &other) const;
		Complex*	operator^(const Complex &other) const;

		// Getters
		Rational	get_imaginary(void) const;
		Rational	get_real(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		std::ostream	&print(std::ostream &os) const;
		IType*			clone(void) const;

	private:
		// Members
		Rational	_real;
		Rational	_imaginary;
};

// Output stream operator overload
// std::ostream	&operator<<(std::ostream &os, const Complex &num);

#endif
