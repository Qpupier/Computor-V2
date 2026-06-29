/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/29 16:19:17 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_HPP
# define COMPLEX_HPP

# include "IType.hpp"
# include "Rational.hpp"
# include "Matrix.hpp"
# include "Vector.hpp"
# include "Polynomial.hpp"

class	Complex: public IType
{
	public:
		// Constructors and destructor
		Complex(void): _real(new Rational(0)), _imaginary(new Rational(1)) {};
		Complex(IType* real, IType* imaginary): _real(real), _imaginary(imaginary) {};
		Complex(const Complex &other): _real(other._real), _imaginary(other._imaginary) {};
		Complex(const IType &other);
		~Complex(void);

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
		Vector*		operator+(const Vector &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		Complex*	operator+(const long long int value) const;
		Complex*	operator-(void) const;
		IType*		operator-(const IType &other) const;
		Complex*	operator-(const Complex &other) const;
		Complex*	operator-(const Rational &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Vector*		operator-(const Vector &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		Complex*	operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Complex*	operator*(const Complex &other) const;
		Complex*	operator*(const Rational &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Vector*		operator*(const Vector &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		Complex*	operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Complex*	operator/(const Complex &other) const;
		Complex*	operator/(const Rational &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Vector*		operator/(const Vector &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Complex*	operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Rational*	operator%(const Rational &other) const;
		Rational*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Vector*		operator%(const Vector &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Rational*	operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		Complex*	operator^(const Rational &other) const;
		Complex*	operator^(const long long int value) const;

		// Getters
		IType*	getReal(void) const;
		IType*	getImaginary(void) const;

		// Setters
		void	setReal(IType* real);
		void	setImaginary(IType* imaginary);

		// Methods
		std::string		to_string(void) const;
		std::ostream&	print(std::ostream &os) const;
		bool			in_D(void) const;
		bool			in_Z(void) const;
		IType*			norm(void) const;
		IType*			clone(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Complex &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Matrix &other) const;
		Rational*		gcd(const Vector &other) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		// Members
		IType*	_real;
		IType*	_imaginary;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Complex &num);


// Functions
void	print_complex_rounded_value(const std::string var, const InfiniteFloat & real, const InfiniteFloat & imaginary);

#endif
