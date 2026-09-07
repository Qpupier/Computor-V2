/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:44:30 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/07 17:39:57 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Complex.hpp"

// Utils

static void				from_polynomial(Complex &complex, 					\
		const Polynomial &polynomial)
{
	if (polynomial.getTerms().empty())
	{
		complex.setReal(new Rational(0));
		complex.setImaginary(new Rational(0));
	}
	else
	{
		if (polynomial.getDividers().size() != 1 					\
				|| *polynomial.getDividers()[0].coefficient != 1 	\
				|| polynomial.getDividers()[0].power 				\
				|| polynomial.getTerms().size() != 1 				\
				|| polynomial.getTerms()[0].power)
			throw ERROR_UNEXPECTED;
		complex.setReal(polynomial.getTerms()[0].coefficient->clone());
		complex.setImaginary(new Rational(0));
	}
}

static std::ostream&	print_value_rational(std::ostream& os, 				\
		bool negative, const std::string& i, Rational* copy_rational)
{
	if (negative)
	{
		IType*	tmp;

		tmp = copy_rational;
		copy_rational = dynamic_cast<Rational*>(-*copy_rational);
		delete tmp;
	}
	if (i.empty() || copy_rational->getNumerator() != 1)
		os << copy_rational->getNumerator();
	if (!i.empty())
		os << i;
	if (copy_rational->getDenominator() != 1)
		os << "/" << copy_rational->getDenominator();
	delete copy_rational;
	return (os);
}

static std::ostream&	print_value_real(std::ostream& os, bool negative, 	\
		const std::string& i, Real* copy_real)
{
	if (negative)
	{
		IType*	tmp;

		tmp = copy_real;
		copy_real = dynamic_cast<Real*>(-*copy_real);
		delete tmp;
	}
	if (i.empty() || *copy_real != 1)
		os << *copy_real;
	if (!i.empty())
		os << i;
	delete copy_real;
	return (os);
}

static std::ostream&	print_value(std::ostream &os, IType* value, 		\
		const std::string &i, bool is_first)
{
	bool		negative_between(false);
	IType*		copy;
	Rational*	copy_rational;
	Real*		copy_real;

	if (!*value)
		return (os);
	copy = value->clone();
	copy_rational = dynamic_cast<Rational*>(copy);
	copy_real = dynamic_cast<Real*>(copy);
	if (!is_first)
	{
		if (*value < 0)
			negative_between = true;
		else
			os << " + ";
	}
	bool negative = negative_between || (is_first && *value < 0 && !i.empty());
	if (negative)
		os << (negative_between ? " - " : "-");
	if (copy_rational)
		return (print_value_rational(os, negative, i, copy_rational));
	if (copy_real)
		return (print_value_real(os, negative, i, copy_real));
	throw ERROR_UNEXPECTED;
	return (os);
}

static IType*			division_real_part(const Complex& a, 				\
		const Complex& b, const Rational& denominator)
{
	IType*	part1;
	IType*	part2;
	IType*	numerator;
	IType*	real;

	part1 = *a.getReal() * *b.getReal();
	part2 = *a.getImaginary() * *b.getImaginary();
	numerator = *part1 + *part2;
	delete part1;
	delete part2;
	real = *numerator / denominator;
	delete numerator;
	return (real);
}

static IType*			division_imaginary_part(const Complex& a, 			\
		const Complex& b, const Rational& denominator)
{
	IType*	part1;
	IType*	part2;
	IType*	numerator;
	IType*	imaginary;

	part1 = *a.getImaginary() * *b.getReal();
	part2 = *a.getReal() * *b.getImaginary();
	numerator = *part1 - *part2;
	delete part1;
	delete part2;
	imaginary = *numerator / denominator;
	delete numerator;
	return (imaginary);
}

static Complex*			division(const Complex& a, const Complex& b, 		\
		const Rational& denominator)
{
	IType*		real;
	IType*		imaginary;
	Complex*	result;

	result = new Complex();
	real = division_real_part(a, b, denominator);
	result->setReal(real);
	imaginary = division_imaginary_part(a, b, denominator);
	result->setImaginary(imaginary);
	return (result);
}

