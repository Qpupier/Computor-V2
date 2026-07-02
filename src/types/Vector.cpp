/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 13:27:17 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/02 17:49:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Vector.hpp"
#include "AST.hpp"

// Utils

static std::vector<std::string>	parse_vector(std::string &vector)
{
	std::vector<std::string>	result;
	std::size_t					pos(0);

	vector = vector.substr(1, vector.size() - 2);
	while (pos != std::string::npos)
	{
		pos = vector.find(',');
		if (pos != std::string::npos)
		{
			result.push_back(vector.substr(0, pos));
			vector.erase(0, pos + 1);
		}
		else
			result.push_back(vector);
	}
	if (result.empty())
		throw LogicError("Invalid vector format: empty vector");
	return (result);
}

static Rational*				value_to_rational(std::string value, 	\
		t_data& data)
{
	AST*		cell;
	Rational*	rational;

	try
	{
		cell = compute_expression(value, data, true);
	}
	catch (const std::exception &e)
	{
		throw LogicError(std::string("Invalid vector format: ") + e.what());
	}
	if (!cell)
		throw LogicError("Invalid vector format: invalid element");
	if (cell->getLeft() || cell->getRight())
	{
		delete cell;
		throw LogicError("Invalid vector format: incomplete element");
	}
	rational = dynamic_cast<Rational*>(cell->getNode()->clone());
	delete cell;
	if (!rational)
		throw LogicError("Invalid vector format: non-rational element");
	return (rational);
}

static Vector					from_matrix(const Matrix* matrix)
{
	Vector	result;

	if (matrix->getHeight() != 1)
		throw ERROR_UNEXPECTED;
	for (unsigned long int i(0); i < matrix->getWidth(); i++)
		result.push_back(matrix->getValue(i, 0)->clone());
	return (result);
}

static Vector					from_polynomial(const Polynomial* polynomial)
{
	if (polynomial->getDividers().size() != 1 					\
			|| *polynomial->getDividers()[0].coefficient != 1 	\
			|| polynomial->getDividers()[0].power 				\
			|| polynomial->getTerms().size() != 1 				\
			|| polynomial->getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	return (Vector(*polynomial->getTerms()[0].coefficient));
}

static void						print_rounded_vector(const Vector* vector)
{
	unsigned long int	size(vector->size());

	std::cout << "[ ";
	for (unsigned int i = 0; i < size; i++)
	{
		std::cout << vector->getRoundedValue(i);
		if (i < size - 1)
			std::cout << " , ";
	}
	std::cout << " ]";
}


// Constructors and destructor

Vector::Vector(unsigned long int size): _vector()
{
	for (unsigned long int i = 0; i < size; i++)
		this->_vector.push_back(new Rational(0));
}

Vector::Vector(std::string str, t_data &data)
{
	std::vector<std::string>	vector;
	unsigned long int			size;

	vector = parse_vector(str);
	size = vector.size();
	for (unsigned long int i = 0; i < size; i++)
		this->_vector.push_back(value_to_rational(vector[i], data));
}

Vector::Vector(const IType &other): _vector()
{
	const Vector*		other_vector;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;

	other_vector = dynamic_cast<const Vector*>(&other);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_vector)
		*this = *other_vector;
	else if (other_matrix)
		*this = from_matrix(other_matrix);
	else if (other_polynomial)
		*this = from_polynomial(other_polynomial);
	else
		throw ERROR_UNEXPECTED;
}

Vector::~Vector(void)
{
	for (IType* value : this->_vector)
		delete value;
	this->_vector.clear();
}


// Operator overloads

Vector::operator bool() const
{
	for (const IType* value : this->_vector)
		if (value && *value)
			return (true);
	return (false);
}

Vector&		Vector::operator=(const Vector &other)
{
	if (this != &other)
	{
		for (IType* value : this->_vector)
			delete value;
		this->_vector.clear();
		for (IType* value : other._vector)
			this->_vector.push_back(value->clone());
	}
	return (*this);
}

Vector		Vector::operator=(const IType &other)
{
	*this = Vector(other);
	return (*this);
}

bool		Vector::operator==(const IType &other) const
{
	Vector	other_vector;

	try
	{
		other_vector = Vector(other);
	}
	catch(const UnexpectedError &e)
	{
		return (false);
	}
	if (this->_vector.size() != other_vector._vector.size())
		return (false);
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		if (this->_vector[i] != other_vector._vector[i])
			return (false);
	return (true);
}

bool		Vector::operator==(const long long int value) const
{
	return (*this == Rational(value));
}

bool		Vector::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Vector::operator!=(const long long int value) const
{
	return (*this != Rational(value));
}

