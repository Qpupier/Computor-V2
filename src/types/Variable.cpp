/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:19:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/10 17:20:46 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Variable.hpp"
#include "UnsupportedError.hpp"

// Utils
static bool			is_multiplication_supported(const Variable &var1, const Variable &var2)
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

static bool			is_division_supported(const Variable &var1, const Variable &var2)
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

static Variable*	polynomial_divion(const Variable &var1, const Variable &var2)
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
	return (new Variable(var1.getName(), power2, power1, power0));
}

static Variable*	division_with_power2(const Variable &var1, const Variable &var2)
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
	return (new Variable(var1.getName(), power2, power1, power0));
}

static Variable*	division_with_power1(const Variable &var1, const Variable &var2)
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
	return (new Variable(var1.getName(), power2, power1, power0));
}

static Variable*	division_with_power0(const Variable &var1, const Variable &var2)
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
	return (new Variable(var1.getName(), power2, power1, power0));
}


// Destructor
Variable::~Variable(void)
{
	delete this->_power2;
	delete this->_power1;
	delete this->_power0;
}


// Operator overloads
Variable&	Variable::operator=(const Variable &other)
{
	if (this != &other)
	{
		this->~Variable();
		this->_name = other._name;
		this->_power2 = other._power2->clone();
		this->_power1 = other._power1->clone();
		this->_power0 = other._power0->clone();
	}
	return (*this);
}

Variable::operator bool() const
{
	return (!*this->_power2 && !*this->_power1 && !*this->_power0);
}

Variable*	Variable::operator+(const Variable &other) const
{
	if (this->_name != other._name)
		throw UnsupportedError("Multiple unknown variables are not supported");
	return (new Variable(this->_name, *this->_power2 + *other._power2, *this->_power1 + *other._power1, *this->_power0 + *other._power0));
}

Variable*	Variable::operator+(const Rational &other) const
{
	return (new Variable(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 + other));
}

Variable*	Variable::operator+(const Complex &other) const
{
	return (new Variable(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 + other));
}

Variable*	Variable::operator+(const Matrix &other) const
{
	return (new Variable(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 + other));
}

IType*		Variable::operator+(const IType &other) const
{
	const Variable	*other_variable;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (*this + *other_variable);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	return (nullptr);
}

Variable*	Variable::operator-(const Variable &other) const
{
	if (this->_name != other._name)
		throw UnsupportedError("Multiple unknown variables are not supported");
	return (new Variable(this->_name, *this->_power2 - *other._power2, *this->_power1 - *other._power1, *this->_power0 - *other._power0));
}

Variable*	Variable::operator-(const Rational &other) const
{
	return (new Variable(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 - other));
}

Variable*	Variable::operator-(const Complex &other) const
{
	return (new Variable(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 - other));
}

Variable*	Variable::operator-(const Matrix &other) const
{
	return (new Variable(this->_name, this->_power2->clone(), this->_power1->clone(), *this->_power0 - other));
}

IType*		Variable::operator-(const IType &other) const
{
	const Variable	*other_variable;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (*this - *other_variable);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	return (nullptr);
}

Variable*	Variable::operator*(const Variable &other) const
{
	IType	*power2;
	IType	*power1;
	IType	*tmp1;
	IType	*tmp2;
	IType	*tmp3;

	if (this->_name != other._name)
		throw UnsupportedError("Multiple unknown variables are not supported");
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
	return (new Variable(this->_name, power2, power1, *this->_power0 * *other._power0));
}

Variable*	Variable::operator*(const Rational &other) const
{
	return (new Variable(this->_name, *this->_power2 * other, *this->_power1 * other, *this->_power0 * other));
}

Variable*	Variable::operator*(const Complex &other) const
{
	return (new Variable(this->_name, *this->_power2 * other, *this->_power1 * other, *this->_power0 * other));
}

Variable*	Variable::operator*(const Matrix &other) const
{
	return (new Variable(this->_name, *this->_power2 * other, *this->_power1 * other, *this->_power0 * other));
}

IType*		Variable::operator*(const IType &other) const
{
	const Variable	*other_variable;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (*this * *other_variable);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	return (nullptr);
}

Variable*	Variable::operator/(const Variable &other) const
{
	if (this->_name != other._name)
		throw UnsupportedError("Multiple unknown variables are not supported");
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

Variable*	Variable::operator/(const Rational &other) const
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
	return (new Variable(this->_name, power2, power1, power0));
}

Variable*	Variable::operator/(const Complex &other) const
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
	return (new Variable(this->_name, power2, power1, power0));
}