static void				print_complex_rounded_value_default(				\
		const InfiniteDecimal& real, const InfiniteDecimal& imaginary)
{
	InfiniteDecimal	tmp;

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

static IType*			test_exact_value_round_part(const Real* part)
{
	std::vector<unsigned char>	decimal_part;
	InfiniteDecimal				inf_part;

	inf_part = part->getValue();
	decimal_part = inf_part.getDecimalPart().getDigits();
	for (std::size_t i(InfiniteDecimal::PRINT_PRECISION); 			\
			i < decimal_part.size() && i < InfiniteDecimal::MAX_PRECISION; i++)
		decimal_part[i] = 0;
	if (InfiniteInt(decimal_part))
		return (new Real(InfiniteDecimal(inf_part.getIntegerPart(), 	\
				decimal_part, inf_part.getIsNegative())));
	return (new Rational(inf_part.getIntegerPart()));
}

static Complex*			test_exact_value(Complex* sqrt, const Complex& value)
{
	Complex*	square;
	Complex*	test;

	test = new Complex(*sqrt);
	if (sqrt->getReal()->getType() == IType::t_type::E_TYPE_REAL)
		test->setReal(test_exact_value_round_part(		\
				dynamic_cast<Real*>(sqrt->getReal())));
	if (sqrt->getImaginary()->getType() == IType::t_type::E_TYPE_REAL)
		test->setImaginary(test_exact_value_round_part(	\
				dynamic_cast<Real*>(sqrt->getImaginary())));
	square = *test ^ 2;
	if (*square == value)
	{
		delete square;
		delete sqrt;
		return (test);
	}
	delete square;
	delete test;
	return (sqrt);
}

static IType*			get_division_numerator(const Complex& a, 			\
		const Complex& b, bool is_real)
{
	IType*	product1;
	IType*	product2;
	IType*	numerator;

	if (is_real)
	{
		product1 = *a.getReal() * *b.getReal();
		product2 = *a.getImaginary() * *b.getImaginary();
		numerator = *product1 + *product2;
	}
	else
	{
		product1 = *a.getImaginary() * *b.getReal();
		product2 = *a.getReal() * *b.getImaginary();
		numerator = *product1 - *product2;
	}
	delete product1;
	delete product2;
	return (numerator);
}

static IType*			get_division_denominator(const Complex& b)
{
	IType*	part1;
	IType*	part2;
	IType*	denominator;

	part1 = *b.getReal() * *b.getReal();
	part2 = *b.getImaginary() * *b.getImaginary();
	denominator = *part1 + *part2;
	delete part1;
	delete part2;
	return (denominator);
}


// Constructors and destructor

Complex::Complex(const IType &other): _real(nullptr), _imaginary(nullptr)
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_complex = dynamic_cast<const Complex*>(&other);
	other_rational = dynamic_cast<const Rational*>(&other);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	other_vector = dynamic_cast<const Vector*>(&other);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_complex)
		*this = *other_complex;
	else if (other_rational)
		*this = Complex(other_rational->clone(), new Rational(0));
	else if (other_polynomial)
		from_polynomial(*this, *other_polynomial);
	else if (other_real)
		*this = Complex(other_real->clone(), new Rational(0));
	else if (other_matrix)
		throw LogicError("A matrix cannot be converted to a complex number");
	else if (other_vector)
		throw LogicError("A vector cannot be converted to a complex number");
	else
		throw ERROR_UNEXPECTED;
}

Complex::~Complex(void)
{
	delete this->_real;
	delete this->_imaginary;
}


// Operator overloads

