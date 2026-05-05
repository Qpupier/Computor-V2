/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:07:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/05 11:15:52 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Matrix.hpp"
#include "AST.hpp"

// Utils
static std::vector<std::string>					parse_line(std::string &line, unsigned long *width)
{
	std::vector<std::string>	row;
	std::size_t					pos;

	line = line.substr(1, line.size() - 2);
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = line.find(',');
		if (pos != std::string::npos)
		{
			row.push_back(line.substr(0, pos));
			line.erase(0, pos + 1);
		}
		else
			row.push_back(line);
	}
	if (row.empty())
		throw LogicError("Invalid matrix format: empty row");
	if (!*width)
		*width = row.size();
	else if (row.size() != *width)
		throw LogicError("Invalid matrix format: inconsistent row sizes");
	return (row);
}

static std::vector<std::vector<std::string>>	parse_matrix(std::string &matrix, unsigned long *width)
{
	std::vector<std::vector<std::string>>	rows;
	std::string								line;
	std::size_t								pos;

	matrix = matrix.substr(1, matrix.size() - 2);
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = matrix.find(';');
		if (pos != std::string::npos)
		{
			line = matrix.substr(0, pos);
			matrix.erase(0, pos + 1);
		}
		else
			line = matrix;
		rows.push_back(parse_line(line, width));
	}
	if (rows.empty())
		throw LogicError("Invalid matrix format: empty matrix");
	return (rows);
}


// Constructors and destructor
Matrix::Matrix(unsigned long width, unsigned long height): _width(width), _height(height)
{
	this->_matrix = new Rational*[this->_height];
	for (unsigned int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = Rational(i == j);
	}
}

Matrix::Matrix(std::string str, t_data &data): _width(0), _height(0)
{
	std::vector<std::vector<std::string>>	rows;
	AST										*cell;
	Rational								*rational;
	unsigned long int						height;

	rows = parse_matrix(str, &this->_width);
	height = rows.size();
	this->_matrix = new Rational*[height];
	for (unsigned int i = 0; i < height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		this->_height++;
		for (unsigned int j = 0; j < this->_width; j++)
		{
			try
			{
				cell = compute_expression(rows[i][j], data, true);
			}
			catch (const std::exception &e)
			{
				this->error(LogicError(std::string("Invalid matrix format: ") + e.what()));
			}
			if (!cell)
				this->error(LogicError("Invalid matrix format: invalid element"));
			if (cell->getLeft() || cell->getRight())
			{
				delete cell;
				this->error(LogicError("Invalid matrix format: incomplete element"));
			}
			rational = dynamic_cast<Rational*>(cell->getNode()->clone());
			delete cell;
			if (!rational)
				this->error(LogicError("Invalid matrix format: non-rational element"));
			this->_matrix[i][j] = *rational;
			delete rational;
		}
	}
}

Matrix::Matrix(const Matrix &other): _width(other._width), _height(other._height)
{
	this->_matrix = new Rational*[this->_height];
	for (unsigned int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = other._matrix[i][j];
	}
}

Matrix::Matrix(const IType &other)
{
	const Matrix		*other_matrix;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	other_rational = dynamic_cast<const Rational*>(&other);
	other_complex = dynamic_cast<const Complex*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_matrix)
		*this = *other_matrix;
	else if (other_rational)
		throw ERROR_UNEXPECTED;
	else if (other_complex)
		throw ERROR_UNEXPECTED;
	else if (other_polynomial)
	{
		if (other_polynomial->getDividers().size() != 1 || *other_polynomial->getDividers()[0].coefficient != Rational(1) || other_polynomial->getDividers()[0].power || other_polynomial->getTerms().size() != 1 || other_polynomial->getTerms()[0].power)
			throw ERROR_UNEXPECTED;//TODO: Size = 0 si polynomial = 0
		*this = Matrix(*other_polynomial->getTerms()[0].coefficient);// FIXME: Leaks?
	}
	else
		throw ERROR_UNEXPECTED;
}

Matrix::~Matrix()
{
	for (unsigned int i = 0; i < this->_height; i++)
		delete[] this->_matrix[i];
	delete[] this->_matrix;
}


// Operator overloads
Matrix&		Matrix::operator=(const Matrix &other)
{
	Matrix	new_matrix(other);

	if (this == &other)
		return (*this);
	this->~Matrix();
	*this = new_matrix;
	return (*this);
}

Matrix::operator bool() const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (this->_matrix[i][j])
				return (true);
	return (false);
}

bool		Matrix::operator==(const Matrix &other) const
{
	if (this->_width != other._width || this->_height != other._height)
		return (false);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (this->_matrix[i][j] != other._matrix[i][j])
				return (false);
	return (true);
}

