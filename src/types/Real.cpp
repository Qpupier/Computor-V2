/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Real.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 11:05:28 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/01 18:53:23 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Real.hpp"

// Utils

static Real		from_polynomial(const Polynomial &polynomial)
{
	if (polynomial.getTerms().empty())
		return (Real(0));
	if (polynomial.getDividers().size() != 1 					\
			|| *polynomial.getDividers()[0].coefficient != 1 	\
			|| polynomial.getDividers()[0].power 				\
			|| polynomial.getTerms().size() != 1 				\
			|| polynomial.getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	return (Real(*polynomial.getTerms()[0].coefficient));
}

static InfiniteInt	compute_lcm(InfiniteInt a, InfiniteInt b)
{
	return (a / compute_gcd(a, b) * b);
}


// Constructors

Real::Real(InfiniteInt numerator, InfiniteInt denominator): 	\
		_numerator(numerator), _denominator(denominator)
{
	if (!denominator)
		throw ERROR_DIVISION_BY_ZERO;
	this->reduce();
}

Real::Real(std::string str)
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

Real::Real(const IType &other)
{
	const Real*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

	other_rational = dynamic_cast<const Real*>(&other);
	other_complex = dynamic_cast<const Complex*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_rational)
		*this = *other_rational;
	else if (other_complex)
	{
		if (*other_complex->getImaginary())
			throw ERROR_UNEXPECTED;
		*this = *other_complex->getReal();
	}
	else if (other_polynomial)
		*this = from_polynomial(*other_polynomial);
	else
		throw ERROR_UNEXPECTED;
}


// Operator overloads

Real::operator bool() const
{
	return (static_cast<bool>(this->_numerator));
}

Real&	Real::operator=(const Real * other)
{
	if (this != other)
	{
		this->_numerator = other->_numerator;
		this->_denominator = other->_denominator;
		delete other;
	}
	return (*this);
}

Real&	Real::operator=(const Real &other)
{
	if (this != &other)
	{
		this->_numerator = other._numerator;
		this->_denominator = other._denominator;
	}
	return (*this);
}

Real	Real::operator=(const Complex &other)
{
	if (other.getImaginary())
		throw LogicError("Cannot convert a complex number with a 	\
				non-zero imaginary part to a rational number");
	return (Real(other.getReal()));
}

Real	Real::operator=(const IType &other)
{
	*this = Real(other);
	return (*this);
}

bool		Real::operator==(const IType &other) const
{
	Real	other_rational;

	try
	{
		other_rational = Real(other);
	}
	catch(const UnexpectedError &e)
	{
		return (false);
	}
	return (this->_numerator == other_rational._numerator 	\
			&& this->_denominator == other_rational._denominator);
}

bool		Real::operator==(const long long int value) const
{
	return (*this == Real(value));
}

bool		Real::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Real::operator!=(const long long int value) const
{
	return (*this != Real(value));
}

