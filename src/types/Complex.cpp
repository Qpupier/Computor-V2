/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 17:09:46 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils

static std::ostream&	print_value(std::ostream &os, Rational value, 	\
		const std::string &i, bool is_first)
{
	Rational	copy(value);
	bool		negative_between(false);

	if (!value)
		return (os);
	if (!is_first)
	{
		if (value < 0)
			negative_between = true;
		else
			os << " + ";
	}
	if (negative_between || (is_first && value < 0 && !i.empty()))
	{
		os << (negative_between ? " - " : "-");
		copy = -value;
	}
	if (i.empty() || copy != 1)
		os << copy.getNumerator();
	if (!i.empty())
		os << i;
	if (copy.getDenominator() != 1)
		os << "/" << copy.getDenominator();
	return (os);
}

static Rational*		division_real_part(const Complex& a, 			\
		const Complex& b, const Rational& denominator)
{
	Rational*	part1;
	Rational*	part2;
	Rational*	numerator;
	Rational*	real;

	part1 = a.getReal() * b.getReal();
	part2 = a.getImaginary() * b.getImaginary();
	numerator = *part1 + *part2;
	delete part1;
	delete part2;
	real = *numerator / denominator;
	delete numerator;
	return (real);
}

static Rational*		division_imaginary_part(const Complex& a, 		\
		const Complex& b, const Rational& denominator)
{
	Rational*	part1;
	Rational*	part2;
	Rational*	numerator;
	Rational*	imaginary;

	part1 = a.getImaginary() * b.getReal();
	part2 = a.getReal() * b.getImaginary();
	numerator = *part1 - *part2;
	delete part1;
	delete part2;
	imaginary = *numerator / denominator;
	delete numerator;
	return (imaginary);
}

static Complex*			division(const Complex& a, const Complex& b, 	\
		const Rational& denominator)
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	result = new Complex();
	real = division_real_part(a, b, denominator);
	result->setReal(*real);
	delete real;
	imaginary = division_imaginary_part(a, b, denominator);
	result->setImaginary(*imaginary);
	delete imaginary;
	return (result);
}

static void				from_polynomial(Complex &complex, 				\
		const Polynomial &polynomial)
{
	if (polynomial.getTerms().empty())
	{
		complex.setReal(Rational(0));
		complex.setImaginary(Rational(0));
	}
	else
	{
		if (polynomial.getDividers().size() != 1 					\
				|| *polynomial.getDividers()[0].coefficient != 1 	\
				|| polynomial.getDividers()[0].power 				\
				|| polynomial.getTerms().size() != 1 				\
				|| polynomial.getTerms()[0].power)
			throw ERROR_UNEXPECTED;
		complex.setReal(Rational(*polynomial.getTerms()[0].coefficient));
		complex.setImaginary(Rational(0));
	}
}

static void				print_complex_rounded_value_default(			\
		const InfiniteDouble& real, const InfiniteDouble& imaginary)
{
	InfiniteDouble	tmp;

	std::cout << real;
	if (imaginary < 0)
	{
		std::cout << " - ";
		tmp = -imaginary;
	}
	else
	{
		std::cout << " + ";
		tmp = imaginary;
	}
	std::cout << tmp << "i";
}


// Constructors

Complex::Complex(const IType &other)
{
	const Complex*		other_complex(dynamic_cast<const Complex*>(&other));
	const Rational*		other_rational(dynamic_cast<const Rational*>(&other));
	const Polynomial*	other_polynomial								\
			(dynamic_cast<const Polynomial*>(&other));

	if (other_complex)
		*this = *other_complex;
	else if (other_rational)
	{
		this->_real = *other_rational;
		this->_imaginary = Rational(0);
	}
	else if (other_polynomial)
		from_polynomial(*this, *other_polynomial);
	else
		throw ERROR_UNEXPECTED;
}


// Operator overloads

Complex&	Complex::operator=(const Complex &other)
{
	if (this != &other)
	{
		this->_real = other._real;
		this->_imaginary = other._imaginary;
	}
	return (*this);
}

Complex		Complex::operator=(const IType &other)
{
	*this = Complex(other);
	return (*this);
}

inline		Complex::operator bool() const
{
	return (this->_real || this->_imaginary);
}

bool		Complex::operator==(const IType &other) const
{
	Complex	other_complex;

	try
	{
		other_complex = Complex(other);
	}
	catch(const UnexpectedError &e)
	{
		return (false);
	}
	return (this->_real == other_complex._real 	\
			&& this->_imaginary == other_complex._imaginary);
}

