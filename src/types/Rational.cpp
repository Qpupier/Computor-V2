/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rational.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 19:46:50 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/23 14:51:57 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Rational.hpp"

// Utils

static Rational		from_complex(const Complex& complex)
{
	if (*complex.getImaginary())
		throw LogicError("A complex number cannot be converted to a "
				"rational number");
	return (Rational(*complex.getReal()));
}

static Rational		from_polynomial(const Polynomial& polynomial)
{
	if (polynomial.getTerms().empty())
		return (Rational(0));
	if (polynomial.getDividers().size() != 1 					
			|| *polynomial.getDividers()[0].coefficient != 1 	
			|| polynomial.getDividers()[0].power 				
			|| polynomial.getTerms().size() != 1 				
			|| polynomial.getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	return (Rational(*polynomial.getTerms()[0].coefficient));
}

static Rational		from_real(const Real& real)
{
	Rational					result;
	std::vector<unsigned char>	all_digits							\
			(real.getValue().getIntegerPart().getDigits());
	std::vector<unsigned char>	decimal_digits;
	Rational*					divider;
	Rational*					tmp_result;

	if (all_digits.size() > InfiniteFloat::PRINT_PRECISION)
		throw LogicError("This real number isn't a rational number");
	decimal_digits = real.getValue().getDecimalPart().getDigits();
	all_digits.insert(all_digits.end(), decimal_digits.begin(), 	\
			decimal_digits.end());
	result = Rational(InfiniteInt(all_digits, 						\
			real.getValue().getIsNegative(), true));
	divider = Rational(10) ^ static_cast<long long int>(decimal_digits.size());
	tmp_result = result / *divider;
	delete divider;
	result = *tmp_result;
	delete tmp_result;
	return (result);
}

static InfiniteInt	compute_lcm(InfiniteInt a, InfiniteInt b)
{
	return (a / compute_gcd(a, b) * b);
}

static IType*		sqrt_final_value(InfiniteFloat sqrt_value, 		\
		InfiniteFloat value)
{
	InfiniteFloat				test_exact_value;
	std::vector<unsigned char>	decimal_part;

	decimal_part = sqrt_value.getDecimalPart().getDigits();
	for (std::size_t i(InfiniteFloat::PRINT_PRECISION); 	\
			i < decimal_part.size() && i < InfiniteFloat::MAX_PRECISION; i++)
		decimal_part[i] = 0;
	if (!InfiniteInt(decimal_part))
		return (new Rational(sqrt_value.getIntegerPart()));
	test_exact_value = InfiniteFloat(sqrt_value.getIntegerPart(), decimal_part);
	if ((test_exact_value ^ 2) == value)
		return (new Real(test_exact_value));
	return (new Real(sqrt_value));
}

static Rational*	get_rational_new_term_coeff(long long int k, 	\
		const Rational& k2)
{
	Rational*	coeff;
	Rational*	coeff_num;
	Rational*	factorial;

	coeff_num = Rational(-1) ^ k;
	factorial = k2.fact();
	coeff = *coeff_num / *factorial;
	delete coeff_num;
	delete factorial;
	return (coeff);
}

static Rational*	cos_get_new_term(const Rational& number, long long int k)
{
	Rational*	coeff;
	Rational*	k2;
	Rational*	new_term;
	Rational*	real_power;

	k2 = Rational(k) * 2;
	coeff = get_rational_new_term_coeff(k, *k2);
	real_power = number ^ *k2;
	delete k2;
	new_term = *coeff * *real_power;
	delete coeff;
	delete real_power;
	return (new_term);
}

static Rational*	sin_get_new_term(const Rational& number, long long int k)
{
	Rational*	coeff;
	Rational*	k2;
	Rational*	new_term;
	Rational*	real_power;
	Rational*	tmp;

	k2 = Rational(k) * 2;
	tmp = k2;
	k2 = *tmp + 1;
	delete tmp;
	coeff = get_rational_new_term_coeff(k, *k2);
	real_power = number ^ *k2;
	delete k2;
	new_term = *coeff * *real_power;
	delete coeff;
	delete real_power;
	return (new_term);
}

static Rational*	exp_get_new_term(const Rational& number, long long int k)
{
	Rational*	power;
	Rational*	factorial;
	Rational*	new_term;

	power = number ^ k;
	factorial = Rational(k).fact();
	new_term = *power / *factorial;
	delete power;
	delete factorial;
	return (new_term);
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
	Rational*	result;
	Rational*	tmp;

	slash_pos = str.find('.');
	if (slash_pos == std::string::npos)
	{
		_numerator = InfiniteInt(str);
		_denominator = InfiniteInt(1);
	}
	else
	{
		result = new Rational(InfiniteInt(str.erase(slash_pos, 1)));
		for (std::size_t i(0); i < str.size() - slash_pos; i++)
		{
			tmp = result;
			result = *result / 10;
			delete tmp;
		}
		_numerator = result->getNumerator();
		_denominator = result->getDenominator();
		delete result;
	}
	this->reduce();
}

Rational::Rational(const IType& other)
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_rational = dynamic_cast<const Rational*>(&other);
	other_complex = dynamic_cast<const Complex*>(&other);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	other_vector = dynamic_cast<const Vector*>(&other);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_rational)
		*this = *other_rational;
	else if (other_complex)
		*this = from_complex(*other_complex);
	else if (other_polynomial)
		*this = from_polynomial(*other_polynomial);
	else if (other_real)
		*this = from_real(*other_real);
	else if (other_matrix)
		throw LogicError("A matrix cannot be converted to a rational number");
	else if (other_vector)
		throw LogicError("A vector cannot be converted to a rational number");
	else
		throw ERROR_UNEXPECTED;
	this->reduce();
}