bool		Real::operator<(const IType &other) const
{
	Real	other_rational;

	try
	{
		other_rational = Real(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (this->_numerator * other_rational._denominator 	\
			< other_rational._numerator * this->_denominator);
}

bool		Real::operator<(const long long int value) const
{
	return (*this < Real(value));
}

bool		Real::operator<=(const IType &other) const
{
	return (*this < other || *this == other);
}

bool		Real::operator<=(const long long int value) const
{
	return (*this <= Real(value));
}

bool		Real::operator>(const IType &other) const
{
	Real	other_rational;

	try
	{
		other_rational = Real(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (!(*this <= other_rational));
}

bool		Real::operator>(const long long int value) const
{
	return (*this > Real(value));
}

bool		Real::operator>=(const IType &other) const
{
	Real	other_rational;

	try
	{
		other_rational = Real(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (!(*this < other_rational));
}

bool		Real::operator>=(const long long int value) const
{
	return (*this >= Real(value));
}

Real*	Real::operator+(const Real &other) const
{
	return (new Real(this->_numerator * other._denominator 	\
				+ other._numerator * this->_denominator, 		\
			this->_denominator * other._denominator));
}

Complex*	Real::operator+(const Complex &other) const
{
	return (Complex(*this, Real()) + other);
}

Matrix*		Real::operator+(const Matrix &other) const
{
	Matrix*					result;
	Real*				tmp;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
		{
			tmp = *this + other[j][i];
			result->setValue(i, j, *tmp);
			delete tmp;
		}
	return (result);
}

Vector*		Real::operator+(const Vector &other) const
{
	unsigned long int	size(other.size());
	Vector*					result;
	Real*				tmp;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
	{
		tmp = *this + other[i];
		result[i] = *tmp;
		delete tmp;
	}
	return (result);
}

Polynomial*	Real::operator+(const Polynomial &other) const
{
	return (other + *this);
}

IType*		Real::operator+(const IType &other) const
{
	const Real*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;// TODO: Ajouter partout les Vectors

	other_rational = dynamic_cast<const Real*>(&other);
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

Real*	Real::operator+(const long long int value) const
{
	return (*this + Real(value));
}

Real*	Real::operator-(void) const
{
	return (*this * (-1));
}

Real*	Real::operator-(const Real &other) const
{
	return (new Real(this->_numerator * other._denominator 	\
				- other._numerator * this->_denominator, 		\
			this->_denominator * other._denominator));
}

Complex*	Real::operator-(const Complex &other) const
{
	return (Complex(*this, Real()) - other);
}

Matrix*		Real::operator-(const Matrix &other) const
{
	Matrix*					result;
	Real*				tmp;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
		{
			tmp = *this - other[j][i];
			result->setValue(i, j, *tmp);
			delete tmp;
		}
	return (result);
}

Vector*		Real::operator-(const Vector &other) const
{
	unsigned long int	size(other.size());
	Vector*					result;
	Real*				tmp;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
	{
		tmp = *this - other[i];
		result[i] = *tmp;
		delete tmp;
	}
	return (result);
}

Polynomial*	Real::operator-(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

IType*		Real::operator-(const IType &other) const
{
	const Real*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_rational = dynamic_cast<const Real*>(&other);
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

Real*	Real::operator-(const long long int value) const
{
	return (*this - Real(value));
}

Real*	Real::operator*(const Real &other) const
{
	return (new Real(this->_numerator * other._numerator, 	\
			this->_denominator * other._denominator));
}

Complex*	Real::operator*(const Complex &other) const
{
	return (Complex(*this, Real()) * other);
}

Matrix*		Real::operator*(const Matrix &other) const
{
	Matrix*					result;
	Real*				tmp;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
		{
			tmp = *this * other[j][i];
			result->setValue(i, j, *tmp);
			delete tmp;
		}
	return (result);
}

Vector*	Real::operator*(const Vector &other) const
{
	unsigned long int	size(other.size());
	Vector*					result;
	Real*				tmp;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
	{
		tmp = *this * other[i];
		result[i] = *tmp;
		delete tmp;
	}
	return (result);
}

Polynomial*	Real::operator*(const Polynomial &other) const
{
	return (other * *this);
}

IType*		Real::operator*(const IType &other) const
{
	const Real*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_rational = dynamic_cast<const Real*>(&other);
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

Real*	Real::operator*(const long long int value) const
{
	return (*this * Real(value));
}

Real*	Real::operator/(const Real &other) const
{
	return (*this * Real(other.getDenominator(), other.getNumerator()));
}

Complex*	Real::operator/(const Complex &other) const
{
	return (Complex(*this, Real()) / other);
}

Matrix*		Real::operator/(const Matrix &other) const
{
	Matrix*					result;
	Real*				tmp;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			try
			{
				tmp = *this / other[j][i];
				result->setValue(i, j, *tmp);
				delete tmp;
			}
			catch (const std::exception &e)
			{
				delete result;
				throw;
			}
	return (result);
}

Vector*		Real::operator/(const Vector &other) const
{
	unsigned long int	size(other.size());
	Vector*					result;
	Real*				tmp;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		try
		{
			tmp = *this / other[i];
			result[i] = *tmp;
			delete tmp;
		}
		catch (const std::exception &e)
		{
			delete result;
			throw;
		}
	return (result);
}

Polynomial*	Real::operator/(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

IType*		Real::operator/(const IType &other) const
{
	const Real*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_rational = dynamic_cast<const Real*>(&other);
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

Real*	Real::operator/(const long long int value) const
{
	return (*this / Real(value));
}

Real*	Real::operator%(const Real &other) const
{
	Real*	result;
	Real*	tmp;

	result = *this / other;
	tmp = result;
	result = other * Real(result->integer_part());
	delete tmp;
	tmp = result;
	result = *this - *result;
	delete tmp;
	return (result);
}

Real*	Real::operator%(const Complex &other) const
{
	return (Complex(*this, Real()) % other);
}

Matrix*		Real::operator%(const Matrix &other) const
{
	Matrix*					result;
	Real*				tmp;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			try
			{
				tmp = *this % other[j][i];
				result->setValue(i, j, *tmp);
				delete tmp;
			}
			catch (const std::exception &e)
			{
				delete result;
				throw;
			}
	return (result);
}

Vector*		Real::operator%(const Vector &other) const
{
	unsigned long int	size(other.size());
	Vector*					result;
	Real*				tmp;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		try
		{
			tmp = *this % other[i];
			result[i] = *tmp;
			delete tmp;
		}
		catch (const std::exception &e)
		{
			delete result;
			throw;
		}
	return (result);
}

Polynomial*	Real::operator%(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

IType*		Real::operator%(const IType &other) const
{
	const Real*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_rational = dynamic_cast<const Real*>(&other);
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

Real*	Real::operator%(const long long int value) const
{
	return (*this % Real(value));
}

Real*	Real::operator^(const Real &other) const
{
	Real*	result;
	Real*	tmp;
	Real	power(other);

	if (!power.in_Z() || power < 0)
		throw UNSUPPORTED_EXPONENT;
	result = new Real(1);
	for (InfiniteInt i(0); i < power.getNumerator(); i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

IType*		Real::operator^(const IType &other) const
{
	Real	power;

	try
	{
		power = Real(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (*this ^ power);
}

Real*	Real::operator^(const long long int value) const
{
	return (*this ^ Real(value));
}


// Getters

InfiniteInt		Real::getNumerator(void) const
{
	return (this->_numerator);
}

InfiniteInt		Real::getDenominator(void) const
{
	return (this->_denominator);
}

InfiniteFloat	Real::getValue(void) const
{
	return (InfiniteFloat(this->_numerator) 	\
			/ InfiniteFloat(this->_denominator));
}


// Methods

IType*			Real::clone(void) const
{
	return (new Real(*this));
}

IType*			Real::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Real::matrix_operator(const IType &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Real::matrix_inversion(void) const
{
	throw ERROR_MATRIX_INVERSION_SQUARE;
	return (nullptr);
}

Real*		Real::gcd(const Real &other) const
{
	InfiniteInt	gcd_numerator;
	InfiniteInt	gcd_denominator;

	if (!this->_numerator)
		return (new Real(other));
	if (!other._numerator)
		return (new Real(*this));
	gcd_numerator = compute_gcd(this->_numerator, other._numerator);
	gcd_denominator = compute_lcm(this->_denominator, other._denominator);
	return (new Real(gcd_numerator, gcd_denominator));
}

Real*		Real::gcd(const Complex &other) const
{
	Real*	first;
	Real*	second;

	first = this->gcd(other.getReal());
	second = first->gcd(other.getImaginary());
	delete first;
	return (second);
}

Real*		Real::gcd(const Matrix &other) const
{
	Real*	gcd;
	Real*	tmp;

	gcd = new Real(*this);
	for (unsigned long int i = 0; i < other.getHeight(); i++)
		for (unsigned long int j = 0; j < other.getWidth(); j++)
		{
			tmp = gcd;
			gcd = gcd->gcd(other[i][j]);
			delete tmp;
		}
	return (gcd);
}

Real*		Real::gcd(const Vector &other) const
{
	Real*	gcd;
	Real*	tmp;

	gcd = new Real(*this);
	for (unsigned long int i = 0; i < other.size(); i++)
	{
		tmp = gcd;
		gcd = gcd->gcd(other[i]);
		delete tmp;
	}
	return (gcd);
}

Real*		Real::gcd(const IType &other) const
{
	const Real*	other_rational;
	const Complex*	other_complex;
	const Matrix*	other_matrix;

	other_rational = dynamic_cast<const Real*>(&other);
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

std::ostream&	Real::print(std::ostream &os) const
{
	Real	copy(*this);
	InfiniteInt	numerator;

	numerator = copy.getNumerator();
	if (copy.in_Z())
		return (os << numerator);
	return (os << numerator << "/" << copy.getDenominator());
}

std::string		Real::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

void			Real::reduce(void)
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

bool			Real::in_D(void) const
{
	return (this->getValue().in_D());
}

bool			Real::in_Z(void) const
{
	Real	copy(*this);

	copy.reduce();
	return (copy.getDenominator() == InfiniteInt(1));
}

InfiniteInt		Real::integer_part(void) const
{
	return (this->_numerator / this->_denominator);
}

void			Real::print_rounded(const std::string var) const
{
	if (this->in_Z())
		return ;
	if (!var.empty())
	{
		std::cout << var;
		if (this->in_D())
			std::cout << " = ";
		else
			std::cout << " ≈ ";
	}
	std::cout << this->getValue() << std::endl;
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Real &num)
{
	return (num.print(os));
}


// Functions
InfiniteInt	compute_gcd(InfiniteInt a, InfiniteInt b)
{
	if (!b)
		return (a >= 0 ? a : -a);
	return (compute_gcd(b, a % b));
}
