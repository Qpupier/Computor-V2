/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 15:53:22 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_HPP
# define COMPLEX_HPP

# include "IType.hpp"
# include "Rational.hpp"
# include "Matrix.hpp"
# include "Variable.hpp"

class	Complex: public IType
{
	public:
		// Constructors and destructor
		Complex(void): _real(Rational(0)), _imaginary(Rational(1)) {};
		Complex(Rational real, Rational imaginary): _real(real), _imaginary(imaginary) {};
		Complex(const Complex &other): _real(other._real), _imaginary(other._imaginary) {};
		~Complex(void) {};

		// Operator overloads
		Complex&	operator=(const Complex &other);
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const Complex &other) const;
		bool		operator==(const Rational &other) const;
		bool		operator==(const Matrix &other) const;
		bool		operator==(const Variable &other) const;
		bool		operator!=(const IType &other) const;
		bool		operator<(const IType &other) const;
		bool		operator<=(const IType &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>=(const IType &other) const;
		IType*		operator+(const IType &other) const;
		Complex*	operator+(const Complex &other) const;
		Complex*	operator+(const Rational &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Variable*	operator+(const Variable &other) const;
		IType*		operator-(const IType &other) const;
		Complex*	operator-(const Complex &other) const;
		Complex*	operator-(const Rational &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Variable*	operator-(const Variable &other) const;
		IType*		operator*(const IType &other) const;
		Complex*	operator*(const Complex &other) const;
		Complex*	operator*(const Rational &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Variable*	operator*(const Variable &other) const;
		IType*		operator/(const IType &other) const;
		Complex*	operator/(const Complex &other) const;
		Complex*	operator/(const Rational &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Variable*	operator/(const Variable &other) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Variable*	operator%(const Variable &other) const;
		IType*		operator^(const IType &other) const;
		Complex*	operator^(const Rational &other) const;
		Complex*	operator^(const Complex &other) const;
		Matrix*		operator^(const Matrix &other) const;
		Variable*	operator^(const Variable &other) const;

		// Getters
		Rational	getReal(void) const;
		Rational	getImaginary(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			clone(void) const;
		std::ostream&	print(std::ostream &os) const;
		std::ostream&	print_variable(std::ostream &os, 	\
				const std::string &var) const;

	private:
		// Members
		Rational	_real;
		Rational	_imaginary;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num);

#endif