// Operator overloads

Rational::operator bool() const
{
	return (static_cast<bool>(this->_numerator));
}

Rational&	Rational::operator=(const Rational* other)
{
	if (this != other)
	{
		this->_numerator = other->_numerator;
		this->_denominator = other->_denominator;
	}
	return (*this);
}

Rational&	Rational::operator=(const Rational& other)
{
	if (this !=& other)
	{
		this->_numerator = other._numerator;
		this->_denominator = other._denominator;
	}
	return (*this);
}

Rational	Rational::operator=(const IType& other)
{
	*this = Rational(other);
	return (*this);
}

bool		Rational::operator==(const IType& other) const
{
	Rational	other_rational;

	try
	{
		other_rational = Rational(other);
	}
	catch(const UnexpectedError& e)
	{
		return (false);
	}
	return (this->_numerator == other_rational._numerator 	\
			&& this->_denominator == other_rational._denominator);
}

bool		Rational::operator==(const long long int value) const
{
	return (*this == Rational(value));
}

bool		Rational::operator!=(const IType& other) const
{
	return (!(*this == other));
}

bool		Rational::operator!=(const long long int value) const
{
	return (*this != Rational(value));
}

bool		Rational::operator<(const IType& other) const
{
	Real	other_real;

	try
	{
		other_real = Real(other);
	}
	catch(const UnexpectedError& e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (Real(*this) < Real(other_real));
}

bool		Rational::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Rational::operator<=(const IType& other) const
{
	return (*this < other || *this == other);
}

bool		Rational::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Rational::operator>(const IType& other) const
{
	Rational	other_rational;

	try
	{
		other_rational = Rational(other);
	}
	catch(const UnexpectedError& e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (!(*this <= other_rational));
}

bool		Rational::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Rational::operator>=(const IType& other) const
{
	Rational	other_rational;

	try
	{
		other_rational = Rational(other);
	}
	catch(const UnexpectedError& e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (!(*this < other_rational));
}

bool		Rational::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

Rational*	Rational::operator+(const Rational& other) const
{
	return (new Rational(this->_numerator * other._denominator 	\
				+ other._numerator * this->_denominator, 		\
			this->_denominator * other._denominator));
}

Complex*	Rational::operator+(const Complex& other) const
{
	return (Complex(this->clone(), new Rational()) + other);
}

Matrix*		Rational::operator+(const Matrix& other) const
{
	Matrix*				result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			result->setValue(i, j, *this + *other[j][i]);
	return (result);
}

IType*		Rational::operator+(const Polynomial& other) const
{
	return (other + *this);
}

Vector*		Rational::operator+(const Vector& other) const
{
	unsigned long int	size(other.size());
	Vector*				result;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		result->setValue(i, *this + *other[i]);
	return (result);
}

Real*		Rational::operator+(const Real& other) const
{
	return (new Real((this->getValue() + other.getValue())));
}

IType*		Rational::operator+(const IType& other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this + *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this + *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator+(const long long int value) const
{
	return (*this + Rational(value));
}

Rational*	Rational::operator-(void) const
{
	return (*this * (-1));
}

Rational*	Rational::operator-(const Rational& other) const
{
	return (new Rational(this->_numerator * other._denominator 	\
				- other._numerator * this->_denominator, 		\
			this->_denominator * other._denominator));
}

Complex*	Rational::operator-(const Complex& other) const
{
	return (Complex(this->clone(), new Rational()) - other);
}

Matrix*		Rational::operator-(const Matrix& other) const
{
	Matrix*				result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			result->setValue(i, j, *this - *other[j][i]);
	return (result);
}

IType*		Rational::operator-(const Polynomial& other) const
{
	IType*	tmp;
	IType*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

Vector*		Rational::operator-(const Vector& other) const
{
	unsigned long int	size(other.size());
	Vector*				result;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		result->setValue(i, *this - *other[i]);
	return (result);
}

Real*		Rational::operator-(const Real& other) const
{
	return (new Real(this->getValue() - other.getValue()));
}

IType*		Rational::operator-(const IType& other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this - *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this - *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

Rational*	Rational::operator*(const Rational& other) const
{
	return (new Rational(this->_numerator * other._numerator, 	\
			this->_denominator * other._denominator));
}

Complex*	Rational::operator*(const Complex& other) const
{
	return (Complex(this->clone(), new Rational()) * other);
}

Matrix*		Rational::operator*(const Matrix& other) const
{
	Matrix*				result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			result->setValue(i, j, *this * *other[j][i]);
	return (result);
}

IType*		Rational::operator*(const Polynomial& other) const
{
	return (other * *this);
}

Vector*		Rational::operator*(const Vector& other) const
{
	unsigned long int	size(other.size());
	Vector*				result;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		result->setValue(i, *this * *other[i]);
	return (result);
}

Real*		Rational::operator*(const Real& other) const
{
	return (new Real(this->getValue() * other.getValue()));
}

IType*		Rational::operator*(const IType& other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this * *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this * *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

Rational*	Rational::operator/(const Rational& other) const
{
	return (*this * Rational(other.getDenominator(), other.getNumerator()));
}

Complex*	Rational::operator/(const Complex& other) const
{
	return (Complex(this->clone(), new Rational()) / other);
}

Matrix*		Rational::operator/(const Matrix& other) const
{
	Matrix*				result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			try
			{
				result->setValue(i, j, *this / *other[j][i]);
			}
			catch (const std::exception& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Polynomial*	Rational::operator/(const Polynomial& other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

Vector*		Rational::operator/(const Vector& other) const
{
	unsigned long int	size(other.size());
	Vector*				result;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		try
		{
			result->setValue(i, *this / *other[i]);
		}
		catch (const std::exception& e)
		{
			delete result;
			throw;
		}
	return (result);
}

Real*		Rational::operator/(const Real& other) const
{
	return (new Real(this->getValue() / other.getValue()));
}

IType*		Rational::operator/(const IType& other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this / *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this / *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

Rational*	Rational::operator%(const Rational& other) const
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

Complex*	Rational::operator%(const Complex& other) const
{
	return (Complex(this->clone(), new Rational()) % other);
}

Matrix*		Rational::operator%(const Matrix& other) const
{
	Matrix*				result;
	unsigned long int	width;
	unsigned long int	height;

	width = other.getWidth();
	height = other.getHeight();
	result = new Matrix(width, height);
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i(0); i < width; i++)
			try
			{
				result->setValue(i, j, *this % *other[j][i]);
			}
			catch (const std::exception& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Polynomial*	Rational::operator%(const Polynomial& other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

Vector*		Rational::operator%(const Vector& other) const
{
	unsigned long int	size(other.size());
	Vector*				result;

	result = new Vector(size);
	for (unsigned long int i(0); i < size; i++)
		try
		{
			result->setValue(i, *this % *other[i]);
		}
		catch (const std::exception& e)
		{
			delete result;
			throw;
		}
	return (result);
}

IType*		Rational::operator%(const Real& other) const
{
	return (new Real(this->getValue() % other.getValue()));
}

IType*		Rational::operator%(const IType& other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

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
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this % *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this % *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Rational*	Rational::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

Rational*	Rational::operator^(const Rational& other) const
{
	Rational*	result;
	Rational*	tmp;
	Rational	power(other);

	if (!power.in_Z() || power < 0)
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

IType*		Rational::operator^(const IType& other) const
{
	Rational	power;

	try
	{
		power = Rational(other);
	}
	catch(const UnexpectedError& e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	return (*this ^ power);
}

Rational*	Rational::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
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

InfiniteFloat	Rational::getValue(void) const
{
	return (InfiniteFloat(this->_numerator) 	\
			/ InfiniteFloat(this->_denominator));
}


// Methods

InfiniteInt		Rational::integer_part(void) const
{
	return (this->_numerator / this->_denominator);
}

std::string		Rational::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			Rational::in_C(void) const
{
	return (true);
}

bool			Rational::in_D(void) const
{
	return (this->getValue().in_D());
}

bool			Rational::in_M(void) const
{
	return (false);
}

bool			Rational::in_N(void) const
{
	return (this->in_Z() && *this >= 0);
}

bool			Rational::in_Q(void) const
{
	return (true);
}

bool			Rational::in_Z(void) const
{
	return (this->getDenominator() == 1);
}

std::ostream&	Rational::print(std::ostream& os) const
{
	Rational	copy(*this);
	InfiniteInt	numerator;

	numerator = copy.getNumerator();
	if (copy.in_Z())
		return (os << numerator);
	return (os << numerator << "/" << copy.getDenominator());
}

IType*			Rational::abs(void) const
{
	return (new Rational(this->_numerator.abs(), this->_denominator.abs()));
}

IType*			Rational::clone(void) const
{
	return (new Rational(*this));
}

IType*			Rational::cos(void) const
{
	long long int	k(0);
	Rational*		result;
	Rational*		new_term;
	Rational*		tmp;
	Real*			real_result;

	result = new Rational(0);
	while (true)
	{
		new_term  = cos_get_new_term(*this, k++);
		tmp = result;
		result = *result + *new_term;
		delete new_term;
		if (result->getValue() == tmp->getValue())
		{
			delete result;
			if (tmp->in_Z())
				return (tmp);
			real_result = new Real(*tmp);
			delete tmp;
			return (real_result);
		}
		delete tmp;
	}
	return (nullptr);
}

IType*			Rational::exp(void) const
{
	long long int	k(0);
	Rational*		result;
	Rational*		new_term;
	Rational*		tmp;
	Real*			real_result;

	result = new Rational(0);
	while (true)
	{
		new_term  = exp_get_new_term(*this, k++);
		tmp = result;
		result = *result + *new_term;
		delete new_term;
		if (result->getValue() == tmp->getValue())
		{
			delete result;
			if (tmp->in_Z())
				return (tmp);
			real_result = new Real(*tmp);
			delete tmp;
			return (real_result);
		}
		delete tmp;
	}
	return (nullptr);
}

IType*			Rational::function_operator(const IType& other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Rational::matrix_operator(const IType& other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	return (nullptr);
}

IType*			Rational::matrix_inversion(void) const
{
	throw ERROR_MATRIX_INVERSION_SQUARE;
	return (nullptr);
}

IType*			Rational::norm(void) const
{
	throw LogicError("Norm is not defined for rational numbers, "
			"use the absolute function instead");
	return (nullptr);
}

IType*			Rational::sin(void) const
{
	long long int	k(0);
	Rational*		result;
	Rational*		new_term;
	Rational*		tmp;
	Real*			real_result;

	result = new Rational(0);
	while (true)
	{
		new_term = sin_get_new_term(*this, k++);
		tmp = result;
		result = *result + *new_term;
		delete new_term;
		if (result->getValue() == tmp->getValue())
		{
			delete result;
			if (tmp->in_Z())
				return (tmp);
			real_result = new Real(*tmp);
			delete tmp;
			return (real_result);
		}
		delete tmp;
	}
	return (nullptr);
}

IType*			Rational::sqrt(void) const
{
	InfiniteFloat	sqrt_value(1);
	InfiniteFloat	value(this->getValue());
	InfiniteFloat	epsilon(1);
	InfiniteFloat	delta(1);

	if (value.getIsNegative())
		return (Complex(*this).sqrt());
	for (int i(0); i < InfiniteFloat::CALCULATION_PRECISION; i++)
		epsilon /= InfiniteFloat(10);
	while (delta >= epsilon)
	{
		sqrt_value = (sqrt_value + value / sqrt_value) / 2;
		delta = sqrt_value * sqrt_value - value;
		if (delta < 0)
			delta *= -1;
	}
	return (sqrt_final_value(sqrt_value, value));
}

IType*			Rational::tan(void) const
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

Rational*		Rational::fact(void) const// TODO: Implementer le parsing correspondant (!)
{
	Rational*	result;
	Rational*	next_number;
	Rational*	next_factorial;
	
	if (!this->in_N())
		throw ERROR_FACTORIAL_FUNCTION;
	if (!*this)
		return (new Rational(1));
	next_number = *this - 1;
	next_factorial = Rational(*next_number).fact();
	delete next_number;
	result = *this * *next_factorial;
	delete next_factorial;
	return (result);
}

Rational*		Rational::gcd(const Rational& other) const
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

Rational*		Rational::gcd(const Complex& other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(*other.getReal());
	second = first->gcd(*other.getImaginary());
	delete first;
	return (second);
}

Rational*		Rational::gcd(const Matrix& other) const
{
	Rational*	gcd;
	Rational*	tmp;

	gcd = new Rational(*this);
	for (unsigned long int i = 0; i < other.getHeight(); i++)
		for (unsigned long int j = 0; j < other.getWidth(); j++)
		{
			tmp = gcd;
			gcd = gcd->gcd(*other[i][j]);
			delete tmp;
		}
	return (gcd);
}

Rational*		Rational::gcd(const Polynomial& other) const
{
	return (other.gcd(*this));
}

Rational*		Rational::gcd(const Vector& other) const
{
	Rational*	gcd;
	Rational*	tmp;

	gcd = new Rational(*this);
	for (unsigned long int i = 0; i < other.size(); i++)
	{
		tmp = gcd;
		gcd = gcd->gcd(*other[i]);
		delete tmp;
	}
	return (gcd);
}

Rational*		Rational::gcd(const Real& other) const
{
	return (new Rational(1));
	(void)other;
}

Rational*		Rational::gcd(const IType& other) const
{
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->gcd(*other_rational));
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->gcd(*other_complex));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->gcd(*other_matrix));
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (this->gcd(*other_polynomial));
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (this->gcd(*other_vector));
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (this->gcd(*other_real));
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Real*			Rational::deg(void) const
{
	return (Real(*this).deg());
}

Real*			Rational::rad(void) const
{
	return (Real(*this).rad());
}

void			Rational::reduce(void)
{
	InfiniteInt	gcd;

	if (this->_denominator < 0)
	{
		this->_numerator *= -1;
		this->_denominator *= -1;
	}
	gcd = compute_gcd(this->_numerator, this->_denominator);
	if (!gcd)
	{
		this->_numerator = InfiniteInt();
		this->_denominator = InfiniteInt(1);
		return ;
	}
	if (gcd == 1)
		return ;
	this->_numerator /= gcd;
	this->_denominator /= gcd;
	if (this->_denominator < 0)
	{
		this->_numerator = -this->_numerator;
		this->_denominator = -this->_denominator;
	}
}

void			Rational::print_rounded(const std::string var) const
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
std::ostream&	operator<<(std::ostream& os, const Rational& num)
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
