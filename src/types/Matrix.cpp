/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:07:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/11 16:59:15 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Matrix.hpp"
#include "AST.hpp"

// Utils

static std::vector<std::string>					parse_line(				\
		std::string &line, unsigned long long int * width)
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

static std::vector<std::vector<std::string>>	parse_matrix(			\
		std::string &matrix, unsigned long long int * width)
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

static Matrix									from_polynomial(		\
		const Polynomial *polynomial)
{
	if (polynomial->getDividers().size() != 1 					\
			|| *polynomial->getDividers()[0].coefficient != 1 	\
			|| polynomial->getDividers()[0].power 				\
			|| polynomial->getTerms().size() != 1 				\
			|| polynomial->getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	return (Matrix(*polynomial->getTerms()[0].coefficient));
}

static Rational*								value_to_rational(		\
		std::string value, t_data &data, Matrix *matrix)
{
	AST*		cell;
	Rational*	rational;

	try
	{
		cell = compute_expression(value, data, true);
	}
	catch (const std::exception &e)
	{
		matrix->error(	\
				LogicError(std::string("Invalid matrix format: ") + e.what()));
	}
	if (!cell)
		matrix->error(LogicError("Invalid matrix format: invalid element"));
	if (cell->getLeft() || cell->getRight())
	{
		delete cell;
		matrix->error(LogicError("Invalid matrix format: incomplete element"));
	}
	rational = dynamic_cast<Rational*>(cell->getNode()->clone());
	delete cell;
	if (!rational)
		matrix->error(	\
				LogicError("Invalid matrix format: non-rational element"));
	return (rational);
}

static void										print_rounded_matrix(	\
		const Matrix *matrix)
{
	unsigned long	width;
	unsigned long	height;

	width = matrix->getWidth();
	height = matrix->getHeight();
	for (unsigned int j = 0; j < height; j++)
	{
		std::cout << "[ ";
		for (unsigned int i = 0; i < width; i++)
		{
			std::cout << matrix->getRoundedValue(i, j);
			if (i < width - 1)
				std::cout << " , ";
		}
		std::cout << " ]";
		if (j < height - 1)
			std::cout << std::endl;
	}
}


// Constructors and destructor

Matrix::Matrix(unsigned long long int width, 	\
		unsigned long long int height): _width(width), _height(height)
{
	if (!width || !height)
	{
		this->_width = 0;
		this->_height = 0;
		this->_matrix = nullptr;
		return ;
	}
	this->_matrix = new Rational*[this->_height];
	for (unsigned long long int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned long long int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = Rational(i == j);
	}
}

Matrix::Matrix(std::string str, t_data &data): _width(0), _height(0)
{
	std::vector<std::vector<std::string>>	rows;
	Rational*								rational;
	unsigned long long int					height;

	rows = parse_matrix(str, &this->_width);
	height = rows.size();
	this->_matrix = new Rational*[height];
	for (unsigned long long int i = 0; i < height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		this->_height++;
		for (unsigned long long int j = 0; j < this->_width; j++)
		{
			rational = value_to_rational(rows[i][j], data, this);
			this->_matrix[i][j] = *rational;
			delete rational;
		}
	}
}

Matrix::Matrix(const Matrix &other): _width(other._width), _height(other._height)
{
	this->_matrix = new Rational*[this->_height];
	for (unsigned long long int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned long long int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = other._matrix[i][j];
	}
}

Matrix::Matrix(const IType &other): Matrix()
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

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
		*this = from_polynomial(other_polynomial);
	else
		throw ERROR_UNEXPECTED;
}

Matrix::~Matrix(void)
{
	this->free();
}


// Operator overloads

Matrix::operator bool() const
{
	return (this->_width && this->_height);
}

Matrix&		Matrix::operator=(const Matrix &other)
{
	if (this == &other)
		return (*this);
	this->free();
	this->_width = other._width;
	this->_height = other._height;
	this->_matrix = new Rational*[other._height];
	for (unsigned int i = 0; i < other._height; i++)
	{
		this->_matrix[i] = new Rational[other._width];
		for (unsigned int j = 0; j < other._width; j++)
			this->_matrix[i][j] = other._matrix[i][j];
	}
	return (*this);
}

Matrix		Matrix::operator=(const IType &other)
{
	*this = Matrix(other);
	return (*this);
}

bool		Matrix::operator==(const IType &other) const
{
	Matrix	other_matrix;

	try
	{
		other_matrix = Matrix(other);
	}
	catch(const UnexpectedError &e)
	{
		return (false);
	}
	if (this->_width != other_matrix._width 	\
			|| this->_height != other_matrix._height)
		return (false);
	for (unsigned long long int j = 0; j < this->_height; j++)
		for (unsigned long long int i = 0; i < this->_width; i++)
			if (this->_matrix[j][i] != other_matrix._matrix[j][i])
				return (false);
	return (true);
}

bool		Matrix::operator==(const long long int value) const
{
	return (*this == Rational(value));
}