bool		Matrix::operator==(const Rational &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator==(const Complex &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator==(const Polynomial &other) const
{
	std::vector<Polynomial::t_term>	terms(other.getTerms());
	std::vector<Polynomial::t_term>	dividers(other.getDividers());

	return (terms.size() == 1 && dividers.size() == 1 	\
			&& *terms[0].coefficient == *this 			\
			&& !terms[0].power 							\
			&& *dividers[0].coefficient == Rational(1) 	\
			&& !dividers[0].power);
}

bool		Matrix::operator==(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this == *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this == *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this == *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this == *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (false);
}

bool		Matrix::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Matrix::operator<(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator<=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator>(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator>=(const IType &other) const
{
	(void)other;
	return (false);
}

Rational*	Matrix::operator[](unsigned int index) const
{
	if (index >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[index]);
}

Matrix*		Matrix::operator+(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] + other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator+(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] + other;
	return (result);
}

Matrix*		Matrix::operator+(const Complex &other) const
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

Polynomial*	Matrix::operator+(const Polynomial &other) const
{
	return (other + *this);
}

IType*		Matrix::operator+(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator-(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] - other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator-(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] - other;
	return (result);
}

Matrix*		Matrix::operator-(const Complex &other) const
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

Polynomial*	Matrix::operator-(const Polynomial &other) const
{
	return (other - *this);
}

IType*		Matrix::operator-(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator*(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] * other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator*(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] * other;
	return (result);
}

Matrix*		Matrix::operator*(const Complex &other) const
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

Polynomial*	Matrix::operator*(const Polynomial &other) const
{
	return (other * *this);
}

IType*		Matrix::operator*(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator/(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] / other._matrix[i][j];
			}
			catch(const LogicError& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator/(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] / other;
			}
			catch(const std::exception &e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator/(const Complex &other) const
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

Polynomial*	Matrix::operator/(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), (Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

IType*		Matrix::operator/(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator%(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] % other._matrix[i][j];
			}
			catch(const LogicError& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator%(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] % other;
			}
			catch(const LogicError& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator%(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (*this % rational);
}

Polynomial*	Matrix::operator%(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), (Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

IType*		Matrix::operator%(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;
	const Polynomial	*other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*		Matrix::operator^(const IType &other) const
{
	Rational	power;
	Matrix*		result;
	Matrix*		tmp;

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
	result = new Matrix(this->_width, this->_height);
	for (int i = 0; i < power.getNumerator(); i++)
	{
		tmp = result;
		result = result->matrix_operator(*this);
		delete tmp;
	}
	return (result);
}


// Getters
unsigned long	Matrix::getWidth(void) const
{
	return (this->_width);
}

unsigned long	Matrix::getHeight(void) const
{
	return (this->_height);
}


// Setters
void	Matrix::setValue(unsigned int i, unsigned int j, Rational *value)
{
	if (i >= this->_height || j >= this->_width)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	this->_matrix[i][j] = value;
}


// Methods
Matrix*			Matrix::matrix_operator(const Matrix &other) const
{
	Matrix		*result;
	Rational	*cell;
	Rational	*mul;
	Rational	*tmp;

	if (this->_width != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(other._width, this->_height);
	for (unsigned int i = 0; i < result->_height; i++)
		for (unsigned int j = 0; j < result->_width; j++)
		{
			cell = new Rational();
			for (unsigned int k = 0; k < this->_width; k++)
			{
				mul = this->_matrix[i][k] * other._matrix[k][j];
				tmp = cell;
				cell = *cell + *mul;
				delete tmp;
				delete mul;
			}
			result->_matrix[i][j] = cell;
		}
	return (result);
}

Matrix*			Matrix::matrix_operator(const Rational &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*			Matrix::matrix_operator(const Complex &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Matrix::matrix_operator(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->matrix_operator(*other_matrix));
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->matrix_operator(*other_rational));
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->matrix_operator(*other_complex));
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

IType*			Matrix::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Matrix::clone(void) const
{
	return (new Matrix(*this));
}

Rational*		Matrix::gcd(const Rational &other) const
{
	return (other.gcd(*this));
}

Rational*		Matrix::gcd(const Complex &other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(other.getReal());
	second = first->gcd(other.getImaginary());
	delete first;
	return (second);
}

Rational*		Matrix::gcd(const Matrix &other) const
{
	Rational*	gcd;
	Rational*	tmp;

	gcd = this->gcd(other[0][0]);
	for (unsigned int i = 0; i < other.getHeight(); i++)
		for (unsigned int j = 0; j < other.getWidth(); j++)
		{
			tmp = gcd;
			gcd = gcd->gcd(other[i][j]);
			delete tmp;
		}
	return (gcd);
}

Rational*		Matrix::gcd(const IType &other) const
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

std::ostream&	Matrix::print(std::ostream &os) const
{
	unsigned long	width;
	unsigned long	height;

	width = this->getWidth();
	height = this->getHeight();
	for (unsigned int i = 0; i < height; i++)
	{
		os << "[ ";
		for (unsigned int j = 0; j < width; j++)
		{
			os << this->_matrix[i][j];
			if (j < width - 1)
				os << " , ";
		}
		os << " ]";
		if (i < height - 1)
			os << std::endl;
	}
	return (os);
}

std::ostream&	Matrix::print_polynomial(std::ostream &os, 	\
		const std::string &var) const
{
	return (os << *this << var);
}

void			Matrix::error(const LogicError &e) const
{
	this->~Matrix();
	throw e;
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix)
{
	return (matrix.print(os));
}