bool		Complex::operator==(const long long int value) const
{
	return (*this == Rational(value));
}

bool		Complex::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Complex::operator!=(const long long int value) const
{
	return (*this != Rational(value));
}

bool		Complex::operator<(const IType &other) const
{
	if (!other)
		return (this->_imaginary < 0 && this->_real < 0);
	return (!this->_imaginary && this->_real < other);
}

bool		Complex::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Complex::operator<=(const IType &other) const
{
	return (!this->_imaginary && this->_real <= other);
}

bool		Complex::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Complex::operator>(const IType &other) const
{
	return (!this->_imaginary && this->_real > other);
}

bool		Complex::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Complex::operator>=(const IType &other) const
{
	return (!this->_imaginary && this->_real >= other);
}

bool		Complex::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

Complex*	Complex::operator+(const Complex &other) const
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	real = this->_real + other._real;
	imaginary = this->_imaginary + other._imaginary;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator+(const Rational &other) const
{
	return (*this + Complex(other, Rational()));
}

Matrix*		Complex::operator+(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational + other);
}

Vector*		Complex::operator+(const Vector &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational + other);
}

Polynomial*	Complex::operator+(const Polynomial &other) const
{
	return (other + *this);
}

IType*		Complex::operator+(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator+(const long long int value) const
{
	return (*this + Rational(value));
}

Complex*	Complex::operator-(void) const
{
	return (*this * (-1));
}

Complex*	Complex::operator-(const Complex &other) const
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	real = this->_real - other._real;
	imaginary = this->_imaginary - other._imaginary;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator-(const Rational &other) const
{
	return (*this - Complex(other, Rational()));
}

Matrix*		Complex::operator-(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational - other);
}

Vector*		Complex::operator-(const Vector &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational - other);
}