Variable*	Variable::operator/(const Matrix &other) const
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
	return (new Variable(this->_name, power2, power1, power0));
}

IType*		Variable::operator/(const IType &other) const
{
	const Variable	*other_variable;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (*this / *other_variable);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	return (nullptr);
}

Variable*	Variable::operator%(const Variable &other) const
{
	Variable	*division;
	Rational	*rational;
	Variable	*tmp;
	Variable	*result;

	division = *this / other;
	rational = dynamic_cast<Rational*>(division->getPower0());
	if (division->getPower2() || division->getPower1() || !rational)
	{
		delete division;
		throw std::logic_error("Impossible modulo");
	}
	tmp = other * Rational(rational->integer_part());
	delete division;
	result = *this - *tmp;
	delete tmp;
	return (result);
}

Variable*	Variable::operator%(const Rational &other) const
{
	return (*this % Variable(this->_name, new Rational(0), new Rational(0), other.clone()));
}

Variable*	Variable::operator%(const Complex &other) const
{
	return (*this % Variable(this->_name, new Rational(0), new Rational(0), other.clone()));
}

Variable*	Variable::operator%(const Matrix &other) const
{
	return (*this % Variable(this->_name, new Rational(0), new Rational(0), other.clone()));
}

IType*		Variable::operator%(const IType &other) const
{
	const Variable	*other_variable;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (*this % *other_variable);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	return (nullptr);
}

Variable*	Variable::operator^(const Variable &other) const
{
	Rational	*power0;

	if (this->_name != other._name)
		throw UnsupportedError("Multiple unknown variables are not supported");
	if (other._power2 || other._power1)
		throw UnsupportedError("An unknown variable cannot be a power");
	power0 = dynamic_cast<Rational*>(other.getPower0());
	if (!power0)
		throw std::logic_error("Impossible modulo");
	return (*this ^ *power0);
}

Variable*	Variable::operator^(const Rational &other) const
{
	Rational	exponent;
	Variable*	result;
	Variable*	tmp;
	int			numerator;

	try
	{
		exponent = other;
		if (!exponent.is_integer())
			throw ERROR_EXPONENT_INTEGER;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_EXPONENT_INTEGER;
	}
	result = new Variable(*this);
	numerator = exponent.getNumerator();
	for (int i = 1; i < numerator; i++)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	return (result);
}

Variable*	Variable::operator^(const Complex &other) const
{
	return (*this ^ Variable(this->_name, new Rational(0), new Rational(0), other.clone()));
}

Variable*	Variable::operator^(const Matrix &other) const
{
	return (*this ^ Variable(this->_name, new Rational(0), new Rational(0), other.clone()));
}

IType*		Variable::operator^(const IType &other) const
{
	const Variable	*other_variable;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (*this ^ *other_variable);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this ^ *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this ^ *other_matrix);
	return (nullptr);
}


// Getters
std::string	Variable::getName(void) const
{
	return (this->_name);
}

IType*		Variable::getPower2(void) const
{
	return (this->_power2);
}

IType*		Variable::getPower1(void) const
{
	return (this->_power1);
}

IType*		Variable::getPower0(void) const
{
	return (this->_power0);
}


// Methods
IType*			Variable::matrix_operator(const Variable &other) const
{
	if (!this->_power2 && !this->_power1)
		return (this->matrix_operator(*other.getPower0()));
	throw ERROR_MATRIX_OPERATOR;
	return (nullptr);
}

IType*			Variable::matrix_operator(const Matrix &other) const
{
	if (this->_power2 || this->_power1)
		throw ERROR_MATRIX_OPERATOR;
	return (this->_power0->matrix_operator(other));
}

IType*			Variable::matrix_operator(const IType &other) const
{
	const Variable	*other_variable;
	const Matrix	*other_matrix;

	other_variable = dynamic_cast<const Variable*>(&other);
	if (other_variable)
		return (this->matrix_operator(*other_variable));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->matrix_operator(*other_matrix));
	throw ERROR_MATRIX_OPERATOR;
	return (nullptr);
}

IType*			Variable::clone(void) const
{
	return (new Variable(*this));
}

std::ostream&	Variable::print(std::ostream &os) const
{
	if (*this->_power2)// To improve
		os << *this->_power2 << this->_name << "^2";
	if (*this->_power1)
	{
		if (*this->_power2)
			os << " + ";
		os << *this->_power1 << this->_name;
	}
	if (*this->_power0)
	{
		if (*this->_power2 || *this->_power1)
			os << " + ";
		os << *this->_power0;
	}
	return (os);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Variable &var)
{
	return (var.print(os));
}
