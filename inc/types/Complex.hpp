/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 13:43:54 by qpupier          ###   ########lyon.fr   */
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
		explicit	operator bool() const;
		Complex&	operator=(const Complex &other);
		Complex		operator=(const IType &other);
		bool		operator==(const IType &other) const;
		bool		operator==(const long long int value) const;
		bool		operator!=(const IType &other) const;
		bool		operator!=(const long long int value) const;
		bool		operator<(const IType &other) const;
		bool		operator<(const long long int value) const;
		bool		operator<=(const IType &other) const;
		bool		operator<=(const long long int value) const;
		bool		operator>(const IType &other) const;
		bool		operator>(const long long int value) const;
		bool		operator>=(const IType &other) const;
		bool		operator>=(const long long int value) const;
		IType*		operator+(const IType &other) const;
		Complex*	operator+(const Complex &other) const;
		Complex*	operator+(const Rational &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		Complex*	operator+(const long long int value) const;
		Complex*	operator-(void) const;
		IType*		operator-(const IType &other) const;
		Complex*	operator-(const Complex &other) const;
		Complex*	operator-(const Rational &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		Complex*	operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Complex*	operator*(const Complex &other) const;
		Complex*	operator*(const Rational &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		Complex*	operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Complex*	operator/(const Complex &other) const;
		Complex*	operator/(const Rational &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Complex*	operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Rational*	operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		Complex*	operator^(const Rational &other) const;
		Complex*	operator^(const long long int value) const;

		// Getters
		Rational	getReal(void) const;
		Rational	getImaginary(void) const;

		// Setters
		void	setReal(const Rational &real);
		void	setImaginary(const Rational &imaginary);

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Complex &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Matrix &other) const;
		std::ostream&	print(std::ostream &os) const;
		std::string		to_string(void) const;
		bool			values_in_D(void) const;
		bool			values_in_Z(void) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		// Members
		Rational	_real;
		Rational	_imaginary;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num);


// Functions
void	print_complex_rounded_value(const std::string var, const InfiniteDouble & real, const InfiniteDouble & imaginary);

#endif