Polynomial*	Complex::operator-(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

IType*		Complex::operator-(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

Complex*	Complex::operator*(const Complex &other) const
{
	Rational*	part1;
	Rational*	part2;
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	part1 = this->_real * other._real;
	part2 = this->_imaginary * other._imaginary;
	real = *part1 - *part2;
	delete part1;
	delete part2;
	part1 = this->_real * other._imaginary;
	part2 = this->_imaginary * other._real;
	imaginary = *part1 + *part2;
	delete part1;
	delete part2;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Complex*	Complex::operator*(const Rational &other) const
{
	Rational*	real;
	Rational*	imaginary;
	Complex*	result;

	real = this->_real * other;
	imaginary = this->_imaginary * other;
	result = new Complex(*real, *imaginary);
	delete real;
	delete imaginary;
	return (result);
}

Matrix*		Complex::operator*(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational * other);
}

Vector*		Complex::operator*(const Vector &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational * other);
}

Polynomial*	Complex::operator*(const Polynomial &other) const
{
	return (other * *this);
}

IType*		Complex::operator*(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

Complex*	Complex::operator/(const Complex &other) const
{
	Rational*	part1;
	Rational*	part2;
	Rational*	denominator;
	Complex*	result;

	part1 = other._real * other._real;
	part2 = other._imaginary * other._imaginary;
	denominator = *part1 + *part2;
	delete part1;
	delete part2;
	if (!*denominator)
	{
		delete denominator;
		throw ERROR_DIVISION_BY_ZERO;
	}
	result = division(*this, other, *denominator);
	delete denominator;
	return (result);
}

Complex*	Complex::operator/(const Rational &other) const
{
	return (*this / Complex(other, Rational()));
}

Matrix*		Complex::operator/(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational / other);
}

Vector*	Complex::operator/(const Vector &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (rational / other);
}

Polynomial*	Complex::operator/(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

IType*		Complex::operator/(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

Rational*	Complex::operator%(const Complex &other) const
{
	Rational	rational;
	Rational	other_rational;

	try
	{
		rational = *this;
		other_rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other_rational);
}

Rational*	Complex::operator%(const Rational &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch(const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other);
}

Matrix*		Complex::operator%(const Matrix &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch(const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other);
}

Vector*	Complex::operator%(const Vector &other) const
{
	Rational	rational;

	try
	{
		rational = *this;
	}
	catch(const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (rational % other);
}

Polynomial*	Complex::operator%(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

IType*		Complex::operator%(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Complex::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

Complex*	Complex::operator^(const Rational &other) const
{
	Complex*	result;
	Complex*	tmp;

	if (!other.in_Z() || other < 0)
		throw UNSUPPORTED_EXPONENT;
	result = new Complex(Rational(1), Rational(0));
	for (InfiniteInt i(0); i < other.getNumerator(); i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

IType*		Complex::operator^(const IType &other) const
{
	Rational	power;

	try
	{
		power = Rational(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (*this ^ power);
}

Complex*	Complex::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
}


// Getters

Rational	Complex::getImaginary(void) const
{
	return (this->_imaginary);
}

Rational	Complex::getReal(void) const
{
	return (this->_real);
}


// Setters

void	Complex::setReal(const Rational &real)
{
	this->_real = real;
}

void	Complex::setImaginary(const Rational &imaginary)
{
	this->_imaginary = imaginary;
}


// Methods

IType*			Complex::clone(void) const
{
	return (new Complex(*this));
}

IType*			Complex::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Complex::matrix_operator(const IType &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	return (nullptr);
}

IType*			Complex::matrix_inversion(void) const
{
	throw ERROR_MATRIX_INVERSION_SQUARE;
	return (nullptr);
}

IType*			Complex::norm(void) const
{
	Rational*	real_squared;
	Rational*	imaginary_squared;
	Rational*	sum;
	Rational*	result;

	real_squared = this->_real * this->_real;
	imaginary_squared = this->_imaginary * this->_imaginary;
	sum = *real_squared + *imaginary_squared;
	delete real_squared;
	delete imaginary_squared;
	result = new Rational(*sum);
	delete sum;// TODO: SQRT
	return (result);
}

Rational*		Complex::gcd(const Rational &other) const
{
	return (other.gcd(*this));
}

Rational*		Complex::gcd(const Complex &other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(other.getReal());
	second = first->gcd(other.getImaginary());
	delete first;
	return (second);
}

Rational*		Complex::gcd(const Matrix &other) const
{
	Rational*	first;
	Rational*	second;

	first = other.gcd(this->getReal());
	second = first->gcd(this->getImaginary());
	delete first;
	return (second);
}

Rational*		Complex::gcd(const Vector &other) const
{
	Rational*	first;
	Rational*	second;

	first = other.gcd(this->getReal());
	second = first->gcd(this->getImaginary());
	delete first;
	return (second);
}

Rational*		Complex::gcd(const IType &other) const
{
	const Rational*	other_rational;
	const Complex*	other_complex;
	const Matrix*	other_matrix;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->gcd(*other_rational));
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->gcd(*other_complex));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->gcd(*other_matrix));
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

std::ostream&	Complex::print(std::ostream &os) const
{
	if (!this->_real && !this->_imaginary)
		return (os << "0");
	else if (this->_real < 0 && this->_imaginary > 0)
	{
		print_value(os, this->_imaginary, "i", true);
		print_value(os, this->_real, "", false);
	}
	else
	{
		print_value(os, this->_real, "", true);
		print_value(os, this->_imaginary, "i", !this->_real);
	}
	return (os);
}

std::string		Complex::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

void			print_complex_rounded_value(const std::string var, 	\
		const InfiniteDouble & real, const InfiniteDouble & imaginary)
{
	if (!var.empty())
	{
		std::cout << var;
		if (real.in_D() && imaginary.in_D())
			std::cout << " = ";
		else
			std::cout << " ≈ ";
	}
	if (!real && !imaginary)
		std::cout << "0";
	else if (!real)
		std::cout << imaginary << "i";
	else if (!imaginary)
		std::cout << real;
	else if (real < 0 && imaginary > 0)
		std::cout << imaginary << "i - " << -real;
	else
		print_complex_rounded_value_default(real, imaginary);
}

void			Complex::print_rounded(const std::string var) const
{
	if (this->values_in_Z())
		return ;
	std::cout << COLOR_DIM;
	print_complex_rounded_value(var, this->_real.getValue(), 	\
			this->_imaginary.getValue());
	std::cout << COLOR_RESET << std::endl;
}

bool			Complex::values_in_D(void) const
{
	return (this->_real.in_D() && this->_imaginary.in_D());
}

bool			Complex::values_in_Z(void) const
{
	return (this->_real.in_Z() && this->_imaginary.in_Z());
}


// Output stream operator overload

std::ostream	&operator<<(std::ostream &os, const Complex &num)
{
	return (num.print(os));
}