bool		Vector::operator<(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Vector::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Vector::operator<=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Vector::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Vector::operator>(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Vector::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Vector::operator>=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Vector::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

IType*		Vector::operator[](unsigned long int index) const// [ ] Utile de laisser les 2 ?
{
	if (index >= this->_vector.size())
		throw ERROR_VECTOR_OUT_OF_RANGE;
	return (this->_vector[index]);
}

IType*		Vector::operator[](unsigned long int index)
{
	if (index >= this->_vector.size())
		throw ERROR_VECTOR_OUT_OF_RANGE;
	return (this->_vector[index]);
}

IType*		Vector::operator+(const IType &other) const
{
	const Vector*		other_vector;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Real*			other_real;

	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this + *other_vector);
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
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this + *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Vector*		Vector::operator+(const Vector &other) const
{
	Vector*	result;

	if (this->_vector.size() != other._vector.size())
		throw ERROR_VECTOR_DIMENSIONS;
	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] + *other._vector[i]);
	return (result);
}

Vector*		Vector::operator+(const Rational &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] + other);
	return (result);
}

Vector*		Vector::operator+(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this + rational);
}

Matrix*		Vector::operator+(const Matrix &other) const
{
	return (Matrix(*this) + other);
}

IType*		Vector::operator+(const Polynomial &other) const
{
	return (other + *this);
}

Vector*		Vector::operator+(const Real &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] + other);
	return (result);
}

Vector*		Vector::operator+(const long long int value) const
{
	return (*this + Rational(value));
}

Vector*		Vector::operator-(void) const
{
	return (*this * (-1));
}

IType*		Vector::operator-(const IType &other) const
{
	const Vector*		other_vector;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Real*			other_real;

	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this - *other_vector);
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
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this - *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Vector*		Vector::operator-(const Vector &other) const
{
	Vector*	result;

	if (this->_vector.size() != other._vector.size())
		throw ERROR_VECTOR_DIMENSIONS;
	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] - *other._vector[i]);
	return (result);
}

Vector*		Vector::operator-(const Rational &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] - other);
	return (result);
}

Vector*		Vector::operator-(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this - rational);
}

Matrix*		Vector::operator-(const Matrix &other) const
{
	return (Matrix(*this) - other);
}

IType*		Vector::operator-(const Polynomial &other) const
{
	IType*	tmp;
	IType*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

Vector*		Vector::operator-(const Real &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] - other);
	return (result);
}

Vector*		Vector::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

IType*		Vector::operator*(const IType &other) const
{
	const Vector*		other_vector;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Real*			other_real;

	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this * *other_vector);
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
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this * *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Vector*		Vector::operator*(const Vector &other) const
{
	Vector*	result;

	if (this->_vector.size() != other._vector.size())
		throw ERROR_VECTOR_DIMENSIONS;
	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] + *other._vector[i]);
	return (result);
}

Vector*		Vector::operator*(const Rational &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] * other);
	return (result);
}

Vector*		Vector::operator*(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this * rational);
}

Matrix*		Vector::operator*(const Matrix &other) const
{
	return (Matrix(*this) * other);
}

IType*		Vector::operator*(const Polynomial &other) const
{
	return (other * *this);
}

Vector*		Vector::operator*(const Real &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] * other);
	return (result);
}

Vector*		Vector::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

IType*		Vector::operator/(const IType &other) const
{
	const Vector*		other_vector;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Real*			other_real;

	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this / *other_vector);
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
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this / *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Vector*		Vector::operator/(const Vector &other) const
{
	Vector*	result;

	if (this->_vector.size() != other._vector.size())
		throw ERROR_VECTOR_DIMENSIONS;
	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] / *other._vector[i]);
	return (result);
}

Vector*		Vector::operator/(const Rational &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] / other);
	return (result);
}

Vector*		Vector::operator/(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this / rational);
}

Matrix*		Vector::operator/(const Matrix &other) const
{
	return (Matrix(*this) / other);
}

IType*		Vector::operator/(const Polynomial &other) const
{
	IType*	tmp;
	IType*	result;

	tmp = Rational(1) / other;
	result = *tmp * *this;
	delete tmp;
	return (result);
}

Vector*		Vector::operator/(const Real &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] / other);
	return (result);
}

Vector*		Vector::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

IType*		Vector::operator%(const IType &other) const
{
	const Vector*		other_vector;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Polynomial*	other_polynomial;
	const Real*			other_real;

	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this % *other_vector);
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
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this % *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Vector*		Vector::operator%(const Vector &other) const
{
	Vector*	result;

	if (this->_vector.size() != other._vector.size())
		throw ERROR_VECTOR_DIMENSIONS;
	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] % *other._vector[i]);
	return (result);
}

Vector*		Vector::operator%(const Rational &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] % other);
	return (result);
}

Vector*		Vector::operator%(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this % rational);
}

Matrix*		Vector::operator%(const Matrix &other) const
{
	return (Matrix(*this) % other);
}

Polynomial*	Vector::operator%(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

Vector*		Vector::operator%(const Real &other) const
{
	Vector*	result;

	result = new Vector(this->_vector.size());
	for (unsigned long int i = 0; i < this->_vector.size(); i++)
		result->setValue(i, *this->_vector[i] % other);
	return (result);
}

Vector*		Vector::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

IType*		Vector::operator^(const IType &other) const
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
	if (!power.in_Z() || power < 0)
		throw UNSUPPORTED_EXPONENT;
	return (*this ^ power);
}

