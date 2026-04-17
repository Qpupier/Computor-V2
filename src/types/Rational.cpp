/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 19:46:50 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/16 14:47:18 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Rational.hpp"

// Utils
static int	compute_gcd(int a, int b)
{
	if (!b)
		return (std::abs(a));
	return (compute_gcd(b, a % b));
}

static int	compute_lcm(int a, int b)
{
	return (a / compute_gcd(a, b) * b);
}

// Constructors
Rational::Rational(int numerator, int denominator): _numerator(numerator), _denominator(denominator)
{
	if (!denominator)
		throw ERROR_DIVISION_BY_ZERO;
	this->reduce();
}

Rational::Rational(std::string str)
{
	std::size_t	slash_pos;

	slash_pos = str.find('.');
	if (slash_pos == std::string::npos)
	{
		_numerator = std::stoi(str);
		_denominator = 1;
	}
	else
	{
		_numerator = std::stoi(str.erase(slash_pos, 1));
		_denominator = static_cast<int>(std::pow(10, str.size() - slash_pos));
	}
	this->reduce();
}

Rational::Rational(const IType &other)
{
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	other_complex = dynamic_cast<const Complex*>(&other);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_rational)
		*this = *other_rational;
	else if (other_complex)
	{
		if (other_complex->getImaginary())
			throw ERROR_UNEXPECTED;
		*this = other_complex->getReal();
	}
	else if (other_matrix)
		throw ERROR_UNEXPECTED;
	else if (other_polynomial)
	{
		if (other_polynomial->getDividers().size() != 1 || *other_polynomial->getDividers()[0].coefficient != Rational(1) || other_polynomial->getDividers()[0].power || other_polynomial->getTerms().size() != 1 || other_polynomial->getTerms()[0].power)
			throw ERROR_UNEXPECTED;//BUG: Size = 0 si polynomial = 0
		*this = Rational(*other_polynomial->getTerms()[0].coefficient);// FIXME: Leaks?
	}
	else
		throw ERROR_UNEXPECTED;
}


// Operator overloads
Rational&	Rational::operator=(const Rational &other)
{
	if (this != &other)
	{
		this->_numerator = other._numerator;
		this->_denominator = other._denominator;
	}
	return (*this);
}

Rational&	Rational::operator=(const Rational *other)
{
	if (this != other)
	{
		this->_numerator = other->_numerator;
		this->_denominator = other->_denominator;
		delete other;
	}
	return (*this);
}

Rational	Rational::operator=(const Complex &other)
{
	if (other.getImaginary())
		throw LogicError("Cannot convert a complex number with a non-zero imaginary part to a rational number");
	return (Rational(other.getReal()));
}

Rational::operator bool() const
{
	return (this->_numerator);
}

bool		Rational::operator==(const Rational &other) const
{
	return (this->_numerator * other._denominator == other._numerator * this->_denominator);
}

bool		Rational::operator==(const Complex &other) const
{
	return (!other.getImaginary() && *this == other.getReal());
}

bool		Rational::operator==(const Matrix &other) const
{
	(void)other;
	return (false);
}

bool		Rational::operator==(const Polynomial &other) const
{
	std::vector<Polynomial::t_term>	terms(other.getTerms());
	std::vector<Polynomial::t_term>	dividers(other.getDividers());

	return (terms.size() == 1 && dividers.size() == 1 	\
			&& *terms[0].coefficient == *this 			\
			&& !terms[0].power 							\
			&& *dividers[0].coefficient == Rational(1) 	\
			&& !dividers[0].power);
}

bool		Rational::operator==(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this == *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this == *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this == *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this == *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (false);
}

bool		Rational::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Rational::operator<(const Rational &other) const
{
	return (this->_numerator * other._denominator < other._numerator * this->_denominator);
}

bool		Rational::operator<(const Complex &other) const
{
	return (!other.getImaginary() && *this < other.getReal());
}

bool		Rational::operator<(const Matrix &other) const
{
	(void)other;
	return (false);
}

bool		Rational::operator<(const Polynomial &other) const
{
	std::vector<Polynomial::t_term>	terms(other.getTerms());
	std::vector<Polynomial::t_term>	dividers(other.getDividers());

	return (terms.size() == 1 && dividers.size() == 1 	\
			&& *terms[0].coefficient < *this 			\
			&& !terms[0].power 							\
			&& *dividers[0].coefficient == Rational(1) 	\
			&& !dividers[0].power);
}

bool		Rational::operator<(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this < *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this < *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this < *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this < *other_polynomial);
	return (false);
}

