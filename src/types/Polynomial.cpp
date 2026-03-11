/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:19:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 17:18:55 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"

// Utils
static bool			is_multiplication_supported(const Polynomial &var1, 	\
		const Polynomial &var2)
{
	IType	*power4;
	IType	*power3;
	IType	*tmp1;
	IType	*tmp2;
	bool	result;

	power4 = *var1.getPower2() * *var2.getPower2();
	tmp1 = *var1.getPower2() * *var2.getPower1();
	tmp2 = *var1.getPower1() * *var2.getPower2();
	power3 = *tmp1 + *tmp2;
	delete tmp1;
	delete tmp2;
	result = *power3 || *power4;
	delete power3;
	delete power4;
	return (!result);
}

static bool			is_division_supported(const Polynomial &var1, 		\
		const Polynomial &var2)
{
	IType	*test1;
	IType	*test2;
	IType	*tmp1;
	IType	*tmp2;
	bool	result;

	tmp1 = *var1.getPower1() * *var2.getPower2();
	tmp2 = *var1.getPower2() * *var2.getPower1();
	test1 = *tmp1 - *tmp2;
	delete tmp1;
	delete tmp2;
	tmp1 = *var1.getPower0() * *var2.getPower2();
	tmp2 = *var1.getPower2() * *var2.getPower0();
	test2 = *tmp1 - *tmp2;
	delete tmp1;
	delete tmp2;
	result = test1 || test2;
	delete test1;
	delete test2;
	return (!result);
}

static Polynomial*	polynomial_divion(const Polynomial &var1, 			\
		const Polynomial &var2)
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	power2 = new Rational(0);
	power1 = new Rational(0);
	try
	{
		power0 = *var1.getPower2() / *var2.getPower2();
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(var1.getName(), power2, power1, power0));
}

static Polynomial*	division_with_power2(const Polynomial &var1, 			\
		const Polynomial &var2)
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	if (var1.getPower1() || var1.getPower0())
		throw UNSUPPORTED_DIVISION;
	power2 = new Rational(0);
	power1 = new Rational(0);
	try
	{
		power0 = *var1.getPower2() / *var2.getPower2();
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(var1.getName(), power2, power1, power0));
}

static Polynomial*	division_with_power1(const Polynomial &var1, 			\
		const Polynomial &var2)
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	if (!var1.getPower0())
		throw UNSUPPORTED_DIVISION;
	power2 = new Rational(0);
	try
	{
		power1 = *var1.getPower2() / *var2.getPower1();
	}
	catch (...)
	{
		delete power2;
		throw;
	}
	try
	{
		power0 = *var1.getPower1() / *var2.getPower1();
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(var1.getName(), power2, power1, power0));
}

static Polynomial*	division_with_power0(const Polynomial &var1, 			\
		const Polynomial &var2)
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	power2 = *var1.getPower2() / *var2.getPower0();
	try
	{
		power1 = *var1.getPower1() / *var2.getPower0();
	}
	catch (...)
	{
		delete power2;
		throw;
	}
	try
	{
		power0 = *var1.getPower0() / *var2.getPower0();
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(var1.getName(), power2, power1, power0));
}

static void			print_first_power(std::ostream &os, 				\
		const IType *power, const std::string &var, const char *exponent)
{
	Rational	minus_one(-1);

	if (!*power)
		return ;
	if (*power == minus_one)
		os << "-" << var << exponent;
	else if (var == std::string() || *power != Rational(1))
		power->print_polynomial(os, var + exponent);
	else
		os << var << exponent;
}

static void			print_power(std::ostream &os, bool first_power, 	\
		const IType *power, const std::string &var)
{
	Rational	minus_one(-1);
	IType		*copy;
	IType		*tmp;

	if (!*power)
		return ;
	copy = power->clone();
	if (!first_power)
	{
		if (*power < Rational(0))
		{
			os << " - ";
			tmp = copy;
			copy = *copy * minus_one;
			delete tmp;
		}
		else
			os << " + ";
	}
	print_first_power(os, copy, var, "");
	delete copy;
}


// Destructor
Polynomial::~Polynomial(void)
{
	delete this->_power2;
	delete this->_power1;
	delete this->_power0;
}


// Operator overloads
Polynomial&	Polynomial::operator=(const Polynomial &other)
{
	if (this != &other)
	{
		this->~Polynomial();
		this->_name = other._name;
		this->_power2 = other._power2->clone();
		this->_power1 = other._power1->clone();
		this->_power0 = other._power0->clone();
	}
	return (*this);
}

Polynomial::operator bool() const
{
	return (!*this->_power2 && !*this->_power1 && !*this->_power0);
}