Vector*		Vector::operator^(const Rational &other) const
{
	Vector*	result;
	Vector*	tmp;

	result = new Vector();
	for (unsigned int i(0); i < this->_vector.size(); i++)
		result->push_back(new Rational(1));
	for (InfiniteInt i(0); i < other.getNumerator(); i++)
	{
		tmp = result;
		try
		{
			result = *result * *this;
		}
		catch(const LogicError &e)
		{
			delete result;
			throw;
		}
		delete tmp;
	}
	return (result);
}

Vector*		Vector::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
}


// Getters

InfiniteFloat	Vector::getRoundedValue(unsigned long int index) const
{
	if (index >= this->_vector.size())
		throw ERROR_VECTOR_OUT_OF_RANGE;
	return (dynamic_cast<const Rational*>(this->_vector[index])->getValue());// [ ] Changer ca
}


// Setters

void			Vector::setValue(unsigned long int index, IType* value)
{
	if (index >= this->_vector.size())
		throw ERROR_VECTOR_OUT_OF_RANGE;
	delete this->_vector[index];
	this->_vector[index] = value;
}


// Methods

std::ostream&	Vector::print(std::ostream &os) const
{
	os << "[";
	for (unsigned long int i(0); i < this->_vector.size(); i++)
	{
		if (!this->_vector[i])
			throw ERROR_UNEXPECTED;
		os << *this->_vector[i];
		if (i < this->_vector.size() - 1)
			os << ", ";
	}
	os << "]";
	return (os);
}

std::string	Vector::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

std::size_t	Vector::size(void) const
{
	return (this->_vector.size());
}

bool		Vector::empty(void) const
{
	return (this->_vector.empty());
}

bool		Vector::in_D(void) const
{
	for (const IType* value : this->_vector)
		if (!value->in_D())
			return (false);
	return (true);
}

bool		Vector::in_Q(void) const
{
	for (const IType* value : this->_vector)
		if (!value->in_Q())
			return (false);
	return (true);
}

bool		Vector::in_Z(void) const
{
	for (const IType* value : this->_vector)
		if (!value->in_Z())
			return (false);
	return (true);
}

IType*		Vector::clone(void) const
{
	return (new Vector(*this));
}

IType*		Vector::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*		Vector::matrix_inversion(void) const
{
	throw ERROR_MATRIX_INVERSION_SQUARE;
	return (nullptr);
}

IType*		Vector::matrix_operator(const IType &other) const
{
	Matrix	this_matrix(*this);
	Matrix	other_matrix;

	try
	{
		other_matrix = Matrix(other);
	}
	catch (const UnexpectedError &e)
	{
		throw ERROR_MATRIX_OPERATOR;
	}
	return (this_matrix.matrix_operator(other_matrix));
}

IType*		Vector::norm(void) const
{
	return (Matrix(*this).norm());
}

Rational*	Vector::gcd(const IType &other) const
{
	const Vector*	other_vector;
	const Rational*	other_rational;
	const Complex*	other_complex;
	const Matrix*	other_matrix;

	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (this->gcd(*other_vector));
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

Rational*	Vector::gcd(const Vector &other) const
{
	Rational*	result;
	Rational*	tmp;

	if (other.empty())
		return (new Rational(1));
	result = this->gcd(*dynamic_cast<const Rational*>(other[0]));
	for (unsigned long int i(1); i < other.size(); i++)
	{
		tmp = result;
		result = result->gcd(*dynamic_cast<const Rational*>(other[i]));
		delete tmp;
	}
	return (result);
}

Rational*	Vector::gcd(const Rational &other) const
{
	return (other.gcd(*this));
}

Rational*	Vector::gcd(const Complex &other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(*dynamic_cast<const Rational*>(other.getReal()));
	second = first->gcd(*dynamic_cast<const Rational*>(other.getImaginary()));
	delete first;
	return (second);
}

Rational*	Vector::gcd(const Matrix &other) const
{
	Rational*	gcd;
	Rational*	tmp;

	gcd = this->gcd(*dynamic_cast<const Rational*>(other[0][0]));// [ ] Changer ca
	for (unsigned int i = 0; i < other.getHeight(); i++)
		for (unsigned int j = 0; j < other.getWidth(); j++)
		{
			tmp = gcd;
			gcd = gcd->gcd(*dynamic_cast<const Rational*>(other[i][j]));
			delete tmp;
		}
	return (gcd);
}

void		Vector::print_rounded(const std::string var) const
{
	if (this->in_Z())
		return ;
	std::cout << COLOR_DIM;
	if (!var.empty())
	{
		if (this->in_D())
			std::cout << var << " = " << std::endl;
		else
			std::cout << var << " ≈ " << std::endl;
	}
	print_rounded_vector(this);
	std::cout << COLOR_RESET << std::endl;
}

void		Vector::push_back(IType* value)
{
	this->_vector.push_back(value);
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream &os, const Vector &vector)
{
	return (vector.print(os));
}
