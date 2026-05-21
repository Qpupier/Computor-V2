/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 19:46:50 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/21 18:06:47 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Rational.hpp"

// Utils
static InfiniteInt	compute_lcm(InfiniteInt a, InfiniteInt b)
{
	return (a / compute_gcd(a, b) * b);
}

// Constructors
Rational::Rational(InfiniteInt numerator, InfiniteInt denominator): 	\
		_numerator(numerator), _denominator(denominator)
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
		_numerator = InfiniteInt(str);
		_denominator = InfiniteInt(1);
	}
	else
	{
		_numerator = InfiniteInt(str.erase(slash_pos, 1));
		_denominator = InfiniteInt(std::to_string(	\
				static_cast<int>(std::pow(10, str.size() - slash_pos))));
	}
	this->reduce();
}

Rational::Rational(const IType &other)
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
		if (other_polynomial->getTerms().empty())
			*this = Rational(0);
		else
		{
			if (other_polynomial->getDividers().size() != 1 			\
					|| *other_polynomial->getDividers()[0].coefficient 	\
						!= Rational(1) 									\
					|| other_polynomial->getDividers()[0].power 		\
					|| other_polynomial->getTerms().size() != 1 		\
					|| other_polynomial->getTerms()[0].power)
				throw ERROR_UNEXPECTED;
			*this = Rational(*other_polynomial->getTerms()[0].coefficient);
		}
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
	return (static_cast<bool>(this->_numerator));
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
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	return (this->_numerator * other._denominator 	\
			<= other._numerator * this->_denominator);
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
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	return (this->_numerator * other._denominator 	\
			> other._numerator * this->_denominator);
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
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	return (this->_numerator * other._denominator 	\
			>= other._numerator * this->_denominator);
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
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	return (new Rational(this->_numerator * other._denominator 	\
				+ other._numerator * this->_denominator, 		\
			this->_denominator * other._denominator));
}

Complex*	Rational::operator+(const Complex &other) const
{
	return (Complex(*this, Rational()) + other);
}

Matrix*		Rational::operator+(const Matrix &other) const
{
	Matrix*					result;
	unsigned long long int	width;
	unsigned long long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long long int i = 0; i < height; i++)
		for (unsigned long long int j = 0; j < width; j++)
			result->setValue(i, j, *this + other[i][j]);
	return (result);
}

Polynomial*	Rational::operator+(const Polynomial &other) const
{
	return (other + *this);
}

IType*		Rational::operator+(const IType &other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	return (new Rational(this->_numerator * other._denominator 	\
				- other._numerator * this->_denominator, 		\
			this->_denominator * other._denominator));
}

Complex*	Rational::operator-(const Complex &other) const
{
	return (Complex(*this, Rational()) - other);
}

Matrix*		Rational::operator-(const Matrix &other) const
{
	Matrix*					result;
	unsigned long long int	width;
	unsigned long long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long long int i = 0; i < height; i++)
		for (unsigned long long int j = 0; j < width; j++)
			result->setValue(i, j, *this - other[i][j]);
	return (result);
}

Polynomial*	Rational::operator-(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = other - *this;
	result = *tmp * Rational(-InfiniteInt(1));
	delete tmp;
	return (result);
}

IType*		Rational::operator-(const IType &other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	return (new Rational(this->_numerator * other._numerator, 	\
			this->_denominator * other._denominator));
}

Complex*	Rational::operator*(const Complex &other) const
{
	return (Complex(*this, Rational()) * other);
}

Matrix*		Rational::operator*(const Matrix &other) const
{
	Matrix*					result;
	unsigned long long int	width;
	unsigned long long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long long int i = 0; i < height; i++)
		for (unsigned long long int j = 0; j < width; j++)
			result->setValue(i, j, *this * other[i][j]);
	return (result);
}

Polynomial*	Rational::operator*(const Polynomial &other) const
{
	return (other * *this);
}

IType*		Rational::operator*(const IType &other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	Matrix*					result;
	unsigned long long int	width;
	unsigned long long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long long int i = 0; i < height; i++)
		for (unsigned long long int j = 0; j < width; j++)
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
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	Rational*	result;
	Rational*	tmp;

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
	Matrix*					result;
	unsigned long long int	width;
	unsigned long long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long long int i = 0; i < height; i++)
		for (unsigned long long int j = 0; j < width; j++)
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
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

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
	Rational*	result;
	Rational*	tmp;
	Rational	power;

	try
	{
		power = Rational(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	if (!power.is_integer() || power < Rational(0))
		throw UNSUPPORTED_EXPONENT;
	result = new Rational(1);
	for (InfiniteInt i(0); i < power.getNumerator(); i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}


// Getters
InfiniteInt		Rational::getNumerator(void) const
{
	return (this->_numerator);
}

InfiniteInt		Rational::getDenominator(void) const
{
	return (this->_denominator);
}

InfiniteDouble	Rational::getValue(void) const
{
	return (InfiniteDouble(this->_numerator) 	\
			/ InfiniteDouble(this->_denominator));
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
	InfiniteInt	gcd_numerator;
	InfiniteInt	gcd_denominator;

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
	for (unsigned long long int i = 0; i < other.getHeight(); i++)
		for (unsigned long long int j = 0; j < other.getWidth(); j++)
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
	InfiniteInt	numerator;

	numerator = copy.getNumerator();
	if (copy.is_integer())
		return (os << numerator);
	return (os << numerator << "/" << copy.getDenominator());
}

void			Rational::print_variable(const std::string var) const
{
	if (this->in_Z())
		return ;
	std::cout << COLOR_DIM;
	if (!var.empty())
		std::cout << var << (this->finite_decimals() ? " = " : " ≈ ");
	// std::cout << this->getValue() << COLOR_RESET << std::endl;// TODO: Useless?
}

void			Rational::reduce(void)
{
	InfiniteInt	gcd;

	gcd = compute_gcd(this->_numerator, this->_denominator);
	if (!gcd)
	{
		this->_numerator = InfiniteInt();
		this->_denominator = InfiniteInt(1);
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

bool			Rational::finite_decimals(void) const
{
	InfiniteInt	denominator;
	InfiniteInt	m;
	InfiniteInt	n;

	denominator = this->_denominator;
	while (denominator % 2 == 0)
	{
		denominator /= 2;
		m++;
	}
	while (denominator % 5 == 0)
	{
		denominator /= 5;
		n++;
	}
	return (denominator == 1);// BUG: A corriger
}

bool			Rational::in_Z(void) const
{
	return (this->is_integer());
}

bool			Rational::is_integer(void) const
{
	Rational	copy(*this);

	copy.reduce();
	return (copy.getDenominator() == InfiniteInt(1));
}

InfiniteInt		Rational::integer_part(void) const
{
	return (this->_numerator / this->_denominator);
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Rational &num)
{
	return (num.print(os));
}


// Functions
InfiniteInt	compute_gcd(InfiniteInt a, InfiniteInt b)
{
	if (!b)
		return (a >= InfiniteInt(0) ? a : -a);
	return (compute_gcd(b, a % b));
}
