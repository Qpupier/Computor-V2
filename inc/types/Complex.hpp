/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/11 12:31:23 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_HPP
# define COMPLEX_HPP

# include "IType.hpp"
# include "Rational.hpp"
# include "Matrix.hpp"
# include "Polynomial.hpp"

class	Complex: public IType
{
	public:
		// Constructors and destructor
		Complex(void): _real(Rational(0)), _imaginary(Rational(1)) {};
		Complex(Rational real, Rational imaginary): _real(real), _imaginary(imaginary) {};
		Complex(const Complex &other): _real(other._real), _imaginary(other._imaginary) {};
		Complex(const IType &other);
		~Complex(void) {};

		// Operator overloads
		Complex&	operator=(const Complex &other);
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const Complex &other) const;
		bool		operator==(const Rational &other) const;
		bool		operator==(const Matrix &other) const;
		bool		operator==(const Polynomial &other) const;
		bool		operator!=(const IType &other) const;
		bool		operator<(const IType &other) const;
		bool		operator<=(const IType &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>=(const IType &other) const;
		IType*		operator+(const IType &other) const;
		Complex*	operator+(const Complex &other) const;
		Complex*	operator+(const Rational &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		IType*		operator-(const IType &other) const;
		Complex*	operator-(const Complex &other) const;
		Complex*	operator-(const Rational &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		IType*		operator*(const IType &other) const;
		Complex*	operator*(const Complex &other) const;
		Complex*	operator*(const Rational &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		IType*		operator/(const IType &other) const;
		Complex*	operator/(const Complex &other) const;
		Complex*	operator/(const Rational &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		IType*		operator^(const IType &other) const;

		// Getters
		Rational	getReal(void) const;
		Rational	getImaginary(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Complex &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Matrix &other) const;
		std::ostream&	print(std::ostream &os) const;
		void			print_variable(const std::string var) const;
		bool			finite_decimals(void) const;

	private:
		// Members
		Rational	_real;
		Rational	_imaginary;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num);

#endif
