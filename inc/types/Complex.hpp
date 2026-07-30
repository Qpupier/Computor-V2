/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:35:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/30 11:20:45 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEX_HPP
# define COMPLEX_HPP

# include "IType.hpp"
# include "Rational.hpp"
# include "Matrix.hpp"
# include "Polynomial.hpp"
# include "Vector.hpp"
# include "Complex.hpp"

class	Complex: public IType
{
	public:
		// Constructors and destructor
		Complex(void): _real(new Rational(0)), _imaginary(new Rational(1)) {};
		Complex(IType* real, IType* imaginary): _real(real), _imaginary(imaginary) {};
		Complex(const Complex &other): _real(other._real->clone()), _imaginary(other._imaginary->clone()) {};
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
		IType*		operator+(const Polynomial &other) const;
		Vector*		operator+(const Vector &other) const;
		Complex*	operator+(const Real &other) const;
		Complex*	operator+(const long long int value) const;
		Complex*	operator-(void) const;
		IType*		operator-(const IType &other) const;
		Complex*	operator-(const Complex &other) const;
		Complex*	operator-(const Rational &other) const;
		Matrix*		operator-(const Matrix &other) const;
		IType*		operator-(const Polynomial &other) const;
		Vector*		operator-(const Vector &other) const;
		Complex*	operator-(const Real &other) const;
		Complex*	operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Complex*	operator*(const Complex &other) const;
		Complex*	operator*(const Rational &other) const;
		Matrix*		operator*(const Matrix &other) const;
		IType*		operator*(const Polynomial &other) const;
		Vector*		operator*(const Vector &other) const;
		Complex*	operator*(const Real &other) const;
		Complex*	operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Complex*	operator/(const Complex &other) const;
		Complex*	operator/(const Rational &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Vector*		operator/(const Vector &other) const;
		Complex*	operator/(const Real &other) const;
		Complex*	operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Complex*	operator%(const Rational &other) const;
		Complex*	operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Vector*		operator%(const Vector &other) const;
		Complex*	operator%(const Real &other) const;
		Complex*	operator%(const long long int value) const;
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
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			arccos(void) const;
		IType*			arcsin(void) const;
		IType*			arctan(void) const;
		IType*			clone(void) const;
		IType*			cos(void) const;
		IType*			e(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			norm(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Rational*		gcd(const IType &other) const;
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