bool		Polynomial::operator==(const Polynomial &other) const
{
	return (this->_name == other._name && *this->_power2 == *other._power2 && *this->_power1 == *other._power1 && *this->_power0 == *other._power0);
}

bool		Polynomial::operator==(const Rational &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 == other);
}

bool		Polynomial::operator==(const Complex &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 == other);
}

bool		Polynomial::operator==(const Matrix &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 == other);
}

bool		Polynomial::operator==(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this == *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this == *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this == *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this == *other_matrix);
	throw ERROR_UNEXPECTED;
	return (false);
}

bool		Polynomial::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Polynomial::operator<(const IType &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 < other);
}

bool		Polynomial::operator<=(const IType &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 <= other);
}

bool		Polynomial::operator>(const IType &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 > other);
}

bool		Polynomial::operator>=(const IType &other) const
{
	return (!*this->_power2 && !*this->_power1 && *this->_power0 >= other);
}

Polynomial*	Polynomial::operator+(const Polynomial &other) const
{
	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	return (new Polynomial(this->_name, *this->_power2 + *other._power2, *this->_power1 + *other._power1, *this->_power0 + *other._power0));
}

Polynomial*	Polynomial::operator+(const Rational &other) const
{
	return (new Polynomial(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 + other));
}

Polynomial*	Polynomial::operator+(const Complex &other) const
{
	return (new Polynomial(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 + other));
}

Polynomial*	Polynomial::operator+(const Matrix &other) const
{
	return (new Polynomial(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 + other));
}