bool		Rational::operator<=(const Rational &other) const
{
	return (this->_numerator * other._denominator <= other._numerator * this->_denominator);
}

bool		Rational::operator<=(const Complex &other) const
{
	return (!other.getImaginary() && *this <= other.getReal());
}

bool		Rational::operator<=(const Matrix &other) const
{
	(void)other;
	return (false);
}

bool		Rational::operator<=(const Polynomial &other) const
{
	std::vector<Polynomial::t_term>	terms(other.getTerms());
	std::vector<Polynomial::t_term>	dividers(other.getDividers());

	return (terms.size() == 1 && dividers.size() == 1 	\
			&& *terms[0].coefficient <= *this 			\
			&& !terms[0].power 							\
			&& *dividers[0].coefficient == Rational(1) 	\
			&& !dividers[0].power);
}

bool		Rational::operator<=(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this <= *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this <= *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this <= *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this <= *other_polynomial);
	return (false);
}

bool		Rational::operator>(const Rational &other) const
{
	return (this->_numerator * other._denominator > other._numerator * this->_denominator);
}

bool		Rational::operator>(const Complex &other) const
{
	return (!other.getImaginary() && *this > other.getReal());
}

bool		Rational::operator>(const Matrix &other) const
{
	(void)other;
	return (false);
}

bool		Rational::operator>(const Polynomial &other) const
{
	std::vector<Polynomial::t_term>	terms(other.getTerms());
	std::vector<Polynomial::t_term>	dividers(other.getDividers());

	return (terms.size() == 1 && dividers.size() == 1 	\
			&& *terms[0].coefficient > *this 			\
			&& !terms[0].power 							\
			&& *dividers[0].coefficient == Rational(1) 	\
			&& !dividers[0].power);
}

bool		Rational::operator>(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this > *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this > *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this > *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this > *other_polynomial);
	return (false);
}

bool		Rational::operator>=(const Rational &other) const
{
	return (this->_numerator * other._denominator >= other._numerator * this->_denominator);
}

bool		Rational::operator>=(const Complex &other) const
{
	return (!other.getImaginary() && *this >= other.getReal());
}

bool		Rational::operator>=(const Matrix &other) const
{
	(void)other;
	return (false);
}

bool		Rational::operator>=(const Polynomial &other) const
{
	std::vector<Polynomial::t_term>	terms(other.getTerms());
	std::vector<Polynomial::t_term>	dividers(other.getDividers());

	return (terms.size() == 1 && dividers.size() == 1 	\
			&& *terms[0].coefficient >= *this 			\
			&& !terms[0].power 							\
			&& *dividers[0].coefficient == Rational(1) 	\
			&& !dividers[0].power);
}

bool		Rational::operator>=(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this >= *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this >= *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this >= *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this >= *other_polynomial);
	return (false);
}

Rational*	Rational::operator+(const Rational &other) const
{
	return (new Rational(this->_numerator * other._denominator + other._numerator * this->_denominator, this->_denominator * other._denominator));
}

Complex*	Rational::operator+(const Complex &other) const
{
	return (Complex(*this, Rational()) + other);
}

Matrix*		Rational::operator+(const Matrix &other) const
{
	Matrix				*result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++)
			result->setValue(i, j, *this + other[i][j]);
	return (result);
}

Polynomial*	Rational::operator+(const Polynomial &other) const
{
	return (other + *this);
}

IType*		Rational::operator+(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator-(const Rational &other) const
{
	return (new Rational(this->_numerator * other._denominator - other._numerator * this->_denominator, this->_denominator * other._denominator));
}

Complex*	Rational::operator-(const Complex &other) const
{
	return (Complex(*this, Rational()) - other);
}

Matrix*		Rational::operator-(const Matrix &other) const
{
	Matrix				*result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++)
			result->setValue(i, j, *this - other[i][j]);
	return (result);
}

Polynomial*	Rational::operator-(const Polynomial &other) const
{
	return (other - *this);
}

IType*		Rational::operator-(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator*(const Rational &other) const
{
	return (new Rational(this->_numerator * other._numerator, this->_denominator * other._denominator));
}

Complex*	Rational::operator*(const Complex &other) const
{
	return (Complex(*this, Rational()) * other);
}

Matrix*		Rational::operator*(const Matrix &other) const
{
	Matrix				*result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++)
			result->setValue(i, j, *this * other[i][j]);
	return (result);
}

Polynomial*	Rational::operator*(const Polynomial &other) const
{
	return (other * *this);
}