Complex&	Complex::operator=(const Complex &other)
{
	if (this != &other)
	{
		delete this->_real;
		delete this->_imaginary;
		this->_real = other._real->clone();
		this->_imaginary = other._imaginary->clone();
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
	if (!this->_real || !this->_imaginary)
		throw ERROR_UNEXPECTED;
	return (*this->_real || *this->_imaginary);
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
	return (*this->_real == *other_complex._real 	\
			&& *this->_imaginary == *other_complex._imaginary);
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
		return ((!*this->_real && *this->_imaginary < 0) 		\
				|| (!*this->_imaginary && *this->_real < 0) 	\
				|| (*this->_real < 0 && *this->_imaginary < 0));
	return (!*this->_imaginary && *this->_real < other);
}

bool		Complex::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Complex::operator<=(const IType &other) const
{
	if (!other)
		return (*this < other || (!*this->_real && !*this->_imaginary));
	return (!*this->_imaginary && *this->_real <= other);
}

bool		Complex::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Complex::operator>(const IType &other) const
{
	if (!other)
		return ((!*this->_real && *this->_imaginary > 0) 		\
				|| (!*this->_imaginary && *this->_real > 0) 	\
				|| (*this->_real > 0 && *this->_imaginary > 0));
	return (!*this->_imaginary && *this->_real > other);
}

bool		Complex::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Complex::operator>=(const IType &other) const
{
	if (!other)
		return (*this > other || (!*this->_real && !*this->_imaginary));
	return (!*this->_imaginary && *this->_real >= other);
}

bool		Complex::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

Complex*	Complex::operator+(const Complex &other) const
{
	IType*	real;
	IType*	imaginary;
	Complex*	result;

	real = *this->_real + *other._real;
	imaginary = *this->_imaginary + *other._imaginary;
	result = new Complex(real, imaginary);
	return (result);
}

Complex*	Complex::operator+(const Rational &other) const
{
	return (*this + Complex(other.clone(), new Rational()));
}

IType*		Complex::operator+(const Matrix &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational + other);
		}
		else
		{
			real = *this;
			return (real + other);
		}
	}
	throw ERROR_OPERATION_MATRIX_COMPLEX;
	return (nullptr);
}

IType*		Complex::operator+(const Polynomial &other) const
{
	return (other + *this);
}

IType*		Complex::operator+(const Vector &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational + other);
		}
		else
		{
			real = *this;
			return (real + other);
		}
	}
	throw ERROR_OPERATION_VECTOR_COMPLEX;
	return (nullptr);
}

Complex*	Complex::operator+(const Real &other) const
{
	return (new Complex(*this->_real + other, this->_imaginary->clone()));
}

IType*		Complex::operator+(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this + *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this + *other_real);
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
	IType*	real;
	IType*	imaginary;
	Complex*	result;

	real = *this->_real - *other._real;
	imaginary = *this->_imaginary - *other._imaginary;
	result = new Complex(real, imaginary);
	return (result);
}

Complex*	Complex::operator-(const Rational &other) const
{
	return (*this - Complex(other.clone(), new Rational()));
}

IType*		Complex::operator-(const Matrix &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational - other);
		}
		else
		{
			real = *this;
			return (real - other);
		}
	}
	throw ERROR_OPERATION_MATRIX_COMPLEX;
	return (nullptr);
}

IType*		Complex::operator-(const Polynomial &other) const
{
	IType*	tmp;
	IType*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

IType*		Complex::operator-(const Vector &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational - other);
		}
		else
		{
			real = *this;
			return (real - other);
		}
	}
	throw ERROR_OPERATION_VECTOR_COMPLEX;
	return (nullptr);
}

Complex*	Complex::operator-(const Real &other) const
{
	return (new Complex(*this->_real - other, this->_imaginary->clone()));
}