bool		Matrix::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Matrix::operator!=(const long long int value) const
{
	return (*this != Rational(value));
}

bool		Matrix::operator<(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Matrix::operator<=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Matrix::operator>(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Matrix::operator>=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

Rational*	Matrix::operator[](unsigned long long int index) const
{
	if (index >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[index]);
}

Rational*	Matrix::operator[](unsigned long long int index)
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
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] + other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator+(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
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
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

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

Matrix*		Matrix::operator+(const long long int value) const
{
	return (*this + Rational(value));
}

Matrix*		Matrix::operator-(void) const
{
	return (*this * (-1));
}

Matrix*		Matrix::operator-(const Matrix &other) const
{
	Matrix*	result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] - other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator-(const Rational &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
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
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

IType*		Matrix::operator-(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

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

Matrix*		Matrix::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

Matrix*		Matrix::operator*(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] * other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator*(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
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
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

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

Matrix*		Matrix::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

Matrix*		Matrix::operator/(const Matrix &other) const
{
	Matrix*	result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] 	\
						/ other._matrix[i][j];
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
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
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

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

IType*		Matrix::operator/(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

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

Matrix*		Matrix::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

Matrix*		Matrix::operator%(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] 	\
						= this->_matrix[i][j] % other._matrix[i][j];
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
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long long int i = 0; i < this->_height; i++)
		for (unsigned long long int j = 0; j < this->_width; j++)
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

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

IType*		Matrix::operator%(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;

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

Matrix*		Matrix::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

Matrix*		Matrix::operator^(const Rational &other) const
{
	Matrix*	result;
	Matrix*	tmp;

	result = new Matrix(this->_width, this->_height);
	for (InfiniteInt i(0); i < other.getNumerator(); i++)
	{
		tmp = result;
		try
		{
			result = result->matrix_operator(*this);
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

IType*		Matrix::operator^(const IType &other) const
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

Matrix*		Matrix::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
}


// Getters

Rational				Matrix::getValue(unsigned long long int i, 		\
		unsigned long long int j) const
{
	if (i >= this->_width || j >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[j][i]);
}

InfiniteDouble		Matrix::getRoundedValue(unsigned long long int i, 	\
		unsigned long long int j) const
{
	if (i >= this->_width || j >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[j][i].getValue());
}

unsigned long long int	Matrix::getWidth(void) const
{
	return (this->_width);
}

unsigned long long int	Matrix::getHeight(void) const
{
	return (this->_height);
}


// Setters
void	Matrix::setValue(unsigned long long int i, 	\
		unsigned long long int j, Rational *value)
{
	if (i >= this->_height || j >= this->_width)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	this->_matrix[i][j] = value;
}


// Methods

Matrix*			Matrix::matrix_operator(const Matrix &other) const
{
	Matrix*		result;
	Rational*	cell;
	Rational*	mul;
	Rational*	tmp;

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

IType*			Matrix::matrix_operator(const IType &other) const
{
	Matrix	other_matrix;

	try
	{
		other_matrix = Matrix(other);
	}
	catch (const UnexpectedError &e)
	{
		throw ERROR_MATRIX_OPERATOR;
	}
	return (this->matrix_operator(other_matrix));
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

std::ostream&	Matrix::print(std::ostream &os) const
{
	unsigned long	width;
	unsigned long	height;

	width = this->_width;
	height = this->_height;
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

bool			Matrix::finite_decimals(void) const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (!this->_matrix[i][j].finite_decimals())
				return (false);
	return (true);
}

bool			Matrix::values_in_D(void) const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (!this->_matrix[i][j].in_D())
				return (false);
	return (true);
}

bool			Matrix::values_in_Z(void) const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (!this->_matrix[i][j].in_Z())
				return (false);
	return (true);
}

void			Matrix::error(const LogicError &e)
{
	this->free();
	throw e;
}

void			Matrix::free(void)
{
	if (!this->_matrix || !this->_width || !this->_height)
		return ;
	for (unsigned int i = 0; i < this->_height; i++)
		delete[] this->_matrix[i];
	delete[] this->_matrix;
	this->_width = 0;
	this->_height = 0;
	this->_matrix = nullptr;
}

void			Matrix::print_rounded(const std::string var) const
{
	if (this->values_in_Z())
		return ;
	std::cout << COLOR_DIM;
	if (!var.empty())
	{
		if (this->values_in_D())
			std::cout << var << " = " << std::endl;
		else
			std::cout << var << " ≈ " << std::endl;
	}
	print_rounded_matrix(this);
	std::cout << COLOR_RESET << std::endl;
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream &os, const Matrix &matrix)
{
	return (matrix.print(os));
}


// Functions

bool	is_matrix(const IType& type)
{
	Matrix	matrix;

	try
	{
		matrix = Matrix(type);
	}
	catch (const UnexpectedError &e)
	{
		return (false);
	}
	return (true);
}