IType*		Rational::operator*(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator/(const Rational &other) const
{
	return (*this * Rational(other.getDenominator(), other.getNumerator()));
}

Complex*	Rational::operator/(const Complex &other) const
{
	return (Complex(*this, Rational()) / other);
}

Matrix*		Rational::operator/(const Matrix &other) const
{
	Matrix				*result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++)
			try
			{
				result->setValue(i, j, *this / other[i][j]);
			}
			catch (const std::exception &e)
			{
				delete result;
				throw;
			}
	return (result);
}

Polynomial*	Rational::operator/(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), (Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

IType*		Rational::operator/(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator%(const Rational &other) const
{
	Rational	*result;
	Rational	*tmp;

	result = *this / other;
	tmp = result;
	result = other * Rational(result->integer_part());
	delete tmp;
	tmp = result;
	result = *this - *result;
	delete tmp;
	return (result);
}

Rational*	Rational::operator%(const Complex &other) const
{
	return (Complex(*this, Rational()) % other);
}

Matrix*		Rational::operator%(const Matrix &other) const
{
	Matrix				*result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++)
			try
			{
				result->setValue(i, j, *this % other[i][j]);
			}
			catch (const std::exception &e)
			{
				delete result;
				throw;
			}
	return (result);
}

Polynomial*	Rational::operator%(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), (Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

IType*		Rational::operator%(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;
	const Polynomial	*other_polynomial;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Rational::operator^(const IType &other) const
{
	Rational	power;
	Rational*	result;
	Rational*	tmp;

	try
	{
		power = Rational(other);
	}
	catch(const LogicError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	if (!power.is_integer() || power < Rational(0))
		throw UNSUPPORTED_EXPONENT;
	result = new Rational(1);
	for (int i = 0; i < power.getNumerator(); i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}


// Getters
int		Rational::getNumerator(void) const
{
	return (this->_numerator);
}

int		Rational::getDenominator(void) const
{
	return (this->_denominator);
}

double	Rational::getValue(void) const
{
	return (static_cast<double>(this->_numerator) / this->_denominator);
}


// Methods
IType*			Rational::matrix_operator(const IType &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Rational::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Rational::clone(void) const
{
	return (new Rational(*this));
}

Rational*		Rational::gcd(const Rational &other) const
{
	int	gcd_numerator;
	int	gcd_denominator;

	if (!this->_numerator)
		return (new Rational(other));
	if (!other._numerator)
		return (new Rational(*this));
	gcd_numerator = compute_gcd(this->_numerator, other._numerator);
	gcd_denominator = compute_lcm(this->_denominator, other._denominator);
	return (new Rational(gcd_numerator, gcd_denominator));
}

Rational*		Rational::gcd(const Complex &other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(other.getReal());
	second = first->gcd(other.getImaginary());
	delete first;
	return (second);
}

Rational*		Rational::gcd(const Matrix &other) const
{
	Rational*	gcd;
	Rational*	tmp;

	gcd = new Rational(*this);
	for (unsigned int i = 0; i < other.getHeight(); i++)
		for (unsigned int j = 0; j < other.getWidth(); j++)
		{
			tmp = gcd;
			gcd = gcd->gcd(other[i][j]);
			delete tmp;
		}
	return (gcd);
}

Rational*		Rational::gcd(const IType &other) const
{
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

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

std::ostream&	Rational::print(std::ostream &os) const
{
	Rational	copy(*this);
	int			numerator;

	numerator = copy.getNumerator();
	if (copy.is_integer())
		return (os << numerator);
	return (os << numerator << "/" << copy.getDenominator());
}

std::ostream&	Rational::print_polynomial(std::ostream &os, 	\
		const std::string &var) const
{
	Rational	copy(*this);
	int			numerator;

	numerator = copy.getNumerator();
	if (copy.is_integer())
		return (os << numerator << var);
	return (os << numerator << var << "/" << copy.getDenominator());
}

void			Rational::reduce(void)
{
	int	gcd;

	gcd = compute_gcd(this->_numerator, this->_denominator);
	if (!gcd)
	{
		this->_numerator = 0;
		this->_denominator = 1;
		return ;
	}
	this->_numerator /= gcd;
	this->_denominator /= gcd;
	if (this->_denominator < 0)
	{
		this->_numerator = -this->_numerator;
		this->_denominator = -this->_denominator;
	}
}

bool			Rational::is_integer(void) const
{
	Rational	copy(*this);

	copy.reduce();
	return (copy.getDenominator() == 1);
}

int				Rational::integer_part(void) const
{
	return (this->_numerator / this->_denominator);
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Rational &num)
{
	return (num.print(os));
}