IType*		Complex::operator-(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this - *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this - *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

Complex*	Complex::operator*(const Complex &other) const
{
	IType*	part1;
	IType*	part2;
	IType*	real;
	IType*	imaginary;
	Complex*	result;

	part1 = *this->_real * *other._real;
	part2 = *this->_imaginary * *other._imaginary;
	real = *part1 - *part2;
	delete part1;
	delete part2;
	part1 = *this->_real * *other._imaginary;
	part2 = *this->_imaginary * *other._real;
	imaginary = *part1 + *part2;
	delete part1;
	delete part2;
	result = new Complex(real, imaginary);
	return (result);
}

Complex*	Complex::operator*(const Rational &other) const
{
	IType*	real;
	IType*	imaginary;
	Complex*	result;

	real = *this->_real * other;
	imaginary = *this->_imaginary * other;
	result = new Complex(real, imaginary);
	return (result);
}

IType*		Complex::operator*(const Matrix &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational * other);
		}
		else
		{
			real = *this;
			return (real * other);
		}
	}
	throw ERROR_OPERATION_MATRIX_COMPLEX;
	return (nullptr);
}

IType*		Complex::operator*(const Polynomial &other) const
{
	return (other * *this);
}

IType*		Complex::operator*(const Vector &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational * other);
		}
		else
		{
			real = *this;
			return (real * other);
		}
	}
	throw ERROR_OPERATION_VECTOR_COMPLEX;
	return (nullptr);
}

Complex*	Complex::operator*(const Real &other) const
{
	return (new Complex(*this->_real * other, *this->_imaginary * other));
}

IType*		Complex::operator*(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this * *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this * *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

Complex*	Complex::operator/(const Complex &other) const
{
	IType*		part1;
	IType*		part2;
	IType*		denominator;
	Complex*	result;

	part1 = *other._real * *other._real;
	part2 = *other._imaginary * *other._imaginary;
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
	return (*this / Complex(other.clone(), new Rational()));
}

IType*		Complex::operator/(const Matrix &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational / other);
		}
		else
		{
			real = *this;
			return (real / other);
		}
	}
	throw ERROR_OPERATION_MATRIX_COMPLEX;
	return (nullptr);
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

IType*		Complex::operator/(const Vector &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational / other);
		}
		else
		{
			real = *this;
			return (real / other);
		}
	}
	throw ERROR_OPERATION_VECTOR_COMPLEX;
	return (nullptr);
}

Complex*	Complex::operator/(const Real &other) const
{
	return (new Complex(*this->_real / other, *this->_imaginary / other));
}

IType*		Complex::operator/(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this / *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this / *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

Complex*	Complex::operator%(const Complex &other) const
{
	Complex*	result;
	IType*		real;
	IType*		imaginary;
	IType*		denominator;

	real = get_division_numerator(*this, other, true);
	imaginary = get_division_numerator(*this, other, false);
	denominator = get_division_denominator(other);
	if (!*denominator)
	{
		delete real;
		delete imaginary;
		delete denominator;
		throw ERROR_DIVISION_BY_ZERO;
	}
	result = new Complex(*real / *denominator, *imaginary / *denominator);
	delete real;
	delete imaginary;
	delete denominator;
	return (result);
}

Complex*	Complex::operator%(const Rational &other) const
{
	return (*this % Complex(other.clone(), new Rational()));
}

IType*		Complex::operator%(const Matrix &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational % other);
		}
		else
		{
			real = *this;
			return (real % other);
		}
	}
	throw ERROR_OPERATION_MATRIX_COMPLEX;
	return (nullptr);
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

IType*		Complex::operator%(const Vector &other) const
{
	Rational	rational;
	Real		real;

	if (!*this->_imaginary)
	{
		if (this->in_Q())
		{
			rational = *this;
			return (rational % other);
		}
		else
		{
			real = *this;
			return (real % other);
		}
	}
	throw ERROR_OPERATION_VECTOR_COMPLEX;
	return (nullptr);
}

Complex*	Complex::operator%(const Real &other) const
{
	return (*this % Complex(other.clone(), new Rational()));
}