IType*		Polynomial::operator+(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator-(const Polynomial &other) const
{
	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	return (new Polynomial(this->_name, *this->_power2 - *other._power2, *this->_power1 - *other._power1, *this->_power0 - *other._power0));
}

Polynomial*	Polynomial::operator-(const Rational &other) const
{
	return (new Polynomial(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 - other));
}

Polynomial*	Polynomial::operator-(const Complex &other) const
{
	return (new Polynomial(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 - other));
}

Polynomial*	Polynomial::operator-(const Matrix &other) const
{
	return (new Polynomial(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 - other));
}

IType*		Polynomial::operator-(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator*(const Polynomial &other) const
{
	IType	*power2;
	IType	*power1;
	IType	*tmp1;
	IType	*tmp2;
	IType	*tmp3;

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	if (!is_multiplication_supported(*this, other))
		throw UnsupportedError("Powers higher than 2 are not supported in polynomial expressions");
	tmp1 = *this->_power2 * *other._power0;
	tmp2 = *this->_power1 * *other._power1;
	tmp3 = *tmp1 + *tmp2;
	delete tmp1;
	delete tmp2;
	tmp1 = *this->_power0 * *other._power2;
	power2 = *tmp3 + *tmp1;
	delete tmp1;
	delete tmp3;
	tmp1 = *this->_power1 * *other._power0;
	tmp2 = *this->_power0 * *other._power1;
	power1 = *tmp1 + *tmp2;
	delete tmp1;
	delete tmp2;
	return (new Polynomial(this->_name, power2, power1, *this->_power0 * *other._power0));
}

Polynomial*	Polynomial::operator*(const Rational &other) const
{
	return (new Polynomial(this->_name, *this->_power2 * other, *this->_power1 * other, *this->_power0 * other));
}

Polynomial*	Polynomial::operator*(const Complex &other) const
{
	return (new Polynomial(this->_name, *this->_power2 * other, *this->_power1 * other, *this->_power0 * other));
}

Polynomial*	Polynomial::operator*(const Matrix &other) const
{
	return (new Polynomial(this->_name, *this->_power2 * other, *this->_power1 * other, *this->_power0 * other));
}

IType*		Polynomial::operator*(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator/(const Polynomial &other) const
{
	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	if (is_division_supported(*this, other))
		return (polynomial_divion(*this, other));
	if (!other._power1 && !other._power0)
		return (division_with_power2(*this, other));
	if (!other._power2 && !other._power0)
		return (division_with_power1(*this, other));
	if (!other._power2 && !other._power1)
		return (division_with_power0(*this, other));
	throw UNSUPPORTED_DIVISION;
	return (nullptr);
}

Polynomial*	Polynomial::operator/(const Rational &other) const
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	power2 = *this->_power2 / other;
	try
	{
		power1 = *this->_power1 / other;
	}
	catch (...)
	{
		delete power2;
		throw;
	}
	try
	{
		power0 = *this->_power0 / other;
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(this->_name, power2, power1, power0));
}

Polynomial*	Polynomial::operator/(const Complex &other) const
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	power2 = *this->_power2 / other;
	try
	{
		power1 = *this->_power1 / other;
	}
	catch (...)
	{
		delete power2;
		throw;
	}
	try
	{
		power0 = *this->_power0 / other;
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(this->_name, power2, power1, power0));
}

Polynomial*	Polynomial::operator/(const Matrix &other) const
{
	IType	*power2;
	IType	*power1;
	IType	*power0;

	power2 = *this->_power2 / other;
	try
	{
		power1 = *this->_power1 / other;
	}
	catch (...)
	{
		delete power2;
		throw;
	}
	try
	{
		power0 = *this->_power0 / other;
	}
	catch (...)
	{
		delete power2;
		delete power1;
		throw;
	}
	return (new Polynomial(this->_name, power2, power1, power0));
}

IType*		Polynomial::operator/(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator%(const Polynomial &other) const
{
	Polynomial	*division;
	Rational	*rational;
	Polynomial	*tmp;
	Polynomial	*result;

	division = *this / other;
	rational = dynamic_cast<Rational*>(division->getPower0());
	if (division->getPower2() || division->getPower1() || !rational)
	{
		delete division;
		throw LogicError("Only rational numbers can be used as modulo");
	}
	tmp = other * Rational(rational->integer_part());
	delete division;
	result = *this - *tmp;
	delete tmp;
	return (result);
}

Polynomial*	Polynomial::operator%(const Rational &other) const
{
	return (*this % Polynomial(this->_name, new Rational(0), new Rational(0), other.clone()));
}

Polynomial*	Polynomial::operator%(const Complex &other) const
{
	return (*this % Polynomial(this->_name, new Rational(0), new Rational(0), other.clone()));
}

Polynomial*	Polynomial::operator%(const Matrix &other) const
{
	return (*this % Polynomial(this->_name, new Rational(0), new Rational(0), other.clone()));
}

IType*		Polynomial::operator%(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator^(const Polynomial &other) const
{
	Rational	*power0;

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	if (other._power2 || other._power1)
		throw UnsupportedError("An unknown variable cannot be a power");
	power0 = dynamic_cast<Rational*>(other.getPower0());
	if (!power0)
		throw LogicError("Only rational numbers can be used as exponent");
	return (*this ^ *power0);
}

Polynomial*	Polynomial::operator^(const Rational &other) const
{
	Rational	exponent;
	Polynomial*	result;
	Polynomial*	tmp;
	int			numerator;

	try
	{
		exponent = other;
		if (!exponent.is_integer())
			throw ERROR_EXPONENT_INTEGER;
	}
	catch (const LogicError &e)
	{
		throw ERROR_EXPONENT_INTEGER;
	}
	result = new Polynomial(*this);
	numerator = exponent.getNumerator();
	for (int i = 1; i < numerator; i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

Polynomial*	Polynomial::operator^(const Complex &other) const
{
	return (*this ^ Polynomial(this->_name, new Rational(0), new Rational(0), other.clone()));
}

Polynomial*	Polynomial::operator^(const Matrix &other) const
{
	return (*this ^ Polynomial(this->_name, new Rational(0), new Rational(0), other.clone()));
}

IType*		Polynomial::operator^(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this ^ *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this ^ *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this ^ *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}


// Getters
std::string	Polynomial::getName(void) const
{
	return (this->_name);
}

IType*		Polynomial::getPower2(void) const
{
	return (this->_power2);
}

IType*		Polynomial::getPower1(void) const
{
	return (this->_power1);
}

IType*		Polynomial::getPower0(void) const
{
	return (this->_power0);
}


// Methods
IType*			Polynomial::matrix_operator(const Polynomial &other) const
{
	if (!this->_power2 && !this->_power1)
		return (this->matrix_operator(*other.getPower0()));
	throw ERROR_MATRIX_OPERATOR;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Polynomial::matrix_operator(const Matrix &other) const
{
	if (this->_power2 || this->_power1)
		throw ERROR_MATRIX_OPERATOR;
	return (this->_power0->matrix_operator(other));
}

IType*			Polynomial::matrix_operator(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Matrix	*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (this->matrix_operator(*other_polynomial));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->matrix_operator(*other_matrix));
	throw ERROR_MATRIX_OPERATOR;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Polynomial::clone(void) const
{
	return (new Polynomial(*this));
}

std::ostream&	Polynomial::print(std::ostream &os) const
{
	print_power(os, true, this->_power2, this->_name + std::string("^2"));
	print_power(os, !*this->_power2, this->_power1, this->_name);
	print_power(os, !*this->_power2 && !*this->_power1, this->_power0, std::string());
	if (!*this->_power2 && !*this->_power1 && !*this->_power0)
		os << "0";
	return (os);
}

std::ostream&	Polynomial::print_polynomial(std::ostream &os, 	\
		const std::string &var) const
{
	throw ERROR_UNEXPECTED;
	(void)var;
	return (os);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Polynomial &var)
{
	return (var.print(os));
}