IType*		Complex::operator%(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this % *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this % *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Complex*	Complex::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

Complex*	Complex::operator^(const Rational &other) const
{
	Complex*	result;
	Complex*	tmp;

	if (!other.in_Z() || other < 0)
		throw UNSUPPORTED_EXPONENT;
	result = new Complex(new Rational(1), new Rational(0));
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

IType::t_type	Complex::getType(void) const
{
	return (IType::t_type::E_TYPE_COMPLEX);
}

IType*			Complex::getImaginary(void) const
{
	return (this->_imaginary);
}

IType*			Complex::getReal(void) const
{
	return (this->_real);
}


// Setters

void	Complex::setReal(IType* real)
{
	delete this->_real;
	this->_real = real;
}

void	Complex::setImaginary(IType* imaginary)
{
	delete this->_imaginary;
	this->_imaginary = imaginary;
}


// Methods

std::string		Complex::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			Complex::in_C(void) const
{
	return (true);
}

bool			Complex::in_D(void) const
{
	return (this->_real->in_D() && this->_imaginary->in_D());
}

bool			Complex::in_M(void) const
{
	return (false);
}

bool			Complex::in_Q(void) const
{
	return (this->_real->in_Q() && this->_imaginary->in_Q());
}

bool			Complex::in_Z(void) const
{
	return (this->_real->in_Z() && this->_imaginary->in_Z());
}

std::ostream&	Complex::print(std::ostream &os) const
{
	if (!this->_real || !this->_imaginary)
		throw ERROR_UNEXPECTED;
	if (!*this->_real && !*this->_imaginary)
		return (os << "0");
	if ((!*this->_real || *this->_real < 0) && *this->_imaginary > 0)
	{
		print_value(os, this->_imaginary, "i", true);
		print_value(os, this->_real, "", false);
	}
	else
	{
		print_value(os, this->_real, "", true);
		print_value(os, this->_imaginary, "i", !*this->_real);
	}
	return (os);
}

IType*			Complex::abs(void) const
{
	if (!*this->getImaginary())
		return (this->getReal()->abs());
	throw LogicError("Absolute function is not defined for complex numbers, "
			"use the norm function instead");
}

IType*			Complex::clone(void) const
{
	return (new Complex(*this));
}

IType*			Complex::cos(void) const
{
	IType*	product;
	IType*	e1;
	IType*	e2;
	IType*	numerator;
	IType*	result;

	product = Complex(new Rational(0), new Rational(1)) * *this;
	e1 = product->exp();
	product = Complex(new Rational(0), new Rational(-1)) * *this;
	e2 = product->exp();
	delete product;
	numerator = *e1 + *e2;
	delete e1;
	delete e2;
	result = *numerator / 2;
	delete numerator;
	return (result);
}

IType*			Complex::exp(void) const
{
	IType*		cosine;
	IType*		e;
	IType*		sine;
	Complex*	result;

	e = this->getReal()->exp();
	cosine = this->getImaginary()->cos();
	sine = this->getImaginary()->sin();
	result = new Complex(*e * *cosine, *e * *sine);
	delete e;
	delete cosine;
	delete sine;
	return (result);
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
	IType*	real_squared;
	IType*	imaginary_squared;
	IType*	sum;
	IType*	result;

	real_squared = *this->_real * *this->_real;
	imaginary_squared = *this->_imaginary * *this->_imaginary;
	sum = *real_squared + *imaginary_squared;
	delete real_squared;
	delete imaginary_squared;
	result = sum->sqrt();
	delete sum;
	return (result);
}

IType*			Complex::sin(void) const
{
	IType*	product;
	IType*	e1;
	IType*	e2;
	IType*	numerator;
	IType*	result;

	product = Complex(new Rational(0), new Rational(1)) * *this;
	e1 = product->exp();
	product = Complex(new Rational(0), new Rational(-1)) * *this;
	e2 = product->exp();
	delete product;
	numerator = *e1 - *e2;
	delete e1;
	delete e2;
	result = *numerator / Complex(new Rational(0), new Rational(2));
	delete numerator;
	return (result);
}

IType*			Complex::sqrt(void) const
{
	IType*	norm;
	IType*	real;
	IType*	imaginary;
	IType*	tmp;
	IType*	tmp2;

	norm = this->norm();
	tmp = *norm + *this->_real;
	tmp2 = *tmp / 2;
	delete tmp;
	real = tmp2->sqrt();
	delete tmp2;
	tmp = *norm - *this->_real;
	tmp2 = *tmp / 2;
	delete tmp;
	imaginary = tmp2->sqrt();
	delete tmp2;
	if (*this->_imaginary < 0)
	{
		tmp = imaginary;
		imaginary = -*imaginary;
		delete tmp;
	}
	delete norm;
	return (test_exact_value(new Complex(real, imaginary), *this));
}

IType*			Complex::tan(void) const
{
	IType*	cosine;
	IType*	result;
	IType*	sine;

	cosine = this->cos();
	if (!*cosine)
	{
		delete cosine;
		throw ERROR_TANGENT_UNDEFINED;
	}
	sine = this->sin();
	result = *sine / *cosine;
	delete cosine;
	delete sine;
	return (result);
}

Rational*		Complex::fact(void) const
{
	if (!this->getImaginary() && this->getReal()->in_Q())
		return (this->getReal()->fact());
	throw ERROR_FACTORIAL_FUNCTION;
	return (nullptr);
}

Rational*		Complex::gcd(const Rational &other) const
{
	return (other.gcd(*this));
}

Rational*		Complex::gcd(const Complex &other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(*other.getReal());
	second = first->gcd(*other.getImaginary());
	delete first;
	return (second);
}

Rational*		Complex::gcd(const Matrix &other) const
{
	Rational*	first;
	Rational*	second;

	first = other.gcd(*this->getReal());
	second = first->gcd(*this->getImaginary());
	delete first;
	return (second);
}

Rational*		Complex::gcd(const Vector &other) const
{
	Rational*	first;
	Rational*	second;

	first = other.gcd(*this->getReal());
	second = first->gcd(*this->getImaginary());
	delete first;
	return (second);
}

Rational*		Complex::gcd(const IType &other) const
{
	const Complex*		other_complex;
	const Rational*		other_rational;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->gcd(*other_complex));
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->gcd(*other_rational));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->gcd(*other_matrix));
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (other_polynomial->gcd(*this));
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (this->gcd(*other_vector));
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (this->gcd(*other_real));
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Real*			Complex::deg(void) const
{
	if (this->in_Q())
		try
		{
			return (Rational(*this).deg());
		}
		catch (...) {}
	else
		try
		{
			return (Real(*this).deg());
		}
		catch (...) {}
	throw ERROR_DEGREE_FUNCTION;
	return (nullptr);
}

Real*			Complex::rad(void) const
{
	if (this->in_Q())
		try
		{
			return (Rational(*this).rad());
		}
		catch (...) {}
	else
		try
		{
			return (Real(*this).rad());
		}
		catch (...) {}
	throw ERROR_RADIAN_FUNCTION;
	return (nullptr);
}

void			Complex::print_rounded(const std::string var) const
{
	if (!this->in_Q() || this->in_Z())
		return ;
	std::cout << COLOR_DIM;
	print_complex_rounded_value(var, Rational(*this->_real).getValue(), 	\
			Rational(*this->_imaginary).getValue());
	std::cout << COLOR_RESET << std::endl;
}


// Output stream operator overload

std::ostream	&operator<<(std::ostream &os, const Complex &num)
{
	return (num.print(os));
}


// Functions

void	print_complex_rounded_value(const std::string var, 	\
		const InfiniteDecimal& real, const InfiniteDecimal& imaginary)
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
	{
		if (imaginary == -1)
			std::cout << "-";
		else if (imaginary != 1)
			std::cout << imaginary;
		std::cout << "i";
	}
	else if (!imaginary)
		std::cout << real;
	else if (real < 0 && imaginary > 0)
		std::cout << imaginary << "i - " << -real;
	else
		print_complex_rounded_value_default(real, imaginary);
}
